#include "../include/tcp_listener.hpp"

using namespace std;

using namespace odcore::base::module;
using namespace odcore::data;
using namespace bsn::data;

TCPReceive server(8000);

TimeTriggeredSender::TimeTriggeredSender(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "TimeTriggeredSender")
    {}

TimeTriggeredSender::~TimeTriggeredSender() {}

void TimeTriggeredSender::setUp() {
    // É usado o id do módulo para receber a porta desejada
    server.set_port(getIdentifier());
    server.start_connection();    
}

void TimeTriggeredSender::tearDown() {
    server.stop_connection();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode TimeTriggeredSender::body() {        
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        string package = server.get_package();
        if(package != ""){            
            cout << "Pacote: \'" << package;
            timespec ts;
            SensorData data(0,0,package,ts);
            Container c(data);        
            getConference().send(c);
            cout << "\' recebido e encaminhado para o processamento." << endl;            
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}