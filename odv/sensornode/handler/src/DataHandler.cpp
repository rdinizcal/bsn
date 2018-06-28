#include "../include/DataHandler.hpp"

DataHandler::DataHandler(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "handler"),
    m_buffer() {}

DataHandler::~DataHandler() {}

void DataHandler::setUp() {
    addDataStoreFor(873, m_buffer);
}

void DataHandler::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataHandler::body(){
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while(!m_buffer.isEmpty()){
            Container container = m_buffer.leave();
            std::cout << "Dado recebido: " << (container.getData<RawData>().getSensorData()) << std::endl;
        }

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
