#include "BodyHubModule.h"

#include "openbasn/model/sensor/Sensor.h"
#include "openbasn/message/Acknowledge.h"

#include "opendavinci/generated/odcore/data/dmcp/PulseAckMessage.h"
#include "opendavinci/odcore/base/Thread.h"

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
    m_health_status("-"),
    m_sensor(),
    m_status_log()
    {}

BodyHubModule::~BodyHubModule() {}

void BodyHubModule::setUp() {
    //setup buffer
    addDataStoreFor(871, m_buffer);
    addDataStoreFor(872, m_buffer);
    addDataStoreFor(873, m_buffer);

    //setup m_status_log
    m_status_log.open("healthstatus_log.csv");
    
    //"Thermometer data, Thermometer risk, ECG data, ECG risk, Oximeter data, Oximeter risk, Health Status, Sent at, Received at, Processed at"
    m_status_log << "Thermometer data, Thermometer risk, ECG data, ECG risk, Oximeter data, Oximeter risk, Health Status, Sent at, Received at, Processed at\n";
}

void BodyHubModule::tearDown() {
    m_status_log.close();
}

string BodyHubModule::calculateHealthStatus(){
    double hr = 0;
    for(uint32_t i = 0; i < m_sensor.size(); i++){

        if (m_sensor[i].second.compare("low") == 0) {
            hr += 0.1;
        } else if (m_sensor[i].second.compare("moderate") == 0) {
            hr += 1;
        } else if (m_sensor[i].second.compare("high") == 0) {
            hr += 5;
        } 
    }

    return (hr<=0)?"unknown":(hr<1)?"Good":(hr<5)?"Moderate":(hr<20)?"Bad":"unknown";
}

void BodyHubModule::updateHealthStatus(SensorData sensordata){
    
    m_sensor[sensordata.getSensorType()-1].first = sensordata.getSensorData();
    m_sensor[sensordata.getSensorType()-1].second = sensordata.getSensorRisk();

    m_health_status = BodyHubModule::calculateHealthStatus();
}

void BodyHubModule::persistHealthStatus(TimeStamp sent_timestamp, TimeStamp received_timestamp){
    
    //"Thermometer data, Thermometer risk, ECG data, ECG risk, Oximeter data, Oximeter risk, Health Status, Sent at, Received at, Processed at"
    for(uint32_t i = 0; i < m_sensor.size(); i++){
        m_status_log << m_sensor[i].first << ",";
        m_status_log << m_sensor[i].second << ",";
    }

    m_status_log << m_health_status << ",";
    m_status_log << sent_timestamp.getYYYYMMDD_HHMMSS() << ",";
    m_status_log << received_timestamp.getYYYYMMDD_HHMMSS() << ",";
    m_status_log << TimeStamp().getYYYYMMDD_HHMMSS() << "\n";
}

void BodyHubModule::printHealthStatus(){
    cout << "----------------------------------------"<<endl;
    for(uint32_t i = 0; i < 3; i++){
        cout << ((i==0)?"Thermometer: ":(i==1)?"ECG: ":"Oximeter: ");
        cout << m_sensor[i].first << " | ";
        cout << m_sensor[i].second << " | " << endl;
    }
    cout << "Health Status: " << m_health_status << endl;
    cout << "----------------------------------------"<<endl;
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        while(!m_buffer.isEmpty()){
            Container container = m_buffer.leave();
            if (container.getDataType() == SensorData::ID()) {
                BodyHubModule::updateHealthStatus(container.getData<SensorData>());
                BodyHubModule::persistHealthStatus(container.getSentTimeStamp(), container.getReceivedTimeStamp());
            }
        }            
        
        BodyHubModule::printHealthStatus();

        PulseAckMessage pulseackmessage;
        Container container(pulseackmessage);
        getConference().send(container);
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}