#include "BloodpressureModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;
using namespace bsn::msg::info;
using namespace bsn::msg::control;

BloodpressureModule::BloodpressureModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bloodpressure"),
    buffer(),
    type("bloodpressure"),
    battery(1),
    available(true),
    diasdata_accuracy(1),
    diascomm_accuracy(1),
    systdata_accuracy(1),
    systcomm_accuracy(1),
    active(true),
    params({{"freq",0.90},{"m_avg",5}}),
    markovSystolic(),
    markovDiastolic(),
    filterSystolic(5),
    filterDiastolic(5),
    sensorConfigSystolic(),
    sensorConfigDiastolic(),
    persist(1),
    path("bloodpressure_output.csv"),
    fp() {}

BloodpressureModule::~BloodpressureModule() {}

void BloodpressureModule::setUp() {
    srand(time(NULL));
    addDataStoreFor(903, buffer);
    
    Operation op;
    
    for(int32_t i = 0; i < 2; i++){
        std::vector<string> t_probs;
        std::array<float, 25> transitions;
        std::array<bsn::range::Range,5> ranges;
        vector<string> lrs,mrs,hrs;
        
        std::string x = (i==0)? "syst" : "dias";

        for(uint32_t i = 0; i < transitions.size(); i++){
            for(uint32_t j = 0; j < 5; j++){
                t_probs = op.split(getKeyValueConfiguration().getValue<std::string>("bloodpressure."+x+"state"+to_string(j)), ',');
                for(uint32_t k = 0; k < 5; k++){
                    transitions[i++] = stod(t_probs[k]);
                }
            }
        }

        { // Configure markov chain
            lrs = op.split(getKeyValueConfiguration().getValue<string>("bloodpressure."+x+"LowRisk"), ',');
            mrs = op.split(getKeyValueConfiguration().getValue<string>("bloodpressure."+x+"MidRisk"), ',');
            hrs = op.split(getKeyValueConfiguration().getValue<string>("bloodpressure."+x+"HighRisk"), ',');

            ranges[0] = Range(-1, -1);
            ranges[1] = Range(-1, -1);
            ranges[2] = Range(stod(lrs[0]), stod(lrs[1]));
            ranges[3] = Range(stod(mrs[0]), stod(mrs[1]));
            ranges[4] = Range(stod(hrs[0]), stod(hrs[1]));
            
            if(i==0){
                markovSystolic = Markov(transitions, ranges, 2);
            } else {
                markovDiastolic = Markov(transitions, ranges, 2);
            }
        }

        { // Configure sensor configuration
            Range low_range = ranges[2];
            
            array<Range,2> midRanges;
            midRanges[0] = ranges[1];
            midRanges[1] = ranges[3];
            
            array<Range,2> highRanges;
            highRanges[0] = ranges[0];
            highRanges[1] = ranges[4];

            array<Range,3> percentages;

            vector<string> low_p = op.split(getKeyValueConfiguration().getValue<string>("global.lowrisk"), ',');
            percentages[0] = Range(stod(low_p[0]),stod(low_p[1]));

            vector<string> mid_p = op.split(getKeyValueConfiguration().getValue<string>("global.midrisk"), ',');
            percentages[1] = Range(stod(mid_p[0]),stod(mid_p[1]));

            vector<string> high_p = op.split(getKeyValueConfiguration().getValue<string>("global.highrisk"), ',');
            percentages[2] = Range(stod(high_p[0]),stod(high_p[1]));

            if(i==0){
                sensorConfigSystolic = SensorConfiguration(0,low_range,midRanges,highRanges,percentages);
            } else {
                sensorConfigDiastolic = SensorConfiguration(0,low_range,midRanges,highRanges,percentages);
            }
        }
    }

    { // Configure sensor accuracy
        diasdata_accuracy = getKeyValueConfiguration().getValue<double>("bloodpressure.diasdata_accuracy") / 100;
        diascomm_accuracy = getKeyValueConfiguration().getValue<double>("bloodpressure.diascomm_accuracy") / 100;
        systdata_accuracy = getKeyValueConfiguration().getValue<double>("bloodpressure.systdata_accuracy") / 100;
        systcomm_accuracy = getKeyValueConfiguration().getValue<double>("bloodpressure.systcomm_accuracy") / 100;
    }

    { // Configure sensor persistency
        persist = getKeyValueConfiguration().getValue<int>("bloodpressure.persist");
        path = getKeyValueConfiguration().getValue<std::string>("bloodpressure.path");

        if (persist) {
            fp.open(path);
            fp << "ID,SYSTOLIC_DATA,DIASTOLIC_DATA,RISK" << endl;
        }
    }
}

