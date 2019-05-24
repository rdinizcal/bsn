#include "OximeterTransferModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;
using namespace bsn::msg::info;
using namespace bsn::msg::control;

OximeterModule::OximeterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "oximeter"),
    buffer(),
    type("oximeter"),
    active(true),
    params({{"freq",0.90},{"m_avg",5}}),
    filter(5),
    {}

OximeterModule::~OximeterModule() {}

void OximeterModule::setUp() {
    addDataStoreFor(901, buffer);
    
    
}

void OximeterModule::tearDown() {

}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode OximeterModule::body(){

    double data;
    double risk;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        
        while(!buffer.isEmpty()){ // Receive control command and module update
            
            // Recebe dados do Task anterior
            container = buffer.leave();
            active = container.getData<OximeterControlCommand>().getActive();
        
        /*
         * Module execution
         */
            
             // TASK: Transfer information to CentralHub
            risk = sensorConfig.evaluateNumber(data);
            
            SensorData sdata(type, data, risk);
            Container sdataContainer(sdata);
            getConference().send(sdataContainer);

        }

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}