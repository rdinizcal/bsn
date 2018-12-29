#include "BloodpressureModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;

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
    filterDiastolic(5) {}

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
}

void BloodpressureModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BloodpressureModule::body(){

    double dataS;
    double dataD;
    Container container;
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

        if(!active){
            continue;
        }

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
            filterSystolic.insert(dataS, type);
            double fDataS = filterSystolic.getValue(type);
            battery -= 0.005*params["m_avg"];

            filterDiastolic.setRange(params["m_avg"]);
            filterDiastolic.insert(dataD, type);
            double fDataD = filterDiastolic.getValue(type);
            battery -= 0.005*params["m_avg"];
            // TODO: send cost and reliability from "filtering" to controller

            //for debugging 
            cout << "Dado filtrado (systolic): " << fDataS << endl;
            cout << "Dado filtrado (diastolic): " << fDataD << endl;

            /*
             * TASK: Transfer information to CentralHub
             */
            SpecData sdatas(fDataS, type);
            Container sdatasContainer(sdatas);
            getConference().send(sdatasContainer);
            battery -= 0.01;

            SpecData sdatad(fDataD, type);
            Container sdatadContainer(sdatad);
            getConference().send(sdatadContainer);
            battery -= 0.01;
            // TODO: send cost and reliability from "transfering" to controller      

            nCycles = 0;
        }

        // TODO: persist it here
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
