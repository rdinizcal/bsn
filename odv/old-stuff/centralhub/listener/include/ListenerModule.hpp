#include <iostream>
#include <algorithm>
#include <array>

#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>
#include "opendavinci/odcore/data/TimeStamp.h"
#include "bsn/communication/TCPReceive.hpp"
#include "bsn/msg/data/SensorData.h"
#include "bsn/msg/data/ResourceUpdate.h"
#include "bsn/operation/Operation.hpp"

class ListenerModule : public odcore::base::module::TimeTriggeredConferenceClientModule {
    private:
        ListenerModule(const ListenerModule &/*obj*/);
        
        ListenerModule& operator=(const ListenerModule &/*obj*/);

    public:
        ListenerModule(const int32_t &argc, char **argv);

        virtual ~ListenerModule();

        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        virtual void setUp();

        virtual void tearDown();
};