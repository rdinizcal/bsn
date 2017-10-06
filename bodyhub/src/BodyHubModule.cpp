#include "BodyHubModule.h"

#include "openbasn/model/sensor/Sensor.h"
#include "openbasn/message/Acknowledge.h"

#include <iostream>

#include <map>
#include <string>
#include <iterator>
#include <algorithm>

using namespace std;

using namespace odcore::base;
using namespace odcore::data;
using namespace odcore::base::module;

using namespace openbasn::data;
using namespace openbasn::message;
using namespace openbasn::model::sensor;

BodyHubModule::BodyHubModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bodyhub"),
    m_id(getIdentifier()),
    m_clock(0),
    m_buffer(),
    m_sensornode(),
    m_datalog()
    {}

BodyHubModule::~BodyHubModule() {}

void BodyHubModule::setUp() {
    //setup buffer
    addDataStoreFor(m_buffer);

    //setup m_datalog
    m_datalog.open("bodyhub"+to_string(m_id)+".csv");
    
    //"SensorNodeID, Number of active sensors, SensorData, SensorData, ..., Health Risk, Sent at, Received at, Processed at";
    m_datalog << "SensorNodeID,Active Sensors,";
    for(uint32_t i = 0; i < getKeyValueConfiguration().getValue<uint32_t>("global.numberOfSensors"); i++) {
        uint32_t sensor_id = i+1;
        m_datalog << getKeyValueConfiguration().getValue<string>("global.sensortype."+ to_string(sensor_id)) << ",";
    }
    m_datalog << "Health Risk,Sent at,Received at,Processed at\n";
}

void BodyHubModule::tearDown() {
    m_datalog.close();
}

void BodyHubModule::processRequest(Request request){
    switch(request.getType()) {
        case Request::REGISTER : 
            {
                m_sensornode.insert(pair<uint32_t, string>(request.getSourceID(), "low"));
                Acknowledge ack(Acknowledge::OK, m_id, request.getSourceID()); 
                Container c_ack(ack);
                getConference().send(c_ack);
                CLOG1 << "SensorNode" << request.getSourceID() << " successfully registered." << endl;
            }
            break;
        case Request::UNREGISTER : 
            m_sensornode.erase(request.getSourceID()); 
            CLOG1 << "SensorNode" << request.getSourceID() << " successfully unregistered." << endl;
            break;
        case Request::SENSOR_DATA : break;
        default : cerr << "Could not recognize request"; break;
    }
}

void BodyHubModule::processSensorNodeData(SensorNodeData sensornodedata, TimeStamp sentTS,TimeStamp receivedTS){
    double health_risk = 0;
    map<int32_t,double> sensor_data_map = sensornodedata.getSensorDataMap();

    //Evaluate Data
    map<int32_t,double>::iterator it = sensor_data_map.begin();
    for(pair<int32_t,double> element : sensor_data_map){
        double sensor_risk = BodyHubModule::evaluateSensorDataRisk(element.first, element.second);

        health_risk += (sensor_risk > 0)?sensor_risk:0;
    }

    //Categorize Data
    if(0 < health_risk && health_risk < 1) {
        m_sensornode[sensornodedata.getSensorNodeID()] = "low";
    } else if (1 <= health_risk && health_risk < 5) {
        m_sensornode[sensornodedata.getSensorNodeID()] = "moderate";
    } else if (5 <= health_risk && health_risk < 20) {
        m_sensornode[sensornodedata.getSensorNodeID()] = "high";
    } else {
        m_sensornode[sensornodedata.getSensorNodeID()] = "unknown";
    }

    //Persist Data
    //"SensorNodeID, Number of active sensors, SensorData, SensorData, ..., Health Risk, Sent at, Received at, Processed at";
    m_datalog << sensornodedata.getSensorNodeID() << ",";
    m_datalog << sensor_data_map.size() << ",";

    for(uint32_t i = 0; i < getKeyValueConfiguration().getValue<uint32_t>("global.numberOfSensors"); i++){
        uint32_t sensor_id = i+1;

        if(sensor_data_map.find(sensor_id) != sensor_data_map.end()){
            m_datalog << sensor_data_map.find(sensor_id)->second<<",";
        } else {
            m_datalog << "-,";
        }
    }

    m_datalog << m_sensornode[sensornodedata.getSensorNodeID()] << ",";
    m_datalog << sentTS.getYYYYMMDD_HHMMSS() << ",";
    m_datalog << receivedTS.getYYYYMMDD_HHMMSS() << ",";
    m_datalog << TimeStamp().getYYYYMMDD_HHMMSS() << "\n";

    //View Data
    CLOG1 << "-------------------------------------------------" << endl;
    CLOG1 << " RECEIVED CONTENT " << endl;
    CLOG1 << " " << sensornodedata.toString() << " risk: " << m_sensornode[sensornodedata.getSensorNodeID()] << endl;
    CLOG1 << " sent at " << sentTS.getYYYYMMDD_HHMMSS() << endl;
    CLOG1 << " received at " << receivedTS.getYYYYMMDD_HHMMSS() << endl;
    CLOG1 << " processed at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
    CLOG1 << " clock tick: " << m_clock << endl;
    CLOG1 << "-------------------------------------------------" << endl;
}

double BodyHubModule::evaluateSensorDataRisk(uint32_t type, double data) {
    
    switch(type){

        case Sensor::THERMOMETER:
            if(37 >= data && data > 35){
                return 0.1; 
            } else if ( (35 >= data && data > 30) || (38 >= data && data > 37) ) {
                return 1;
            } else if ( (50 >= data && data > 38) || (30 >= data && data > 0) ) {
                return 5;
            } else {
                return 0; 
            }
            break;

        case Sensor::ECG:
            if ( (150 >= data && data > 120) || (80 >= data && data > 0) ) {
                return 5;
            } else if (120 >= data && data > 80) {
                return 0.1;
            } else {
                return 0;
            }
            break;

        case Sensor::OXIMETER:
            if( 100 >= data && data > 94) {
                return 0.1;
            } else if ( 94 >= data && data > 90) {
                return 1;
            } else if ( 90 >= data && data > 0) {
                return 5;
            } else {
                return 0;
            }
            break;

        default:
            return -1;
            break;
    }
}

void BodyHubModule::requestSensorNodeData(string health_risk_label) {
    map<uint32_t, string>::iterator it = m_sensornode.begin();
    for (pair<uint32_t, string> element : m_sensornode) {
        if( element.second.compare(health_risk_label) == 0) {
            Request request(Request::SENSOR_DATA, m_id, element.first);
            Container c_req(request);
            getConference().send(c_req);
        }
    }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        Container c = m_buffer.leave();

        if(c.getDataType() == Request::ID()) {
            BodyHubModule::processRequest(c.getData<Request>());
        } else if (c.getDataType() == SensorNodeData::ID()) {
            BodyHubModule::processSensorNodeData(c.getData<SensorNodeData>(),c.getSentTimeStamp(),c.getReceivedTimeStamp());
        } else {
            m_clock++;

            if(m_clock%15 == 0) {
                BodyHubModule::requestSensorNodeData("low");
            } 

            if(m_clock%5 == 0) { 
                BodyHubModule::requestSensorNodeData("moderate");
            }

            BodyHubModule::requestSensorNodeData("high");
            BodyHubModule::requestSensorNodeData("unknown");

                
        }
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}