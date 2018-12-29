#include "BloodpressureModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;
using namespace bsn::msg::control;

BloodpressureModule::BloodpressureModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bloodpressure"),
    buffer(),
    type("bloodpressure"),
    battery(100),
    active(true),
    params({{"freq",0.1},{"m_avg",5}}),
    markovSystolic(),
    markovDiastolic(),
    filterSystolic(5),
    filterDiastolic(5),
    sensorConfigSystolic(),
    sensorConfigDiastolic() {}

BloodpressureModule::~BloodpressureModule() {}

void BloodpressureModule::setUp() {
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

        /**
         * Configure markov chain
         */ 
        {
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

        /**
         * Configure sensor configuration
         */
        {
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
}

void BloodpressureModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BloodpressureModule::body(){

    Container container;
    double dataS;
    double dataD;
    double risk;
    double nCycles = 0;
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        /*
         * Receive control command and module update
         */
        while(!buffer.isEmpty()){
            container = buffer.leave();

            active = container.getData<BloodpressureControlCommand>().getActive();
            params = container.getData<BloodpressureControlCommand>().getParams();
        }

        if(!active){ continue; }

        /*
         * Module execution
         */
        if(++nCycles >= int32_t(1.0/params["freq"])){
            
            //for debugging 
            cout << "Estado atual (systolic): " << markovSystolic.currentState << endl;
            cout << "Estado atual (diastolic): " << markovDiastolic.currentState << endl;
            
            /*
             * TASK: Collect bloodpressure data
             */
            dataS = markovSystolic.calculate_state();      
            markovSystolic.next_state();
            battery -= 0.001;
            dataD = markovDiastolic.calculate_state();      
            markovDiastolic.next_state();
            battery -= 0.001;
            // TODO: send cost and reliability from "data generation" to controller
            
            //for debugging 
            cout << "Dado gerado (systolic): " << dataS << endl;
            cout << "Dado gerado (diastolic): " << dataD << endl;

            /*
             * TASK: Filter data with moving average
             */
            filterSystolic.setRange(params["m_avg"]);
            filterSystolic.insert(dataS, "bpms");
            dataS = filterSystolic.getValue("bpms");
            battery -= 0.005*params["m_avg"];

            filterDiastolic.setRange(params["m_avg"]);
            filterDiastolic.insert(dataD, "bpmd");
            dataD = filterDiastolic.getValue("bpmd");
            battery -= 0.005*params["m_avg"];
            // TODO: send cost and reliability from "filtering" to controller

            //for debugging 
            cout << "Dado filtrado (systolic): " << dataS << endl;
            cout << "Dado filtrado (diastolic): " << dataD << endl;

            /*
             * TASK: Transfer information to CentralHub
             */
            risk = sensorConfigSystolic.evaluateNumber(dataS);

            SensorData sdataS("bpms", dataS, risk);
            Container sdataSContainer(sdataS);
            getConference().send(sdataSContainer);
            battery -= 0.01;

            // for debugging
            cout << sdataS.toString() << endl;

            risk = sensorConfigDiastolic.evaluateNumber(dataD);
            SensorData sdataD("bpmd", dataD, risk);
            Container sdatadContainer(sdataD);
            getConference().send(sdatadContainer);
            battery -= 0.01;

            // for debugging
            cout << sdataD.toString() << endl;

            // TODO: send cost and reliability from "transfering" to controller      

            nCycles = 0;
        }

        // TODO: persist it here
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
