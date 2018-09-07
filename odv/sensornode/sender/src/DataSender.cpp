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
    string type;
    double data;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) { 

        while (!m_buffer.isEmpty()) {

            Container container = m_buffer.leave();
            data = container.getData<FilteredData>().getSensorData();
            type = container.getData<FilteredData>().getSensorType();

            std::cout << "Dado recebido de um " << type << ": " << data << std::endl;

            string package = type;
            package += '-';
            package += to_string(data);
            sender.send(package);
        }

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
