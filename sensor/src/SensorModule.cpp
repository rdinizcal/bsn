#include "SensorModule.h"

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/generated/odcore/data/dmcp/PulseAckMessage.h"

#include <iostream>

using namespace std;


using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;
using namespace odcore::data::dmcp;

using namespace openbasn::data;
using namespace openbasn::message;
using namespace openbasn::model::sensor;


SensorModule::SensorModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "sensor"),
    m_id(getIdentifier()),
    m_isRegistered(false),
    m_clock_tick(0),
    m_buffer(),
    m_sensor(),
    m_risk("low")
    {}

SensorModule::~SensorModule() {}

void SensorModule::setUp() {
    //setup module buffer
    addDataStoreFor(871, m_buffer);
    addDataStoreFor(872, m_buffer);

    SensorModule::getSensorConfiguration();
}

void SensorModule::tearDown() {
    //Send UNREGISTER request
    Request req(Request::UNREGISTER, m_id);
    Container c_req(req);
    getConference().send(c_req);
}

void SensorModule::getSensorConfiguration(){
    int32_t sensortypes = getKeyValueConfiguration().getValue<int32_t>("global.sensortypes");
    
    for(int32_t i = 0; i < sensortypes; i++) {
        string sensor_type;
        int32_t sensor_id = i+1;

        sensor_type = getKeyValueConfiguration().getValue<string>("global.sensortype."+ to_string(sensor_id));

        if((getKeyValueConfiguration().getValue<bool>("sensor."+ sensor_type +".active") == 1)){
            bool   active = (getKeyValueConfiguration().getValue<bool>("sensor."+ sensor_type +".active") == 1);
            float  samplerate = getKeyValueConfiguration().getValue<float>("sensor."+ sensor_type +".samplerate");
            string mean_behavior = getKeyValueConfiguration().getValue<string>("sensor."+ sensor_type +".mean.behavior");
            double stddev = getKeyValueConfiguration().getValue<double>("sensor."+ sensor_type +".stddev");
            double mean = getKeyValueConfiguration().getValue<double>("sensor."+ sensor_type +".mean."+mean_behavior);

            m_sensor = Sensor(sensor_id, samplerate, active, mean, stddev);
        }
    }
}

void SensorModule::sendSensorData(SensorData sensordata){
    Container container(sensordata);
    getConference().send(container);
    CLOG1 << sensordata.toString() << " sent at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
}

void SensorModule::sendRequest(Request request){
    Container container(request);
    getConference().send(container);
    CLOG1 << request.toString() << " sent at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
}

void SensorModule::processRequest(Request request){
    if(request.getDestinationID() == m_id){ 
        if(request.getType() == Request::SENSOR_DATA){
            m_risk = request.getContent();
        } 
    }
}

void SensorModule::processAcknowledge(Acknowledge acknowledge){
    if(acknowledge.getDestinationID() == m_id && acknowledge.getType() == Acknowledge::OK){
        m_isRegistered = true;
        m_clock_tick = 0;
        CLOG1 << "Sensor" << m_id << " successfully registered." << endl;
    }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorModule::body() {

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
         m_clock_tick++;
        
        if(!m_isRegistered){

            if(m_clock_tick == 15){
                return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
            } else if (m_clock_tick == 1) {
                Request request(Request::REGISTER, m_id);
                SensorModule::sendRequest(request);
            } else {
                while(!m_buffer.isEmpty()){
                    Container c = m_buffer.leave();
    
                    if(c.getDataType() == Acknowledge::ID()){
                        SensorModule::processAcknowledge(c.getData<Acknowledge>());
                    }
                }
            }

        } else {

            while(!m_buffer.isEmpty()){
                Container c = m_buffer.leave();

                if(c.getDataType() == Request::ID()){
                    SensorModule::processRequest(c.getData<Request>());
                }

            }

            if( (m_risk.compare("low") == 0 && m_clock_tick == 15) 
                || (m_risk.compare("moderate") == 0 && m_clock_tick == 5) 
                || (m_risk.compare("high") == 0 || m_risk.compare("unknown") == 0) ){
                
                SensorModule::sendSensorData(SensorData(m_id, m_sensor.getType(), m_sensor.getData()));
                m_clock_tick = 0;
            } 
            
        } 

        PulseAckMessage pulseackmessage;
        Container ccc(pulseackmessage);
        getConference().send(ccc);
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}