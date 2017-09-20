#include <iostream>

#include "SensorNodeModule.h"
#include "Thermometer.h"

using namespace std;

SensorNodeModule::SensorNodeModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "SensorNodeModule")
     {}

SensorNodeModule::~SensorNodeModule() {}

void SensorNodeModule::setUp() {}

void SensorNodeModule::tearDown() {}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorNodeModule::body() { 
    
    //Instantiate Sensors
    Thermometer thermometer(1, 10, true, 36, 3);

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        float data = thermometer.getData();

        cout << "Temperature: " << data << endl;
        //if(sendDataEvent) sendData
    }
    
    //Destroy Sensors

return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}