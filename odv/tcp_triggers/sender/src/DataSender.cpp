#include "../include/DataSender.hpp"

DataSender::DataSender(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "handler"),
    m_buffer() {}

DataSender::~DataSender() {}

TCPSend sender("localhost",8080);

void DataSender::setUp() {
    addDataStoreFor(873, m_buffer);
    sender.connect();
}

void DataSender::tearDown(){
    sender.disconnect();
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataSender::body(){    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while(!m_buffer.isEmpty()){
            Container container = m_buffer.leave();
            std::cout << "Dado recebido: " << (container.getData<RawData>().getSensorData()) << std::endl;
            sender.send(to_string(container.getData<RawData>().getSensorData()));
        }

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
