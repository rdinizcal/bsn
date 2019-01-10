#include "OximeterModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;
using namespace bsn::msg::info;
using namespace bsn::msg::control;

OximeterModule::OximeterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "oximeter"),
    buffer(),
    type("oximeter"),
    battery(1),
    available(true),
    data_accuracy(1),
    comm_accuracy(1),
    active(true),
    params({{"freq",0.90},{"m_avg",5}}),
    markov(),
    filter(5),
    sensorConfig(),
    persist(1),
    path("oximeter_output.csv"),
    fp() {}

OximeterModule::~OximeterModule() {}

void OximeterModule::setUp() {
    srand(time(NULL));
    addDataStoreFor(901, buffer);
    
    Operation op;
    
    std::vector<string> t_probs;
    std::array<float, 25> transitions;
    std::array<bsn::range::Range,5> ranges;

    for(uint32_t i = 0; i < transitions.size(); i++){
        for(uint32_t j = 0; j < 5; j++){
            t_probs = op.split(getKeyValueConfiguration().getValue<std::string>("oximeter.state"+to_string(j)), ',');
            for(uint32_t k = 0; k < 5; k++){
                transitions[i++] = stod(t_probs[k]);
            }
        }
    }
    
    { // Configure markov chain
        vector<string> lrs,mrs,hrs;

        lrs = op.split(getKeyValueConfiguration().getValue<string>("oximeter.LowRisk"), ',');
        mrs = op.split(getKeyValueConfiguration().getValue<string>("oximeter.MidRisk"), ',');
        hrs = op.split(getKeyValueConfiguration().getValue<string>("oximeter.HighRisk"), ',');

        ranges[0] = Range(-1, -1);
        ranges[1] = Range(-1, -1);
        ranges[2] = Range(stod(lrs[0]),  stod(lrs[1]));
        ranges[3] = Range(stod(mrs[0]), stod(mrs[1]));
        ranges[4] = Range(stod(hrs[0]), stod(hrs[1]));

        markov = Markov(transitions, ranges, 2);
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

        sensorConfig = SensorConfiguration(0,low_range,midRanges,highRanges,percentages);
    }

    { // Configure sensor data_accuracy
        data_accuracy = getKeyValueConfiguration().getValue<double>("oximeter.data_accuracy") / 100;
        comm_accuracy = getKeyValueConfiguration().getValue<double>("oximeter.data_accuracy") / 100;
    }

    { // Configure sensor persistency
        persist = getKeyValueConfiguration().getValue<int>("oximeter.persist");
        path = getKeyValueConfiguration().getValue<std::string>("oximeter.path");

        if (persist) {
            fp.open(path);
            fp << "ID,DATA,RISK" << endl;
        }
    }
}

void OximeterModule::tearDown() {
    if (persist)
        fp.close();
}

void OximeterModule::sendTaskInfo(const std::string &task_id, const double &cost, const double &reliability, const double &frequency) {
    TaskInfo task(task_id, cost, reliability, frequency);
    Container taskContainer(task);
    getConference().send(taskContainer);
}

void OximeterModule::sendContextInfo(const std::string &context_id, const bool &value) {
    ContextInfo context(context_id, value);
    Container contextContainer(context);
    getConference().send(contextContainer);
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode OximeterModule::body(){

    Container container;
    double data;
    double risk;
    bool first_exec = true;
    double accuracyValue;
    double offset;
    int id = 0;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        if(first_exec){ // Send context info warning controller that this sensor is available
            sendContextInfo("SaO2_available",true);
            sendTaskInfo("G3_T1.11",0.001,data_accuracy,params["freq"]);
            sendTaskInfo("G3_T1.12",0.005*params["m_avg"],1,params["freq"]);
            sendTaskInfo("G3_T1.13",0.01,comm_accuracy,params["freq"]);
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
            sendContextInfo("SaO2_available",active);
        }

        /*
         * Receive control command and module update
         */
        while(!buffer.isEmpty()){
            container = buffer.leave();

            active = container.getData<OximeterControlCommand>().getActive();
            params["freq"] = container.getData<OximeterControlCommand>().getFrequency();
            cout << "New frequency: " << params["freq"] * 100 << endl;
        }

        if(!active){ 
            if(battery <= 1) battery += 0.05;
            continue; 
        }

        /*
         * Module execution
         */
        if((rand() % 100)+1 < int32_t(params["freq"]*100)){
            
            { // TASK: Collect oximeter data
                double offset = (1 - data_accuracy + (double)rand() / RAND_MAX * (1 - data_accuracy)) * data;

                data = markov.calculate_state();

                if (rand() % 2 == 0)
                    data = data + offset;
                else
                    data = data - offset;

                markov.next_state();
                battery -= 0.001;
                

                //for debugging 
                cout << "Dado gerado: " << data << endl;
            }
            
            { // TASK: Filter data with moving average
                filter.setRange(params["m_avg"]);
                filter.insert(data, type);
                data = filter.getValue(type);
                battery -= 0.005*params["m_avg"];


                //for debugging 
                cout << "Dado filtrado: " << data << endl;
            }
            
            { // TASK: Transfer information to CentralHub
                risk = sensorConfig.evaluateNumber(data);
                
                SensorData sdata(type, data, risk);
                Container sdataContainer(sdata);
                if((rand() % 100)+1 > int32_t(comm_accuracy*100)) getConference().send(sdataContainer);
                battery -= 0.003;

                // for debugging
                cout << "Risk: " << risk << "%"  << endl;
            }

        }

        { // Persist sensor data
            if (persist) {
              fp << id++ << ",";
              fp << data << ",";
              fp << risk << endl;
           }
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}