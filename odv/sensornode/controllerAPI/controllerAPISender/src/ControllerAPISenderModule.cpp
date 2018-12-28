#include "ControllerAPISenderModule.hpp"

using namespace odcore::data;

using namespace bsn::communication;

ControllerAPISenderModule::ControllerAPISenderModule(const int32_t  &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ControllerAPISenderModule"),
    buffer() {}

ControllerAPISenderModule::~ControllerAPISenderModule() {}

TCPSend sender(8000);

void ControllerAPISenderModule::setUp() {
    sender.set_port(getIdentifier());
    sender.setIP("localhost");
    sender.connect();
}

void ControllerAPISenderModule::tearDown(){
    sender.disconnect();
}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ControllerAPISenderModule::body(){
   
    std::string package;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        /*
         * Receive contextual information from the 
         * modules and send it to the control unit
         * through TCPConnection
         */
        while(!buffer.isEmpty()){
            Container container = buffer.leave();

            /*
             * Contextual information that will be forwarded here:
             *   - Current sensor data risk                
             *   - Current battery level   
             *   - Sensors' availability
             *   - Sensors' reliability (unreliable data and communication failure index) 
             */
            
            package = "teste1/teste2";
            sender.send(package);
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
