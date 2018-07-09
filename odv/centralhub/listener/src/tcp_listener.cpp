#include "../include/tcp_listener.hpp"

using namespace std;

// We add some of OpenDaVINCI's namespaces for the sake of readability.
using namespace odcore::base::module;
using namespace odcore::data;
using namespace bsn::data;

TCPReceive server(8080);

TimeTriggeredSender::TimeTriggeredSender(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "TimeTriggeredSender")
    {}

TimeTriggeredSender::~TimeTriggeredSender() {}

void TimeTriggeredSender::setUp() {
    server.start_connection();    
}

void TimeTriggeredSender::tearDown() {
    server.stop_connection();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode TimeTriggeredSender::body() {    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        string package = server.get_package();
        if(package != ""){            
            cout << "Package: " << package << endl;
            timespec ts;
            SensorData data(0,0,package,ts);
            Container c(data);        
            getConference().send(c);
            cout << "done." << endl;            
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}