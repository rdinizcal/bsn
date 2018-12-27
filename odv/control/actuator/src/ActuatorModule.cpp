#include "ActuatorModule.hpp"

ActuatorModule::ActuatorModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ActuatorModule"){}

ActuatorModule::~ActuatorModule() {}

void ActuatorModule::setUp() {
}

void ActuatorModule::tearDown(){    
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ActuatorModule::body(){

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) { 
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
