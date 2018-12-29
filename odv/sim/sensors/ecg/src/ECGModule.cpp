#include "ECGModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;

using namespace bsn::msg::data;
using namespace bsn::msg::control;

ECGModule::ECGModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ecg"),
    buffer(),
    type("ecg"),
    battery(100),
    active(true),
    params({{"freq",0.1},{"m_avg",5}}),
    markov(),
    filter(5) {}

ECGModule::~ECGModule() {}

void ECGModule::setUp() {
    addDataStoreFor(902, buffer);
    
    Operation op;
    
    std::vector<string> t_probs;
    std::array<float, 25> transitions;
    std::array<bsn::range::Range,5> ranges;

    for(uint32_t i = 0; i < transitions.size(); i++){
        for(uint32_t j = 0; j < 5; j++){
            t_probs = op.split(getKeyValueConfiguration().getValue<std::string>("ecg.state"+to_string(j)), ',');
            for(uint32_t k = 0; k < 5; k++){
                transitions[i++] = stod(t_probs[k]);
            }
        }
    }
    
    vector<string> lrs,mrs0,hrs0,mrs1,hrs1;

    lrs = op.split(getKeyValueConfiguration().getValue<string>("ecg.LowRisk"), ',');
    mrs0 = op.split(getKeyValueConfiguration().getValue<string>("ecg.MidRisk0"), ',');
    hrs0 = op.split(getKeyValueConfiguration().getValue<string>("ecg.HighRisk0"), ',');
    mrs1 = op.split(getKeyValueConfiguration().getValue<string>("ecg.MidRisk1"), ',');
    hrs1 = op.split(getKeyValueConfiguration().getValue<string>("ecg.HighRisk1"), ',');

    ranges[0] = Range(stod(hrs0[0]), stod(hrs0[1]));
    ranges[1] = Range(stod(mrs0[0]), stod(mrs0[1]));
    ranges[2] = Range(stod(lrs[0]),  stod(lrs[1]));
    ranges[3] = Range(stod(mrs1[0]), stod(mrs1[1]));
    ranges[4] = Range(stod(hrs1[0]), stod(hrs1[1]));

    markov = Markov(transitions, ranges, 2);
}

void ECGModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ECGModule::body(){

    double data;
    Container container;
    double nCycles = 0;
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        /*
         * Receive control command and module update
         */
        while(!buffer.isEmpty()){
            container = buffer.leave();

            active = container.getData<ECGControlCommand>().getActive();
            params = container.getData<ECGControlCommand>().getParams();
        }

        if(!active){
            continue;
        }

        /*
         * Module execution
         */
        if(++nCycles >= int32_t(1.0/params["freq"])){
            
            //for debugging 
            cout << "Estado atual: " << markov.currentState << endl;
             
            /*
             * TASK: Collect ecg data
             */
            data = markov.calculate_state();      
            markov.next_state();
            battery -= 0.001;
            // TODO: send cost and reliability from "data generation" to controller
            
            //for debugging 
            cout << "Dado gerado: " << data << endl;

            /*
             * TASK: Filter data with moving average
             */
            filter.setRange(params["m_avg"]);
            filter.insert(data, type);
            double fData = filter.getValue(type);
            battery -= 0.005*params["m_avg"];
            // TODO: send cost and reliability from "filtering" to controller

            //for debugging 
            cout << "Dado filtrado: " << fData << endl;

            /*
             * TASK: Transfer information to CentralHub
             */
            SpecData sdata(data, type);
            Container sdataContainer(sdata);
            getConference().send(sdataContainer);
            battery -= 0.01;
            // TODO: send cost and reliability from "transfering" to controller      

            nCycles = 0;
        }

        // TODO: persist it here
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
