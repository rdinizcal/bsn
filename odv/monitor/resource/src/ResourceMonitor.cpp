#include "../include/ResourceMonitor.hpp"

using namespace odcore::data; 
using namespace bsn::data;

ResourceMonitor::ResourceMonitor(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ResourceMonitor"),
    data_buffer(),
    mResource() {}

ResourceMonitor::~ResourceMonitor() {}

void ResourceMonitor::setUp() {
    addDataStoreFor(874,data_buffer);

    const std::string rName  = getKeyValueConfiguration().getValue<std::string>("resourcemonitor.name");
    const double rCapacity = getKeyValueConfiguration().getValue<double>("resourcemonitor.capacity");
    const double rInitialLevel = getKeyValueConfiguration().getValue<double>("resourcemonitor.initialLevel");
    const double rUnit = getKeyValueConfiguration().getValue<double>("resourcemonitor.unit");

    mResource.setName(rName);
    mResource.setCapacity(rCapacity);
    mResource.setCurrentLevel(rInitialLevel);
    mResource.setUnit(rUnit);
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

            std::cout << "Current " << mResource.getName() << " level: " << mResource.getCurrentLevel() << endl;

            ResourceInfo rInfo(mResource);
            Container rInfoContainer(rInfo);
            getConference().send(rInfoContainer);
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
