#include <iostream>
#include <algorithm>
#include <array>

#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>
#include "opendavinci/odcore/data/TimeStamp.h"
#include "bsn/communication/TCPReceive.hpp"
#include "bsn/data/SensorData.h"
#include "bsn/data/ResourceUpdate.h"
#include "bsn/operation/Operation.hpp"

class TimeTriggeredSender : public odcore::base::module::TimeTriggeredConferenceClientModule {
    private:
        TimeTriggeredSender(const TimeTriggeredSender &/*obj*/);
        
        TimeTriggeredSender& operator=(const TimeTriggeredSender &/*obj*/);

    public:
        TimeTriggeredSender(const int32_t &argc, char **argv);

        virtual ~TimeTriggeredSender();

        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        virtual void setUp();

        virtual void tearDown();
};