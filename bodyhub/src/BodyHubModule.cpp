#include "BodyHubModule.h"

#include "openbasn/model/sensor/Sensor.h"
#include "openbasn/message/Acknowledge.h"

#include "opendavinci/generated/odcore/data/dmcp/PulseAckMessage.h"

#include <iostream>

#include <map>
#include <string>
#include <iterator>
#include <algorithm>

using namespace std;

using namespace odcore::base;
using namespace odcore::data;
using namespace odcore::base::module;
using namespace odcore::data::dmcp;

using namespace openbasn::data;
using namespace openbasn::message;
using namespace openbasn::model::sensor;

BodyHubModule::BodyHubModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bodyhub"),
    m_id(getIdentifier()),
    m_buffer(),
    m_health_risk("low"),
    m_healthrisk_counter(0),
    m_sensornode(),
    m_datalog()
    {}

BodyHubModule::~BodyHubModule() {}

void BodyHubModule::setUp() {
    //setup buffer
    addDataStoreFor(871, m_buffer);
    addDataStoreFor(872, m_buffer);
    addDataStoreFor(873, m_buffer);

    //setup m_datalog
    m_datalog.open("bodyhub"+to_string(m_id)+".csv");
    
    //"SensorNode, data, data, ..., Sensor risk category, Health risk, Sent at, Received at, Processed at"
    m_datalog << "SensorNode,";
    for(uint32_t i = 0; i < getKeyValueConfiguration().getValue<uint32_t>("global.sensortypes"); i++) {
        uint32_t sensor_id = i+1;
        m_datalog << getKeyValueConfiguration().getValue<string>("global.sensortype."+ to_string(sensor_id)) << ",";
    }
    m_datalog << "Sensor risk category,Health risk,Sent at,Received at,Processed at\n";
}

void BodyHubModule::tearDown() {
    m_datalog.close();

    map<uint32_t,string>::iterator it = m_sensornode.begin();
    for(pair<uint32_t,string> element : m_sensornode){

        Request req(Request::UNREGISTER, m_id, element.first);
        Container c_req(req);
        getConference().send(c_req);

        m_sensornode.erase(element.first); 
        CLOG1 << "SensorNode" << element.first << " successfully unregistered." << endl;
    }
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
        default: break;
    }
}

void BodyHubModule::processSensorNodeData(SensorNodeData sensornodedata, TimeStamp sentTS,TimeStamp receivedTS){
    double sensor_risk;
    string sensor_risk_category;

    //Evaluate Data
    sensor_risk = BodyHubModule::evaluateSensorDataRisk(sensornodedata.getSensorType(), sensornodedata.getSensorData());

    //Categorize Data
    if(0 < sensor_risk && sensor_risk < 1) {
        sensor_risk_category = "low";
    } else if (1 <= sensor_risk && sensor_risk < 5) {
        sensor_risk_category = "moderate";
    } else if (5 <= sensor_risk && sensor_risk < 20) {
        sensor_risk_category = "high";
    } else {
        sensor_risk_category = "unknown";
    }
    
    //Persist Data
    //"SensorNode, data, data, ..., Sensor risk category, Health risk, Sent at, Received at, Processed at"
    m_datalog << sensornodedata.getSensorNodeID() << ",";
    for(int32_t i = 0; i < getKeyValueConfiguration().getValue<int32_t>("global.sensortypes"); i++){
        int32_t sensor_id = i+1;
        if(sensornodedata.getSensorType() == sensor_id){
            m_datalog << sensornodedata.getSensorData() << ",";
        } else {
            m_datalog << "-,";
        }
    }
    m_datalog << sensor_risk_category << ",";
    m_datalog << m_health_risk << ",";
    m_datalog << sentTS.getYYYYMMDD_HHMMSS() << ",";
    m_datalog << receivedTS.getYYYYMMDD_HHMMSS() << ",";
    m_datalog << TimeStamp().getYYYYMMDD_HHMMSS() << "\n";

    //Take action if sensornode data risk has changed
    if(m_sensornode[sensornodedata.getSensorNodeID()].compare(sensor_risk_category) != 0){
        m_sensornode[sensornodedata.getSensorNodeID()] = sensor_risk_category;
        m_healthrisk_counter = 0;

        Request request(Request::SENSOR_DATA, m_id, sensornodedata.getSensorNodeID(), sensor_risk_category);
        Container c_req(request);
        getConference().send(c_req);
    } else {
        m_healthrisk_counter++;
    }

    //Evaluate health risk
    if(m_healthrisk_counter == 2){
        m_health_risk = sensor_risk_category;
    }

    //View Data
    CLOG1 << "-------------------------------------------------" << endl;
    CLOG1 << " RECEIVED CONTENT " << endl;
    CLOG1 << " " << sensornodedata.toString() << " risk: " << m_sensornode[sensornodedata.getSensorNodeID()] << endl;
    CLOG1 << " sent at " << sentTS.getYYYYMMDD_HHMMSS() << endl;
    CLOG1 << " received at " << receivedTS.getYYYYMMDD_HHMMSS() << endl;
    CLOG1 << " processed at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
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

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        while(!m_buffer.isEmpty()){
            Container c = m_buffer.leave();

            if(c.getDataType() == Request::ID()) {
                BodyHubModule::processRequest(c.getData<Request>());
            } else if (c.getDataType() == SensorNodeData::ID()) {
                BodyHubModule::processSensorNodeData(c.getData<SensorNodeData>(),c.getSentTimeStamp(),c.getReceivedTimeStamp());
            }
        } 

        PulseAckMessage pulseackmessage;
        Container ccc(pulseackmessage);
        getConference().send(ccc);
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}