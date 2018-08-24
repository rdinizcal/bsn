#include <iostream>
#include <fstream>

#include "bsn/data/SensorData.h"
#include "opendavinci/odcore/data/TimeStamp.h"
#include "bsn/configuration/sensor_configuration.hpp"
#include "opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h"

class DataTriggeredReceiver : public odcore::base::module::DataTriggeredConferenceClientModule {
    private:
        DataTriggeredReceiver(const DataTriggeredReceiver &/*obj*/);
        DataTriggeredReceiver& operator=(const DataTriggeredReceiver &/*obj*/);

    public:    
        DataTriggeredReceiver(const int32_t &argc, char **argv);

        virtual ~DataTriggeredReceiver();

        virtual void nextContainer(odcore::data::Container &c);

    private:
        virtual void setUp();

        virtual void tearDown();
};