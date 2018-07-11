#include "../include/DataCollectorModule.hpp"

DataCollectorModule::DataCollectorModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "DataCollectorModule"),
    mGeneratedData() {}

DataCollectorModule::~DataCollectorModule() {}

void DataCollectorModule::setUp() {}

void DataCollectorModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataCollectorModule::body(){
    average filter;
    DataCollector dataCollector;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        //Gera o dado
        mGeneratedData = dataCollector.generateDataByNormalDist(747.52, 102.4);

        // Converte para Celsius
        mGeneratedData = to_celsius(mGeneratedData);
        cout << "Unfiltered " << mGeneratedData << ' ';

        // Filtra
        filter.insert(mGeneratedData);
        double filtered_data = filter.get_value();

        //Atribui dados à estrutura de dados especifica
        RawData rawdata(filtered_data);
        
        //Encapsula dado
        Container container(rawdata);

        //Envia pacote
        getConference().send(container);

        std::cout << "Dado " << filtered_data << " gerado e enviado." << std::endl;
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
