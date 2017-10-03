#include "BodyHubModule.h"

#include "opendavinci/odcore/base/FIFOQueue.h"

#include "openbasn/data/SensorNodeData.h"
#include "openbasn/message/Request.h"
#include "openbasn/model/sensor/Sensor.h"

#include <iostream>
#include <fstream>
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
    sensornode_risk()
    {}

BodyHubModule::~BodyHubModule() {}

void BodyHubModule::setUp() {}

void BodyHubModule::tearDown() {
    //Unregister all

    //Loop thrugh all registered nodes
    //    sensornode_risk.erase(req.getSourceID());
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 
    FIFOQueue fifo;
    addDataStoreFor(fifo);
    int32_t clock_tick = 0;

    ofstream data_file;
    data_file.open("bodyhub"+to_string(m_id)+".csv");
    //file header
    data_file << "Sensor Node #, Active Sensors, Thermometer Data, ECG Data, Oximeter Data, Health Risk, Sent at, Received at, Processed at\n";

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        Container c = fifo.leave();

        if(c.getDataType() == Request::ID()) {
            Request req = c.getData<Request>();

            if(req.getRequestType() == Request::REGISTER) {
                sensornode_risk.insert(pair<uint32_t, string>(req.getSourceID(), "low"));
            } else if (req.getRequestType() == Request::UNREGISTER) {
                sensornode_risk.erase(req.getSourceID());
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
                sensornode_risk[sd.getSensorNodeID()] = "low";
            } else if (1 <= health_risk_value && health_risk_value < 5) {
                sensornode_risk[sd.getSensorNodeID()] = "moderate";
            } else if (5 <= health_risk_value && health_risk_value < 20) {
                sensornode_risk[sd.getSensorNodeID()] = "high";
            } else {
                sensornode_risk[sd.getSensorNodeID()] = "unknown";
            }

            //View Data
            cout << "-------------------------------------------------" << endl;
            cout << " RECEIVED CONTENT " << endl;
            cout << " " << sd.toString() << " risk: " << sensornode_risk[sd.getSensorNodeID()] << endl;
            cout << " sent at " << c.getSentTimeStamp().getYYYYMMDD_HHMMSS() << endl;
            cout << " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSS() << endl;
            cout << " processed at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
            cout << " clock_tick: " << clock_tick << endl;
            cout << "-------------------------------------------------" << endl;

            //Persist Data
            //file header
            //"Sensor Node #, Active Sensors, Thermometer Data, ECG Data, Oximeter Data, Health Risk, Sent at, Received at, Processed at";
            data_file << sd.getSensorNodeID() << ",";
            data_file << sensor_data_map.size() << ",";

            if(thermometer_data > 0){
                data_file << thermometer_data << ",";
            } else {
                data_file << "- ,";
            }

            if(ecg_data > 0){
                data_file << ecg_data << ",";
            } else {
                data_file << "- ,";
            }

            if(oximeter_data > 0){
                data_file << oximeter_data << ",";
            } else {
                data_file << "- ,";
            }

            data_file << sensornode_risk[sd.getSensorNodeID()] << ",";
            data_file << c.getSentTimeStamp().getYYYYMMDD_HHMMSS() << ",";
            data_file << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSS() << ",";
            data_file << TimeStamp().getYYYYMMDD_HHMMSS() << "\n";

        } else {

            //Request Data
            if(clock_tick%15 == 0){
    
                map<uint32_t, string>::iterator it = sensornode_risk.begin();
                for (pair<uint32_t, string> element : sensornode_risk) {
                    if( element.second.compare("low") == 0) {
                        Request request(Request::SENSOR_DATA, m_id, element.first);
                        Container c_req(request);
                        getConference().send(c_req);
                    }
                }
    
            } 
            
            if(clock_tick%5 == 0) {
    
                map<uint32_t, string>::iterator it = sensornode_risk.begin();
                for (pair<uint32_t, string> element : sensornode_risk) {
                    if( element.second.compare("moderate") == 0) {
                        Request request(Request::SENSOR_DATA, m_id, element.first);
                        Container c_req(request);
                        getConference().send(c_req);
                    }
                }
    
            } 
            
            if (clock_tick%1 == 0) {
    
                map<uint32_t, string>::iterator it = sensornode_risk.begin();
                for (pair<uint32_t, string> element : sensornode_risk) {
                    if( element.second.compare("high") == 0 || element.second.compare("unknown") == 0) {
                        Request request(Request::SENSOR_DATA, m_id, element.first);
                        Container c_req(request);
                        getConference().send(c_req);
                    }
                }
    
            }
    
            clock_tick++;
        }

    }
    
    data_file.close();

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}