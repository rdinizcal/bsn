#include "../include/DataCollector.hpp"

DataCollector::DataCollector(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "data"),
    generated_data() {}

DataCollector::~DataCollector() {}

void DataCollector::setUp() {}

void DataCollector::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataCollector::body(){
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        generated_data = generateDataByNormalDist(747.52, 102.4);
        RawData rawdata(generated_data);
        Container container(rawdata);
        getConference().send(container);
        std::cout << "Dado " << generated_data << " gerado e enviado" << std::endl;
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
