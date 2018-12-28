#include "ManagerModule.hpp"

ManagerModule::ManagerModule(const int32_t  &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ManagerModule")
    {}

ManagerModule::~ManagerModule() {}

void ManagerModule::setUp() {
}

void ManagerModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ManagerModule::body(){
   
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
