#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>

#include <iostream>

#include "opendavinci/odcore/data/TimeStamp.h"

#include "../../../communication/include/TCPReceive.hpp"

#include "bsn/data/SensorData.h"

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