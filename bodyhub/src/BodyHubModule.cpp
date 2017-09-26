#include "BodyHubModule.h"

#include "opendavinci/odcore/base/FIFOQueue.h"

#include "openbasn/data/SensorData.h"
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
    m_id(getIdentifier())
     {}

BodyHubModule::~BodyHubModule() {}

void BodyHubModule::setUp() {}

void BodyHubModule::tearDown() {}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 
    FIFOQueue fifo;
    addDataStoreFor(fifo);
    int32_t clock_tick = 0;
    
    map <uint32_t, string> snm_risk;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        Container c = fifo.leave();

        if(c.getDataType() == Request::ID()) {
            //Receive Request
            Request req = c.getData<Request>();

            if(req.getRequestType() == Request::REGISTER) {
                snm_risk.insert(pair<uint32_t, string>(req.getSourceID(), "low"));
                cout << "SensorNode#" << req.getSourceID() << " successfully registered." << endl;
            } else if (req.getRequestType() == Request::UNREGISTER) {
                snm_risk.erase(req.getSourceID());
                cout << "SensorNode#" << req.getSourceID() << " successfully unregistered." << endl;                
            }

        } else if(c.getDataType() == SensorData::ID()){
            //Receive SensorData
            SensorData sd = c.getData<SensorData>();

            //Categorize Data
            double health_risk_value = 0;
            double data = sd.getData();
            if(sd.getSensorType() == Sensor::THERMOMETER){

                if(37 >= data && data > 35){
                    health_risk_value += 0.1; 
                } else if ( (35 >= data && data > 30) || (38 >= data && data > 37) ) {
                    health_risk_value += 1;
                } else if ( (50 >= data && data > 38) || (30 >= data && data > 0) ) {
                    health_risk_value += 10;
                } else {
                    health_risk_value = 0; 
                }

            } else if (sd.getSensorType() == Sensor::OXIMETER) {

                if( 100 >= data && data > 94) {
                    health_risk_value += 0.1;
                } else if ( 94 >= data && data > 90) {
                    health_risk_value += 1;
                } else if ( 90 >= data && data > 0) {
                    health_risk_value += 10;
                } else {
                    health_risk_value = 0;
                }

            } else if (sd.getSensorType() == Sensor::ECG) {
                
                if ( (150 >= data && data > 120) || (80 >= data && data > 0) ) {
                    health_risk_value += 10;
                } else if (120 >= data && data > 80) {
                    health_risk_value += 0.1;
                } else {
                    health_risk_value = 0;
                }

            } else {
                cerr << "Could not recognize sensor." << endl;
            }

            if(0 < health_risk_value && health_risk_value < 1) {
                snm_risk[sd.getSensorNodeID()] = "low";
            } else if (1 <= health_risk_value && health_risk_value < 10) {
                snm_risk[sd.getSensorNodeID()] = "moderate";
            } else if (10 <= health_risk_value) {
                snm_risk[sd.getSensorNodeID()] = "high";
            } else {
                snm_risk[sd.getSensorNodeID()] = "unknown";
            }

            //View Data
            cout << "-------------------------------------------------" << endl;
            cout << sd.toString() << " | risk: " << snm_risk[sd.getSensorNodeID()] << endl;
            cout << " sent at " << c.getSentTimeStamp().getYYYYMMDD_HHMMSS() << endl;
            cout << " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSS() << endl;
            cout << " processed at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
            cout << " clock_tick: " << clock_tick << endl;
            cout << "-------------------------------------------------" << endl;

            //Persist Data

        }

        //Request Data
        if(clock_tick%15 == 0){

            map<uint32_t, string>::iterator it = snm_risk.begin();
            for (pair<uint32_t, string> element : snm_risk) {
                if( element.second.compare("low") == 0) {
                    Request request(Request::SENSOR_DATA, m_id, element.first);
                    Container c_req(request);
                    getConference().send(c_req);
                }
            }

        } 
        
        if(clock_tick%5 == 0) {

            map<uint32_t, string>::iterator it = snm_risk.begin();
            for (pair<uint32_t, string> element : snm_risk) {
                if( element.second.compare("moderate") == 0) {
                    Request request(Request::SENSOR_DATA, m_id, element.first);
                    Container c_req(request);
                    getConference().send(c_req);
                }
            }

        } 
        
        if (clock_tick%1 == 0) {

            map<uint32_t, string>::iterator it = snm_risk.begin();
            for (pair<uint32_t, string> element : snm_risk) {
                if( element.second.compare("high") == 0) {
                    Request request(Request::SENSOR_DATA, m_id, element.first);
                    Container c_req(request);
                    getConference().send(c_req);
                }
            }

        }

        clock_tick++;
    }
    
return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}