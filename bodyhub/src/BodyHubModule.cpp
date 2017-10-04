#include "BodyHubModule.h"

#include "openbasn/data/SensorNodeData.h"
#include "openbasn/message/Request.h"
#include "openbasn/model/sensor/Sensor.h"

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
    m_datalog << "Sensor Node #,Active Sensors,Thermometer Data,ECG Data,Oximeter Data,Health Risk,Sent at,Received at,Processed at\n";
}

void BodyHubModule::tearDown() {
    m_datalog.close();
    
    //Unregister all

    //Loop thrugh all registered nodes
    //    m_sensornode.erase(req.getSourceID());
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        Container c = m_buffer.leave();

        if(c.getDataType() == Request::ID()) {
            Request req = c.getData<Request>();

            if(req.getRequestType() == Request::REGISTER) {
                m_sensornode.insert(pair<uint32_t, string>(req.getSourceID(), "low"));
            } else if (req.getRequestType() == Request::UNREGISTER) {
                m_sensornode.erase(req.getSourceID());
            }

        } else if(c.getDataType() == SensorNodeData::ID()){
            //Receive SensorNodeData
            SensorNodeData sd = c.getData<SensorNodeData>();

            //Categorize Data
            double health_risk_value = 0;
            double thermometer_data = 0;
            double ecg_data = 0;
            double oximeter_data = 0;
            map<int32_t,double> sensor_data_map = sd.getSensorDataMap();

            map<int32_t,double>::iterator it = sensor_data_map.begin();
            for(pair<int32_t,double> element : sensor_data_map){

                int32_t sensor_type = element.first;
                double sensor_data = element.second;

                if(sensor_type == Sensor::THERMOMETER){
    
                    if(37 >= sensor_data && sensor_data > 35){
                        health_risk_value += 0.1; 
                    } else if ( (35 >= sensor_data && sensor_data > 30) || (38 >= sensor_data && sensor_data > 37) ) {
                        health_risk_value += 1;
                    } else if ( (50 >= sensor_data && sensor_data > 38) || (30 >= sensor_data && sensor_data > 0) ) {
                        health_risk_value += 5;
                    } else {
                        health_risk_value = 0; 
                    }

                    thermometer_data = sensor_data;
    
                } else if (sensor_type == Sensor::OXIMETER) {
    
                    if( 100 >= sensor_data && sensor_data > 94) {
                        health_risk_value += 0.1;
                    } else if ( 94 >= sensor_data && sensor_data > 90) {
                        health_risk_value += 1;
                    } else if ( 90 >= sensor_data && sensor_data > 0) {
                        health_risk_value += 5;
                    } else {
                        health_risk_value = 0;
                    }

                    oximeter_data = sensor_data;
    
                } else if (sensor_type == Sensor::ECG) {
                    
                    if ( (150 >= sensor_data && sensor_data > 120) || (80 >= sensor_data && sensor_data > 0) ) {
                        health_risk_value += 5;
                    } else if (120 >= sensor_data && sensor_data > 80) {
                        health_risk_value += 0.1;
                    } else {
                        health_risk_value = 0;
                    }
                    
                    ecg_data = sensor_data;

                } else {
                    cerr << "Could not recognize sensor." << endl;
                }
            }

            if(0 < health_risk_value && health_risk_value < 1) {
                m_sensornode[sd.getSensorNodeID()] = "low";
            } else if (1 <= health_risk_value && health_risk_value < 5) {
                m_sensornode[sd.getSensorNodeID()] = "moderate";
            } else if (5 <= health_risk_value && health_risk_value < 20) {
                m_sensornode[sd.getSensorNodeID()] = "high";
            } else {
                m_sensornode[sd.getSensorNodeID()] = "unknown";
            }

            //View Data
            cout << "-------------------------------------------------" << endl;
            cout << " RECEIVED CONTENT " << endl;
            cout << " " << sd.toString() << " risk: " << m_sensornode[sd.getSensorNodeID()] << endl;
            cout << " sent at " << c.getSentTimeStamp().getYYYYMMDD_HHMMSS() << endl;
            cout << " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSS() << endl;
            cout << " processed at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
            cout << " clock tick: " << m_clock << endl;
            cout << "-------------------------------------------------" << endl;

            //Persist Data
            //file header
            //"Sensor Node #, Active Sensors, Thermometer Data, ECG Data, Oximeter Data, Health Risk, Sent at, Received at, Processed at";
            m_datalog << sd.getSensorNodeID() << ",";
            m_datalog << sensor_data_map.size() << ",";

            if(thermometer_data > 0){
                m_datalog << thermometer_data << ",";
            } else {
                m_datalog << "- ,";
            }

            if(ecg_data > 0){
                m_datalog << ecg_data << ",";
            } else {
                m_datalog << "- ,";
            }

            if(oximeter_data > 0){
                m_datalog << oximeter_data << ",";
            } else {
                m_datalog << "- ,";
            }

            m_datalog << m_sensornode[sd.getSensorNodeID()] << ",";
            m_datalog << c.getSentTimeStamp().getYYYYMMDD_HHMMSS() << ",";
            m_datalog << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSS() << ",";
            m_datalog << TimeStamp().getYYYYMMDD_HHMMSS() << "\n";

        } else {

            //Request Data
            if(m_clock%15 == 0){
    
                map<uint32_t, string>::iterator it = m_sensornode.begin();
                for (pair<uint32_t, string> element : m_sensornode) {
                    if( element.second.compare("low") == 0) {
                        Request request(Request::SENSOR_DATA, m_id, element.first);
                        Container c_req(request);
                        getConference().send(c_req);
                    }
                }
    
            } 
            
            if(m_clock%5 == 0) {
    
                map<uint32_t, string>::iterator it = m_sensornode.begin();
                for (pair<uint32_t, string> element : m_sensornode) {
                    if( element.second.compare("moderate") == 0) {
                        Request request(Request::SENSOR_DATA, m_id, element.first);
                        Container c_req(request);
                        getConference().send(c_req);
                    }
                }
    
            } 
            
            if (m_clock%1 == 0) {
    
                map<uint32_t, string>::iterator it = m_sensornode.begin();
                for (pair<uint32_t, string> element : m_sensornode) {
                    if( element.second.compare("high") == 0 || element.second.compare("unknown") == 0) {
                        Request request(Request::SENSOR_DATA, m_id, element.first);
                        Container c_req(request);
                        getConference().send(c_req);
                    }
                }
    
            }
    
            m_clock++;
        }

    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}