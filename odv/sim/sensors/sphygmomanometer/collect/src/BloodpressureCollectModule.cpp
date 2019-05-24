#include "BloodpressureModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;
using namespace bsn::msg::info;
using namespace bsn::msg::control;

BloodpressureModule::BloodpressureModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bloodpressure"),
    buffer(),
    type("bloodpressure"),
    active(true),
    params({{"freq",0.90},{"m_avg",5}}),
    filterSystolic(5),
    filterDiastolic(5),
    {}

BloodpressureModule::~BloodpressureModule() {}

void BloodpressureModule::setUp() {
    addDataStoreFor(903, buffer);
    
}

void BloodpressureModule::tearDown() {
    
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BloodpressureModule::body(){

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
            /*
            **
            ***
            //  Que valor dataS e DataD devem receber?
            // Que valor Risk deve receber?
            ***
            **
            */
            // TASK: Collect bloodpressure data            

            SensorData sdata(type, dataS, risk);
            Container sdataContainer(sdata);
            getConference().send(sdataContainer);

            SensorData ddata(type, dataD, risk);
            Container ddataContainer(ddata);
            getConference().send(ddataContainer);

        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
