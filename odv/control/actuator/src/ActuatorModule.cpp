#include "ActuatorModule.hpp"

using namespace odcore::data;

using namespace bsn::communication;

ActuatorModule::ActuatorModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ActuatorModule"),
    buffer(){}

ActuatorModule::~ActuatorModule() {}

TCPSend sender(8000);

void ActuatorModule::setUp() {
    sender.set_port(getIdentifier());
    sender.setIP("localhost");
    sender.connect();
}

void ActuatorModule::tearDown(){
    sender.disconnect();    
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ActuatorModule::body(){

    std::string package;
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) { 
        /*
         * Receive actions to send to the managed system
         */
        while(!buffer.isEmpty()){
            Container container = buffer.leave();

            
            package = "teste1/teste2";
            sender.send(package);
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
