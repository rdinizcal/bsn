#include "../include/DataSender.hpp"

using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;
using namespace bsn::data;

DataSender::DataSender(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "datasender"),
    m_buffer() {}

DataSender::~DataSender() {}

TCPSend sender(8000);

void DataSender::setUp() {
    // Recebe FilteredData
    ip = getKeyValueConfiguration().getValue<std::string>("datasender.ip");
    addDataStoreFor(876, m_buffer);
    sender.set_port(getIdentifier());
    sender.setIP(ip);
    sender.connect();
}

void DataSender::tearDown(){    
    sender.disconnect();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataSender::body(){    
    // Retira o id a partir da porta que lhe foi dada
    int id = getIdentifier() - 8080;
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {        
        while(!m_buffer.isEmpty()){
            Container container = m_buffer.leave();
            std::cout << "Dado recebido: " << to_string(container.getData<FilteredData>().getSensorData()) << std::endl;        
            string package = to_string(id);
            package += '-';
            package += to_string(container.getData<FilteredData>().getSensorData());
            sender.send(package);
        }

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
