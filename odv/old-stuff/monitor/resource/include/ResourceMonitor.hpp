#ifndef RESOURCE_MONITOR_HPP
#define RESOURCE_MONITOR_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include <iostream>

#include "bsn/msg/data/ResourceUpdate.h"
#include "bsn/msg/info/ResourceInfo.h"
#include "bsn/resource/Resource.hpp"

class ResourceMonitor : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
    private:
        ResourceMonitor(const ResourceMonitor & /*obj*/);
        ResourceMonitor &operator=(const ResourceMonitor & /*obj*/);
        virtual void setUp();
        virtual void tearDown();

    public:
        ResourceMonitor(const int32_t &argc, char **argv);
        virtual ~ResourceMonitor();

        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        odcore::base::FIFOQueue buffer;
        bsn::resource::Resource mResource;
};

#endif 