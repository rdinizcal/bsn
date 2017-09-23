#include "SensorNodeModule.h"

#include "openbasn/data/SensorData.h"
#include "openbasn/model/sensor/Sensor.h"

#include <iostream>

using namespace std;

using namespace odcore;
using namespace odcore::data;
using namespace odcore::base::module;

using namespace openbasn::data;
using namespace openbasn::model::sensor;


SensorNodeModule::SensorNodeModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "sensornode"),
    m_id(getIdentifier())
     {}

SensorNodeModule::~SensorNodeModule() {}

void SensorNodeModule::setUp() {}

void SensorNodeModule::tearDown() {}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorNodeModule::body() {
    
    //Configure thermometer
    bool t_active = (getKeyValueConfiguration().getValue<bool>("sensornode.thermometer.active") == 1);
    float t_samplerate = getKeyValueConfiguration().getValue<float>("sensornode.thermometer.samplerate");
    string t_mean_behavior = getKeyValueConfiguration().getValue<string>("sensornode.thermometer.mean.behavior");
    double t_stddev = getKeyValueConfiguration().getValue<double>("sensornode.thermometer.stddev");
    double t_mean = getKeyValueConfiguration().getValue<double>("sensornode.thermometer.mean."+t_mean_behavior);

    //Configure ecg
    bool e_active = (getKeyValueConfiguration().getValue<bool>("sensornode.ecg.active") == 1);
    float e_samplerate = getKeyValueConfiguration().getValue<float>("sensornode.ecg.samplerate");
    string e_mean_behavior = getKeyValueConfiguration().getValue<string>("sensornode.ecg.mean.behavior");
    double e_stddev = getKeyValueConfiguration().getValue<double>("sensornode.ecg.stddev");
    double e_mean = getKeyValueConfiguration().getValue<double>("sensornode.ecg.mean."+e_mean_behavior);

    //Configure oximeter
    bool o_active = (getKeyValueConfiguration().getValue<bool>("sensornode.oximeter.active") == 1);
    float o_samplerate = getKeyValueConfiguration().getValue<float>("sensornode.oximeter.samplerate");
    string o_mean_behavior = getKeyValueConfiguration().getValue<string>("sensornode.oximeter.mean.behavior");
    double o_stddev = getKeyValueConfiguration().getValue<double>("sensornode.oximeter.stddev");
    double o_mean = getKeyValueConfiguration().getValue<double>("sensornode.oximeter.mean."+o_mean_behavior);

    //Instantiate Sensors
    Sensor thermometer(Sensor::THERMOMETER, t_samplerate, t_active, t_mean, t_stddev);
    Sensor ecg(Sensor::ECG, e_samplerate, e_active, e_mean, e_stddev);
    Sensor oximeter(Sensor::OXIMETER, o_samplerate, o_active,o_mean , o_stddev);

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        if(thermometer.isActive()){
            SensorData sd(m_id, thermometer.getSensorType(), thermometer.getData());
            Container c(sd);
            getConference().send(c);
            cout << sd.toString() << " sent." << endl;
        }

        if(ecg.isActive()) {
            SensorData sd(m_id, ecg.getSensorType(), ecg.getData());
            Container c(sd);
            getConference().send(c);
            cout << sd.toString() << " sent." << endl;
        }

        if(oximeter.isActive()){
            SensorData sd(m_id, oximeter.getSensorType(), oximeter.getData());
            Container c(sd);
            getConference().send(c);
            cout << sd.toString() << " sent." << endl;
        }
    }
    
return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}