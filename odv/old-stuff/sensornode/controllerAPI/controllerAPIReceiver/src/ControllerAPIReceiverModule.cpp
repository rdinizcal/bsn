#include "ControllerAPIReceiverModule.hpp"

using namespace odcore::data;

using namespace bsn::communication;

ControllerAPIReceiverModule::ControllerAPIReceiverModule(const int32_t  &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ControllerAPIReceiverModule"),
    buffer() {}

ControllerAPIReceiverModule::~ControllerAPIReceiverModule() {}

TCPReceive receiver(8000);

void ControllerAPIReceiverModule::setUp() {
    receiver.set_port(getIdentifier());
    receiver.start_connection();   
}

void ControllerAPIReceiverModule::tearDown(){
    receiver.stop_connection();
}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ControllerAPIReceiverModule::body(){
   
    std::string package;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        /* 
         * Receive commands from the control unit 
         * and spread it to the respective modules
         */
        package = receiver.get_package();
        
        if (package != "") {

            /*
             * Split the package 
             * Map actions into control command messages
             * Spread'em through the node 
             */ 
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
