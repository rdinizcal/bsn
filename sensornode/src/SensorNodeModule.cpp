#include "SensorNodeModule.h"

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/generated/odcore/data/dmcp/PulseAckMessage.h"

#include "openbasn/data/SensorNodeData.h"
#include "openbasn/message/Request.h"
#include "openbasn/message/Acknowledge.h"

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
    m_isRegistered(false),
    m_clock_tick(0),
    m_buffer(),
    m_sensor_vector(),
    m_number_of_sensors(),
    m_risk("low")
    {}

SensorNodeModule::~SensorNodeModule() {}

void SensorNodeModule::setUp() {
    //setup module buffer
    addDataStoreFor(871, m_buffer);
    addDataStoreFor(872, m_buffer);

    SensorNodeModule::getSensorConfiguration();
}

void SensorNodeModule::tearDown() {
    //Send UNREGISTER request
    Request req(Request::UNREGISTER, m_id);
    Container c_req(req);
    getConference().send(c_req);
}

void SensorNodeModule::getSensorConfiguration(){
    m_number_of_sensors = getKeyValueConfiguration().getValue<uint32_t>("global.numberOfSensors");
    
    for(uint32_t i = 0; i < m_number_of_sensors; i++) {
        string sensor_type;
        uint32_t sensor_id = i+1;

        sensor_type = getKeyValueConfiguration().getValue<string>("global.sensortype."+ to_string(sensor_id));

        if((getKeyValueConfiguration().getValue<bool>("sensornode."+ sensor_type +".active") == 1)){
            bool   active = (getKeyValueConfiguration().getValue<bool>("sensornode."+ sensor_type +".active") == 1);
            float  samplerate = getKeyValueConfiguration().getValue<float>("sensornode."+ sensor_type +".samplerate");
            string mean_behavior = getKeyValueConfiguration().getValue<string>("sensornode."+ sensor_type +".mean.behavior");
            double stddev = getKeyValueConfiguration().getValue<double>("sensornode."+ sensor_type +".stddev");
            double mean = getKeyValueConfiguration().getValue<double>("sensornode."+ sensor_type +".mean."+mean_behavior);

            m_sensor_vector.push_back(Sensor(sensor_id, samplerate, active, mean, stddev));
        }
    }
}

void SensorNodeModule::sendRequest(Request request){
    Container c(request);
    getConference().send(c);
    CLOG1 << req.toString() << " sent" << endl;
}

void SensorNodeModule::processAcknowledge(Acknowledge acknowledge){
    if(acknowledge.getDestinationID() == m_id && acknowledge.getType() == Acknowledge::OK){
        m_isRegistered = true;
        m_clock_tick = 0;
        CLOG1 << "SensorNode" << m_id << " successfully registered." << endl;
    }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorNodeModule::body() {

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
         m_clock_tick++;
        
        if(!m_isRegistered){

            if(m_clock_tick == 15){
                return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
            } else if (m_clock_tick == 1) {
                Request request(Request::REGISTER, m_id);
                SensorNodeModule::sendRequest(request)
            } else {
                while(!m_buffer.isEmpty()){
                    Container c = m_buffer.leave();
    
                    if(c.getDataType() == Acknowledge::ID()){
                        SensorNodeModule::processAcknowledge(c.getData<Acknowledge>());
                    }
                }
            }

        } else {

            while(!m_buffer.isEmpty()){
                Container c = m_buffer.leave();

                if(c.getDataType() == Request::ID()){
                    Request req = c.getData<Request>();
                
                    if(req.getDestinationID() == m_id){ 

                        if(req.getType() == Request::SENSOR_DATA){
                            m_risk = req.getContent();
                        } 

                    }
                }

            }

            if( (m_risk.compare("low") == 0 && m_clock_tick == 15) 
                || (m_risk.compare("moderate") == 0 && m_clock_tick == 5) 
                || (m_risk.compare("high") == 0 || m_risk.compare("unknown") == 0) ){
                SensorNodeData snData(m_id);
    
                for(uint32_t i = 0; i < m_sensor_vector.size(); i++) {
                    snData.addSensorData(m_sensor_vector[i].getSensorType(), m_sensor_vector[i].getData());
                }
            
                Container c_send(snData);
                getConference().send(c_send);
                cout << snData.toString() << " sent at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
    
                m_clock_tick = 0;
            } 
            
        } 

        PulseAckMessage pulseackmessage;
        Container ccc(pulseackmessage);
        getConference().send(ccc);
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}