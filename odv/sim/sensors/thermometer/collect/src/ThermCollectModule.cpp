#include "ThermCollectModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::taskMsg;

ThermCollectModule::ThermCollectModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "thermometer"),
    buffer(),
    type("thermometer"),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    markov()
    {}

ThermCollectModule::~ThermCollectModule() {}

void ThermCollectModule::setUp() {
    //srand(time(NULL));
    addDataStoreFor(THERMCOLLECTMODULE_MSG_QUE, buffer);
    
    // Configure sensor configuration    
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
  
    { // Configure markov chain
        std::vector<string> lrs,mrs0,hrs0,mrs1,hrs1;

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
}

void ThermCollectModule::tearDown() {
   
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ThermCollectModule::body(){

    double data;
    int i = 0;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        
        // Apenas executa uma vez a cada segundo
        while(i > 10){ // Receive control command and module update
            data = markov.calculate_state();
            markov.next_state();      

            ThermCollectTaskMessage collectMsg(data);
            Container collectContainer(collectMsg);
            getConference().send(collectContainer);
            i = 0;
        }
        i++;
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
