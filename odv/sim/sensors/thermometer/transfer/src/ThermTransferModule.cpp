#include "ThermCollector.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;

ThermCollector::ThermCollector(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "thermometer"),
    buffer(),
    type("thermometer"),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    filter(5),
    sensorConfig(),
    {}

ThermCollector::~ThermCollector() {}

void ThermCollector::setUp() {
    addDataStoreFor(900, buffer);

}

void ThermCollector::tearDown() {
    
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ThermCollector::body(){

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
