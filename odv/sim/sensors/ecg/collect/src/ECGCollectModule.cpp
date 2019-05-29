#include "ECGCollectModule.hpp"
#define ECGCOLLECTMODULE_MSG_QUE 4011

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
