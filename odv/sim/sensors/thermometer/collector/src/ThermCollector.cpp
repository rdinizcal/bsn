#include "ThermCollectorModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::range;
using namespace bsn::generator;
using namespace bsn::operation;
using namespace bsn::configuration;

using namespace bsn::msg::data;

ThermCollectorModule::ThermCollectorModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "thermometer"),
    buffer(),
    type("thermometer"),
    active(true),
    params({{"freq",0.9},{"m_avg",5}}),
    filter(5),
    {}

ThermCollectorModule::~ThermCollectorModule() {}

void ThermCollectorModule::setUp() {
    //srand(time(NULL));
    addDataStoreFor(900, buffer);
    
   
}

void ThermCollectorModule::tearDown() {
   
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ThermCollectorModule::body(){

    double data;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
           
        { // TASK: Collect thermometer data with data_accuracy
            
            data = markov.calculate_state();
            
            double offset = (1 - data_accuracy + (double)rand() / RAND_MAX * (1 - data_accuracy)) * data;

            CollectedData cData(type, data);
            Container container(cData);
            getConference().send(container);
        }
        
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
