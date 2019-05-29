#include "ThermTransferCollector.hpp"
#define THERMTRANSFERMODULE_MSG_QUE 4043

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;

ThermTransferCollector::ThermTransferCollector(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "thermometer"),
    buffer(),
    type("thermometer"),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    filter(5),
    sensorConfig(),
    {}

ThermTransferCollector::~ThermTransferCollector() {}

void ThermTransferCollector::setUp() {
    addDataStoreFor(THERMTRANSFERMODULE_MSG_QUE, buffer);

}

void ThermTransferCollector::tearDown() {
    
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ThermTransferCollector::body(){

    double data;
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        

        while(!buffer.isEmpty()){
           
            container = buffer.leave();
            data = container.getData<SensorData>().getData();
            
            // TASK: Transfer information to CentralHub
            risk = sensorConfig.evaluateNumber(data);
        
            SensorData sdata(type, data, risk);
            Container sdataContainer(sdata);
            getConference().send(sdataContainer);
            
            
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
