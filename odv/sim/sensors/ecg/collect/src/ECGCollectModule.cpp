#include "ECGCollectModule.hpp"
#include "bsn/libbsn/include/msg/MessageQueueCodes.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;
using namespace bsn::msg::info;
using namespace bsn::msg::control;

ECGCollectModule::ECGCollectModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ecg"),
    buffer(),
    type("ecg"),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    filter(5),
    {}

ECGCollectModule::~ECGCollectModule() {}

void ECGCollectModule::setUp() {
    //srand(time(NULL));
    addDataStoreFor(ECGCOLLECTMODULE_MSG_QUE, buffer);

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
  
    // Configure markov chain
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

void ECGCollectModule::tearDown() {

}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ECGCollectModule::body() {
  
    double data;
    double risk;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
       
        while(!buffer.isEmpty()){ // Receive control command and module update
            container = buffer.leave();

            active = container.getData<ECGControlCommand>().getActive();

        /*
         * Module execution
         */
            
            // TASK: Collect ecg data

            /*
            **
            ***
            //  Que valor dataS e DataD devem receber?
            // Que valor Risk deve receber?
            ***
            **
            */

           
            SensorData sdata(type, data, risk);
            Container sdataContainer(sdata);
            getConference().send(sdataContainer);

            
        }
            
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
