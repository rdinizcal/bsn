#include "BodyHubModule.h"

#include "opendavinci/odcore/base/FIFOQueue.h"

#include "openbasn/data/SensorData.h"
#include "openbasn/model/sensor/Sensor.h"

#include <iostream>

using namespace std;

using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;

using namespace openbasn::data;
using namespace openbasn::model::sensor;

BodyHubModule::BodyHubModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bodyhub")
     {}

BodyHubModule::~BodyHubModule() {}

void BodyHubModule::setUp() {}

void BodyHubModule::tearDown() {}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 
    FIFOQueue fifo;
    addDataStoreFor(7, fifo);

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        //Receive Data
        Container c = fifo.leave();
        SensorData sd = c.getData<SensorData>();

        //Categorize Data
        string health_risk;
        double data = sd.getData();
        if(sd.getSensorType() == Sensor::THERMOMETER){
            if(37 >= data && data > 35){
                health_risk = "normal";
            } else if ( (35 >= data && data > 30) || (38 >= data && data > 37) ) {
                health_risk = "moderate";
            } else if ( (50 >= data && data > 38) || (30 >= data && data > 0) ) {
                health_risk = "high";
            } else {
                health_risk = "unknown";
            }
        } else if (sd.getSensorType() == Sensor::OXIMETER) {
            if( 100 >= data && data > 94) {
                health_risk = "normal";
            } else if ( 94 >= data && data > 90) {
                health_risk = "moderate";
            } else if ( 90 >= data && data > 0) {
                health_risk = "high";
            } else {
                health_risk = "unknown";
            }
        } else if (sd.getSensorType() == Sensor::ECG) {
            if ( (150 >= data && data > 120) || (80 >= data && data > 0) ) {
                health_risk = "high";
            } else if (120 >= data && data > 80) {
                health_risk = "normal";
            }else {
                health_risk = "unknown";
            }
            }
        //View Data
        cout << "-------------------------------------------------" << endl;
        cout << sd.toString() << " | Risk: " << health_risk << endl;
        cout << " sent at " << c.getSentTimeStamp().getYYYYMMDD_HHMMSS() << endl;
        cout << " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSS() << endl;
        cout << " processed at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
        cout << "-------------------------------------------------" << endl;

        //Persist Data

    }
    
return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}