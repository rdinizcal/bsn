#include "ThermFilterModule.hpp"
#include "bsn/libbsn/include/msg/MessageQueueCodes.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::resource;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;

ThermFilterModule::ThermFilterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "thermometer"),
    buffer(),
    type("thermometer"),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    filter(5),
    {}

ThermFilterModule::~ThermFilterModule() {}

void ThermFilterModule::setUp() {
    addDataStoreFor(THERMFILTERMODULE_MSG_QUE, buffer);
    
}

void ThermFilterModule::tearDown() {

}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ThermFilterModule::body(){

    double data;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        /*
         * Module execution
         */
            // TASK: Filter data with moving average
        while(!buffer.isEmpty()){ // Receive control command and module update
            container = buffer.leave();
            data = container.getData<SensorData>().getData()

            filter.setRange(params["m_avg"]);
            filter.insert(data, type);
            data = filter.getValue(type);
            
            SensorData sdata(type, data, risk);
            Container sdataContainer(sdata);
            getConference().send(sdataContainer);
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
