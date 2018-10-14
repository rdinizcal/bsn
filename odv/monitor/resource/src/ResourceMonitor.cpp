#include "../include/ResourceMonitor.hpp"

using namespace odcore::data; 
using namespace bsn::data;

ResourceMonitor::ResourceMonitor(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ResourceMonitor"),
    data_buffer(),
    mResource() {}

ResourceMonitor::~ResourceMonitor() {}

void ResourceMonitor::setUp() {

    const std::string rName = "Battery";
    const double rCapacity = 100;
    const double rInitialLevel = 100;
    const double rUnit = 0.1;

    mResource.setName(rName);
    mResource.setCapacity(rCapacity);
    mResource.setCurrentLevel(rInitialLevel);
    mResource.setUnit(rUnit);

    addDataStoreFor(874,data_buffer);
}

void ResourceMonitor::tearDown(){}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ResourceMonitor::body(){

    Container container;
    int units = 0;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        while (!data_buffer.isEmpty()) {
            container = data_buffer.leave();
            units = container.getData<ResourceUpdate>().getUnits();
            
            if(units>=0){
                mResource.generate(units);
            } else {
                mResource.consume((-1)*units);
            }

            ResourceInfo rInfo(mResource);
            Container rInfoContainer(rInfo);
            getConference().send(rInfoContainer);
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
