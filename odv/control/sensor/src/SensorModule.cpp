#include "SensorModule.hpp"

using namespace std;

using namespace odcore::base::module;
using namespace odcore::data;

SensorModule::SensorModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "SensorModule")
    {}

SensorModule::~SensorModule() {}

void SensorModule::setUp() {
}

void SensorModule::tearDown() {
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorModule::body() {
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}