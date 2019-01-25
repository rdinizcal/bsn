#include "JokerModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::msg::data;

JokerModule::JokerModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "joker"),
    buffer() {}

JokerModule::~JokerModule() {}

void JokerModule::setUp() {}

void JokerModule::tearDown() {}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode JokerModule::body(){

    Container container;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {        
        SensorData sdata("null", 0, 0);
        Container sdataContainer(sdata);
        getConference().send(sdataContainer);
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
