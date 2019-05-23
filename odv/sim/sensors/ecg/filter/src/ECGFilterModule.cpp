#include "ECGFilterModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;
using namespace bsn::msg::info;
using namespace bsn::msg::control;

ECGFilterModule::ECGFilterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ecg"),
    buffer(),
    type("ecg"),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    filter(5)
     {}

ECGFilterModule::~ECGFilterModule() {}

void ECGFilterModule::setUp() {

}

void ECGFilterModule::tearDown() {
   
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ECGFilterModule::body() {
  
    double data;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
       
        /*
         * Module execution
         */
        if((rand() % 100)+1 < int32_t(params["freq"]*100)){
            
             // TASK: Filter data with moving average
            filter.setRange(params["m_avg"]);
            filter.insert(data, type);
            data = filter.getValue(type);
            
        }

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
