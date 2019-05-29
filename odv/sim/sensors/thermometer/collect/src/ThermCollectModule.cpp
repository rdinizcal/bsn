#include "ThermCollectModule.hpp"
#define THERMCOLLECTMODULE_MSG_QUE 4041

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;

ThermCollectModule::ThermCollectModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "thermometer"),
    buffer(),
    type("thermometer"),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    filter(5),
    {}

ThermCollectModule::~ThermCollectModule() {}

void ThermCollectModule::setUp() {
    //srand(time(NULL));
    addDataStoreFor(THERMCOLLECTMODULE_MSG_QUE, buffer);
    
   
}

void ThermCollectModule::tearDown() {
   
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ThermCollectModule::body(){

    double data;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
           
        { // TASK: Collect thermometer data with data_accuracy
            
            CollectedData cData(type, data);
            Container container(cData);
            getConference().send(container);
        }
        
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
