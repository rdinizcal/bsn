#include "../include/DataCollectorModule.hpp"

DataCollectorModule::DataCollectorModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "DataCollectorModule"),
    mGeneratedData() {}

DataCollectorModule::~DataCollectorModule() {}

void DataCollectorModule::setUp() {}

void DataCollectorModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataCollectorModule::body(){

    DataCollector dataCollector;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        //Gera o dado
        mGeneratedData = dataCollector.generateDataByNormalDist(747.52, 102.4);

        // Converte para Celsius
        mGeneratedData = to_celsius(mGeneratedData);

        //Atribui dados à estrutura de dados especifica
        RawData rawdata(mGeneratedData);
        
        //Encapsula dado
        Container container(rawdata);

        //Envia pacote
        getConference().send(container);

        std::cout << "Dado " << mGeneratedData << " gerado e enviado." << std::endl;
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
