#include "ThermFilterModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;

using namespace bsn::msg::taskMsg;

ThermFilterModule::ThermFilterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "thermometerfilter"),
    buffer(),
    type("thermometer"),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    filter(5),
    falhaRand()
    {}

ThermFilterModule::~ThermFilterModule() {}

void ThermFilterModule::setUp() {
    addDataStoreFor(THERMCOLLECTMODULE_MSG_QUE, buffer);
    addDataStoreFor(THERMFILTERMODULE_MSG_QUE, buffer);
    
}

void ThermFilterModule::tearDown() {

}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ThermFilterModule::body(){

    Container container;
    double data;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        if(falhaRand.seOcorreuFalha() ){
                usleep(40000); 
        }

        /*
         * Module execution
         */
            // TASK: Filter data with moving average
        while(!buffer.isEmpty()){ // Receive control command and module update
            container = buffer.leave();
            data = container.getData<ThermometerCollectTaskMessage>().getData();

            filter.setRange(params["m_avg"]);
            filter.insert(data, type);
            data = filter.getValue(type);
     
            ThermometerFilterTaskMessage sdata(data);
            Container filterContainer(sdata);
            getConference().send(filterContainer);
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
