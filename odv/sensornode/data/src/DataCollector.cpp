#include "../include/DataCollector.hpp"

DataCollector::DataCollector(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "data"),
    generated_data() {}

DataCollector::~DataCollector() {}

void DataCollector::setUp() {}

void DataCollector::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataCollector::body(){
    std::default_random_engine generator;
    std::random_device rd;
    generator.seed(rd());
    std::normal_distribution<double> distribution(747.52, 102.4); // 747.52 é o valor esperado e 102.4 é a variação do valor

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        generated_data = distribution(generator);
        RawData rawdata(generated_data);
        Container container(rawdata);
        getConference().send(container);
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
