#include "SensorNodeModule.h"

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/generated/odcore/data/dmcp/PulseAckMessage.h"
#include "opendavinci/odcore/base/Thread.h"

#include <time.h>
#include <iostream>

using namespace std;


using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;
using namespace odcore::data::dmcp;

using namespace openbasn::data;
using namespace openbasn::message;
using namespace openbasn::model::sensor;


SensorNodeModule::SensorNodeModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "sensornode"),
    m_id(getIdentifier()),
    m_buffer(),
    m_clock(0),
    m_sensor(),
    m_emergency_state(false) {}

    SensorNodeModule::~SensorNodeModule() {}

void SensorNodeModule::setUp() {
    //setup module buffer
    addDataStoreFor(871, m_buffer);
    addDataStoreFor(872, m_buffer);

    SensorNodeModule::getSensorConfiguration();
}

void SensorNodeModule::tearDown() {}

void SensorNodeModule::getSensorConfiguration(){
    int32_t sensortypes = getKeyValueConfiguration().getValue<int32_t>("global.sensortypes");
    
    for(int32_t i = 0; i < sensortypes; i++) {
        string sensor_type;
        int32_t sensor_id = i+1;

        sensor_type = getKeyValueConfiguration().getValue<string>("global.sensortype."+ to_string(sensor_id));

        if((getKeyValueConfiguration().getValue<bool>("sensornode."+ sensor_type +".active") == 1)){
            bool   active = (getKeyValueConfiguration().getValue<bool>("sensornode."+ sensor_type +".active") == 1);
            float  samplerate = getKeyValueConfiguration().getValue<float>("sensornode."+ sensor_type +".samplerate");
            string mean_behavior = getKeyValueConfiguration().getValue<string>("sensornode."+ sensor_type +".mean.behavior");
            double stddev = getKeyValueConfiguration().getValue<double>("sensornode."+ sensor_type +".stddev");
            double mean = getKeyValueConfiguration().getValue<double>("sensornode."+ sensor_type +".mean."+mean_behavior);

            m_sensor = Sensor(sensor_id, samplerate, active, mean, stddev);
        }
    }
}

void SensorNodeModule::sendSensorData(SensorData sensordata){
    Container container(sensordata);
    getConference().send(container);
    CLOG1 << sensordata.toString() << " sent at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
}

string SensorNodeModule::categorize(uint32_t type, double data) {
    
    switch(type){

        case Sensor::THERMOMETER:
            if(37 >= data && data > 35){
                return "low"; 
            } else if ( (35 >= data && data > 30) || (38 >= data && data > 37) ) {
                return "moderate";
            } else if ( (50 >= data && data > 38) || (30 >= data && data > 0) ) {
                return "high";
            } else {
                return "unknown"; 
            }
            break;

        case Sensor::ECG:
            if ( (150 >= data && data > 120) || (80 >= data && data > 0) ) {
                return "high";
            } else if (120 >= data && data > 80) {
                return "low";
            } else {
                return "unknown";
            }
            break;

        case Sensor::OXIMETER:
            if( 100 >= data && data > 94) {
                return "low";
            } else if ( 94 >= data && data > 90) {
                return "moderate";
            } else if ( 90 >= data && data > 0) {
                return "high";
            } else {
                return "unknown";
            }
            break;

        default:
            return "unknown";
            break;
    }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorNodeModule::body() {

    srand(time(NULL));
    double tps = getKeyValueConfiguration().getValue<double>("sensornode.tps");
    TimeStamp previous;
    string sensor_risk="low";

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        Thread::usleepFor(rand() % tps);

        //atualiza relogio
        m_clock = ((TimeStamp()-previous).toMicroseconds())/1000000L;
        CLOG1<<"M_CLOCK: "<<m_clock<<endl;

        //amostra dados do sensor
        sensor_risk = SensorNodeModule::categorize(m_sensor.getType(),m_sensor.getData());

        //avalia
        if(((sensor_risk=="high" || sensor_risk=="unknown") && m_clock>1) 
            ||(sensor_risk=="moderate" && m_clock>5)
            ||(sensor_risk=="low" && m_clock>15)) {

                //envia dado
                SensorNodeModule::sendSensorData(SensorData(m_id, m_sensor.getType(), m_sensor.getData(), sensor_risk));
                previous = TimeStamp();
        }
        
        //envia pulse_ack
        PulseAckMessage pulseackmessage;
        Container c(pulseackmessage);
        getConference().send(c);
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}