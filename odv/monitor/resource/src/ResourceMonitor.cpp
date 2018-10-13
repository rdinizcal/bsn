#include "../include/ResourceMonitor.hpp"

ResourceMonitor::ResourceMonitor(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ResourceMonitor")
    {}

ResourceMonitor::~ResourceMonitor() {}

void ResourceMonitor::setUp() {
    //addDataStoreFor(,);
}

void ResourceMonitor::tearDown(){}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ResourceMonitor::body(){


    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
