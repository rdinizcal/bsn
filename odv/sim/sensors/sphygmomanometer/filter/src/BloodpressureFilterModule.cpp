#include "BloodpressureFilterModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;

using namespace bsn::msg::taskMsg;

BloodpressureFilterModule::BloodpressureFilterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bloodpressurefilter"),
    buffer(),
    type("bloodpressure"),
    active(true),
    params({{"freq",0.90},{"m_avg",5}}),
    filterSystolic(5),
    filterDiastolic(5)
    {}

BloodpressureFilterModule::~BloodpressureFilterModule() {}

void BloodpressureFilterModule::setUp() {
    addDataStoreFor(BLOODPRESSUREFILTERMODULE_MSG_QUE, buffer);
    
}

void BloodpressureFilterModule::tearDown() {
    
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BloodpressureFilterModule::body(){

    Container container;
    double dataS;
    double dataD;
    double filterS;
    double filterD;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
      
        while(!buffer.isEmpty()){ // Receive control command and module update
            container = buffer.leave();

            dataS = container.getData<BloodpressureCollectTaskMessage>().getDataS();
            dataD = container.getData<BloodpressureCollectTaskMessage>().getDataD();
         
        /*
         * Module execution
         */
        
        // TASK: Filter data with moving average
            filterSystolic.setRange(params["m_avg"]);
            filterSystolic.insert(dataS, "bpms");
            filterS = filterSystolic.getValue("bpms");
           

            filterDiastolic.setRange(params["m_avg"]);
            filterDiastolic.insert(dataD, "bpmd");
            filterD = filterDiastolic.getValue("bpmd");
           
            BloodpressureFilterTaskMessage filterMsg(filterS, filterD);
            Container filterContainer(filterMsg);
            getConference().send(filterContainer);
        
        }
    }
    

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
