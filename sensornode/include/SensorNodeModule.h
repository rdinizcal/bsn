#ifndef SENSORNODE_MODULE_H_
#define SENSORNODE_MODULE_H_

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

#include "openbasn/data/SensorData.h"
#include "openbasn/model/sensor/Sensor.h"

#include <deque>
#include <fstream>
#include <time.h>
#include <iostream>
#include <chrono>
#include <string>

using namespace std;

using namespace odcore::base;
using namespace odcore::base::module;

using namespace openbasn::data;
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

        void getSensorConfiguration();
        string generateData(string /*actual status*/);
        string statusAnalysis(string /*categorized data*/, string /*actual status*/);
        void sendSensorData(SensorData /*sensordata*/);

    private:
        uint32_t m_id;
        int32_t m_sensor_type;
        string m_status;
        deque<string> m_data_queue;

        ofstream m_status_log;
        ofstream m_sensornode_log;

        int32_t n;
};

#endif