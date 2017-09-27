#ifndef SENSORNODE_MODULE_H_
#define SENSORNODE_MODULE_H_

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "openbasn/model/sensor/Sensor.h"

using namespace std;

using namespace odcore::base::module;

using namespace openbasn::model::sensor;

class SensorNodeModule : public TimeTriggeredConferenceClientModule {

    private:      
        SensorNodeModule(const SensorNodeModule &/*obj*/);
        SensorNodeModule& operator=(const SensorNodeModule &/*obj*/); 

    public:
        SensorNodeModule(const int32_t &argc, char **argv);
        virtual ~SensorNodeModule();
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        virtual void setUp();
        virtual void tearDown();

    private:
        uint32_t m_id;
        vector<Sensor> m_sensor_vector;
        uint32_t m_number_of_sensors;
};

#endif