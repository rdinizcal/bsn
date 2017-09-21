#include <iostream>

#include "BodyHubModule.h"

using namespace std;

BodyHubModule::BodyHubModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "BodyHubModule")
     {}

BodyHubModule::~BodyHubModule() {}

void BodyHubModule::setUp() {}

void BodyHubModule::tearDown() {}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    //ReceiveData
    
    //PersistData

    //ViewData
    }
    
return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}