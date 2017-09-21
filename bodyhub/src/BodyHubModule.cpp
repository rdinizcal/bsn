#include "BodyHubModule.hpp"

#include <iostream>
#include "SensorData.hpp"
#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/data/Container.h"

using namespace std;
using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;

BodyHubModule::BodyHubModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "BodyHubModule")
     {}

BodyHubModule::~BodyHubModule() {}

void BodyHubModule::setUp() {}

void BodyHubModule::tearDown() {}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 
    FIFOQueue fifo;
    addDataStoreFor(7, fifo);

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        //ReceiveData
        Container c = fifo.leave();
        if(c.getDataType() == SensorData::ID()){
            SensorData sd = c.getData<SensorData>();

            cout << "-------------------------------------------------" << endl;
            cout << sd.toString() << endl;
            cout << " sent at " << c.getSentTimeStamp().getYYYYMMDD_HHMMSS() << endl;
            cout << " received at " << c.getReceivedTimeStamp().getYYYYMMDD_HHMMSS() << endl;
            cout << "-------------------------------------------------" << endl;
        }
        //PersistData

        //ViewData
    }
    
return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}