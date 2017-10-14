#ifndef SENSORNODE_MODULE_H_
#define SENSORNODE_MODULE_H_

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

#include "openbasn/data/SensorData.h"
#include "openbasn/message/Request.h"
#include "openbasn/message/Acknowledge.h"
#include "openbasn/model/sensor/Sensor.h"

using namespace std;

using namespace odcore::base;
using namespace odcore::base::module;

using namespace openbasn::data;
using namespace openbasn::message;
using namespace openbasn::model::sensor;

class SensorModule : public TimeTriggeredConferenceClientModule {

    private:      
        SensorModule(const SensorModule &/*obj*/);
        SensorModule& operator=(const SensorModule &/*obj*/); 

    public:
        SensorModule(const int32_t &argc, char **argv);
        virtual ~SensorModule();
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        virtual void setUp();
        virtual void tearDown();

        void getSensorConfiguration();
        void sendSensorData(SensorData /*sensordata*/);
        string categorize(uint32_t /*type*/, double /*data*/);

    private:
        uint32_t m_id;
        FIFOQueue m_buffer;
        int32_t m_clock;
        Sensor m_sensor;
        bool m_emergency_state;
};

#endif