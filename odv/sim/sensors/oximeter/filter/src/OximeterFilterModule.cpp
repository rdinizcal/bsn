#include "OximeterFilterModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;

using namespace bsn::msg::taskMsg;

OximeterFilterModule::OximeterFilterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "oximeterfilter"),
    buffer(),
    type("oximeter"),
    active(true),
    params({{"freq",0.90},{"m_avg",5}}),
    filter(5),
    falhaRand()
    {}

OximeterFilterModule::~OximeterFilterModule() {}

void OximeterFilterModule::setUp() {
    addDataStoreFor(OXIMETERCOLLECTMODULE_MSG_QUE, buffer);
    addDataStoreFor(OXIMETERFILTERMODULE_MSG_QUE, buffer);
}

void OximeterFilterModule::tearDown() {
    
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode OximeterFilterModule::body(){

    Container container;
    double data;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        if(falhaRand.seOcorreuFalha() ){
                usleep(40000);
        }

        /*
         * Module execution
         */
        while(!buffer.isEmpty()){ // Receive control command and module update
            container = buffer.leave();
            data = container.getData<OximeterCollectTaskMessage>().getData();
            
         // TASK: Filter data with moving average
            filter.setRange(params["m_avg"]);
            filter.insert(data, type);
            data = filter.getValue(type);

            OximeterFilterTaskMessage sdata(data);
            Container filterContainer(sdata);
            getConference().send(filterContainer);

        }
    }
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}