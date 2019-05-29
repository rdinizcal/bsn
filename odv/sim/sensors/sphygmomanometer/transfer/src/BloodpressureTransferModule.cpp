#include "BloodpressureTransferModule.hpp"
#define BLOODPRESURETRANSFERMODULE_MSG_QUE 4033

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::resource;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;
using namespace bsn::msg::info;
using namespace bsn::msg::control;

BloodpressureTransferModule::BloodpressureTransferModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bloodpressure"),
    buffer(),
    type("bloodpressure"),
    active(true),
    params({{"freq",0.90},{"m_avg",5}}),
    filterSystolic(5),
    filterDiastolic(5),
    {}

BloodpressureTransferModule::~BloodpressureTransferModule() {}

void BloodpressureTransferModule::setUp() {
    //srand(time(NULL));
    addDataStoreFor(BLOODPRESURETRANSFERMODULE_MSG_QUE, buffer);

}

void BloodpressureTransferModule::tearDown() {
    
}


odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BloodpressureTransferModule::body(){

    Container container;
    double dataS;
    double dataD;
    double risk;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        

      

        while(!buffer.isEmpty()){ // Receive control command and module update
            container = buffer.leave();

            active = container.getData<BloodpressureControlCommand>().getActive();
        

       
        /*
         * Module execution
         */        
            
        //TASK: Transfer information to CentralHub
            risk = sensorConfigSystolic.evaluateNumber(dataS);
            SensorData sdataS("bpms", dataS, risk);
            Container sdataSContainer(sdataS);
            getConference().send(sdataSContainer);

    

            risk = sensorConfigDiastolic.evaluateNumber(dataD);
            SensorData sdataD("bpmd", dataD, risk);
            Container sdatadContainer(sdataD);
            getConference().send(sdatadContainer);

            

        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