void BloodpressureModule::tearDown() {
    if (persist)
        fp.close();
}

void BloodpressureModule::sendTaskInfo(const std::string &task_id, const double &cost, const double &reliability, const double &frequency) {
    TaskInfo task(task_id, cost, reliability, frequency);
    Container taskContainer(task);
    getConference().send(taskContainer);
}

void BloodpressureModule::sendContextInfo(const std::string &context_id, const bool &value) {
    ContextInfo context(context_id, value);
    Container contextContainer(context);
    getConference().send(contextContainer);
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BloodpressureModule::body(){

    Container container;
    double dataS;
    double dataD;
    double risk;
    bool first_exec = true;
    double accuracyValue;
    double offset;
    int id = 0;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        if(first_exec){ // Send context info warning controller that this sensor is available
            sendContextInfo("ABP_available",true);
            sendTaskInfo("G3_T1.411",0.001,systdata_accuracy,params["freq"]);
            sendTaskInfo("G3_T1.412",0.001,diasdata_accuracy,params["freq"]);
            sendTaskInfo("G3_T1.42",0.005*params["m_avg"]*2,1,params["freq"]);
            sendTaskInfo("G3_T1.43",0.01,(systcomm_accuracy+diascomm_accuracy)/2,params["freq"]);
            first_exec = false; 
        }

        { // recharge routine
            //for debugging
            cout << "Battery level: " << battery*100 << "%" << endl;
            if(!active && battery > 0.8){
                active = true;
            }
            if(active && battery < 0.02){
                active = false;
            }
            sendContextInfo("ABP_available", active);
        }

        /*
         * Receive control command and module update
         */
        while(!buffer.isEmpty()){
            container = buffer.leave();

            active = container.getData<BloodpressureControlCommand>().getActive();
            params["freq"] = container.getData<BloodpressureControlCommand>().getFrequency();
            cout << "New frequency: " << params["freq"]*100 << endl;
        }

        if(!active){ 
            if(battery <= 1) battery += 0.05;
            continue; 
        }

        /*
         * Module execution
         */
        if((rand() % 100)+1 < int32_t(params["freq"]*100)){
            
            { // TASK: Collect bloodpressure data
                double offset = (1 - systdata_accuracy + (double)rand() / RAND_MAX * (1 - systdata_accuracy)) * dataS;

                dataS = markovSystolic.calculate_state();      

                if (rand() % 2 == 0)
                    dataS = dataS + offset;
                else
                    dataS = dataS - offset;

                markovSystolic.next_state();
                battery -= 0.001;

                offset = (1 - diasdata_accuracy + (double)rand() / RAND_MAX * (1 - diasdata_accuracy)) * dataD;
                
                dataD = markovDiastolic.calculate_state();


                if (rand() % 2 == 0)
                    dataD = dataD + offset;
                else
                    dataD = dataD - offset;

                markovDiastolic.next_state();
                battery -= 0.001;
                

                //for debugging 
                cout << "New data (systolic): " << dataS << endl;
                cout << "New data (diastolic): " << dataD << endl;
            }

            { // TASK: Filter data with moving average
                filterSystolic.setRange(params["m_avg"]);
                filterSystolic.insert(dataS, "bpms");
                dataS = filterSystolic.getValue("bpms");
                battery -= 0.005*params["m_avg"];

                filterDiastolic.setRange(params["m_avg"]);
                filterDiastolic.insert(dataD, "bpmd");
                dataD = filterDiastolic.getValue("bpmd");
                battery -= 0.005*params["m_avg"];


                //for debugging 
                cout << "Filtered data (systolic): " << dataS << endl;
                cout << "Filtered data (diastolic): " << dataD << endl;
            }
            
            { //TASK: Transfer information to CentralHub
                risk = sensorConfigSystolic.evaluateNumber(dataS);
                SensorData sdataS("bpms", dataS, risk);
                Container sdataSContainer(sdataS);
                if((rand() % 100)+1 > int32_t(systcomm_accuracy*100))getConference().send(sdataSContainer);
                battery -= 0.003;

                // for debugging
                cout << "Risk: " << risk << "%"  << endl;

                risk = sensorConfigDiastolic.evaluateNumber(dataD);
                SensorData sdataD("bpmd", dataD, risk);
                Container sdatadContainer(sdataD);
                if((rand() % 100)+1 > int32_t(diascomm_accuracy*100)) getConference().send(sdatadContainer);
                battery -= 0.003;

                // for debugging
                cout << "Risk: " << risk << "%"  << endl;
            }

        }

        { // Persist sensor data
            if (persist) {
              fp << id++ << ",";
              fp << dataS << ",";
              fp << dataD << ",";
              fp << risk << endl;
            }
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
