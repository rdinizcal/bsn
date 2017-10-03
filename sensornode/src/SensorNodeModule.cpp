#include "SensorNodeModule.h"

#include "opendavinci/odcore/base/FIFOQueue.h"

#include "openbasn/data/SensorNodeData.h"
#include "openbasn/message/Request.h"


#include <iostream>

using namespace std;

using namespace odcore::base;
using namespace odcore::data;
using namespace odcore::base::module;

using namespace openbasn::data;
using namespace openbasn::message;
using namespace openbasn::model::sensor;


SensorNodeModule::SensorNodeModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "sensornode"),
    m_id(getIdentifier()),
    m_sensor_vector(),
    m_number_of_sensors()
    {}

SensorNodeModule::~SensorNodeModule() {}

void SensorNodeModule::setUp() {
    //Send REGISTER request
    Request req(Request::REGISTER, m_id);
    Container c_req(req);
    getConference().send(c_req);
}

void SensorNodeModule::tearDown() {
    //Send UNREGISTER request
    Request req(Request::UNREGISTER, m_id);
    Container c_req(req);
    getConference().send(c_req);
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorNodeModule::body() {
    
    m_number_of_sensors = getKeyValueConfiguration().getValue<uint32_t>("sensornode.numberOfSensors");
    
    for(uint32_t i = 0; i < m_number_of_sensors; i++) {
        string sensor_type;
        uint32_t sensor_id = i+1;

        sensor_type = getKeyValueConfiguration().getValue<string>("sensornode.sensortype."+ to_string(sensor_id));

        if((getKeyValueConfiguration().getValue<bool>("sensornode."+ sensor_type +".active") == 1)){
            bool   active = (getKeyValueConfiguration().getValue<bool>("sensornode."+ sensor_type +".active") == 1);
            float  samplerate = getKeyValueConfiguration().getValue<float>("sensornode."+ sensor_type +".samplerate");
            string mean_behavior = getKeyValueConfiguration().getValue<string>("sensornode."+ sensor_type +".mean.behavior");
            double stddev = getKeyValueConfiguration().getValue<double>("sensornode."+ sensor_type +".stddev");
            double mean = getKeyValueConfiguration().getValue<double>("sensornode."+ sensor_type +".mean."+mean_behavior);

            m_sensor_vector.push_back(Sensor(sensor_id, samplerate, active, mean, stddev));
        }
    }

    FIFOQueue fifo;
    addDataStoreFor(3, fifo);
    addDataStoreFor(8, fifo);

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        Container c_rec = fifo.leave();
        
        if(c_rec.getDataType() == Request::ID()){
            Request req = c_rec.getData<Request>();

            if(req.getRequestType() == Request::SENSOR_DATA && req.getDestinationID() == m_id){
    
                SensorNodeData snData(m_id);

                for(uint32_t i = 0; i < m_sensor_vector.size(); i++) {
                    snData.addSensorData(m_sensor_vector[i].getSensorType(), m_sensor_vector[i].getData());
                }
    
                Container c_send(snData);
                getConference().send(c_send);
                cout << snData.toString() << " sent at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
            }
        }
    }
    
return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}