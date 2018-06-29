#include "../include/tcp_listener.hpp"

using namespace std;

// We add some of OpenDaVINCI's namespaces for the sake of readability.
using namespace odcore::base::module;
using namespace odcore::data;
using namespace bsn::data;

TCPReceiveBytes server(8080);

TimeTriggeredSender::TimeTriggeredSender(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "TimeTriggeredSender")
    {}

TimeTriggeredSender::~TimeTriggeredSender() {}

void TimeTriggeredSender::setUp() {
    server.start_connection();
    cout << "This method is called before the component's body is executed." << endl;
}

void TimeTriggeredSender::tearDown() {
    server.stop_connection();
    cout << "This method is called after the program flow returns from the component's body." << endl;
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode TimeTriggeredSender::body() {
    sleep(2);
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