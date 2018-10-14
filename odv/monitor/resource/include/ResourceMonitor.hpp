#ifndef RESOURCE_MONITOR_HPP
#define RESOURCE_MONITOR_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/data/ResourceUpdate.h"
#include "bsn/data/ResourceInfo.h"
#include "bsn/resource/Resource.hpp"

class ResourceMonitor : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
    private:
        ResourceMonitor(const ResourceMonitor & /*obj*/);
        ResourceMonitor &operator=(const ResourceMonitor & /*obj*/);
        virtual void setUp();
        virtual void tearDown();

    public:
        /**
         * Construtor e destrutor da classe.
         */
        ResourceMonitor(const int32_t &argc, char **argv);
        virtual ~ResourceMonitor();

        /**
         * Método efetivamente executado.
         */
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        bsn::resource::Resource mResource;
        odcore::base::FIFOQueue data_buffer;
};

#endif 