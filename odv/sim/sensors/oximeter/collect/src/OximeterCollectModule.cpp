#include "OximeterCollectModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;

using namespace bsn::msg::taskMsg;

OximeterCollectModule::OximeterCollectModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "oximeter"),
    buffer(),
    type("oximeter"),
    active(true),
    params({{"freq",0.90},{"m_avg",5}}),
    markov()
    {}

OximeterCollectModule::~OximeterCollectModule() {}

void OximeterCollectModule::setUp() {
    //srand(time(NULL));
    addDataStoreFor(OXIMETERCOLLECTMODULE_MSG_QUE, buffer);
    
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

   
}

void OximeterCollectModule::tearDown() {

}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode OximeterCollectModule::body(){

    double data;
    double risk;
    int i = 0;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        i = 0;
        // Apenas executa uma vez a cada segundo
        while(i > 10){ // Receive control command and module update
            
            
            // TASK: Collect ecg data
            data = markov.calculate_state();
            markov.next_state();              


            // Send data from Collect task to Filter task
            OximeterCollectTaskMessage collectMsg(data);
            Container collectContainer(collectMsg);
            getConference().send(collectContainer);
            i = 0;
        }   

        i++;
            
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}