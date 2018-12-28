#include "SensorModule.hpp"

using namespace bsn::communication;

TCPReceive server(8000);

SensorModule::SensorModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "SensorModule")
    {}

SensorModule::~SensorModule() {}

void SensorModule::setUp() {
    server.set_port(getIdentifier());
    server.start_connection();
}

void SensorModule::tearDown() {
    server.stop_connection();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorModule::body() {
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    
        std::string package = server.get_package();
        
        if (package != ""){
            
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}