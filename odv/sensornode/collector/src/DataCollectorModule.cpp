#include "../include/DataCollectorModule.hpp"

DataCollectorModule::DataCollectorModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "DataCollectorModule"),
    mSensor_id(getIdentifier()),
    mGeneratedData() {}

DataCollectorModule::~DataCollectorModule() {}

void DataCollectorModule::setUp() {}

void DataCollectorModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataCollectorModule::body(){
    DataGenerator DataGenerator;    

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        // Gera o dado de acordo com o id do sensor
        if (mSensor_id == 0) { // termometro
            mGeneratedData = DataGenerator.generateDataByNormalDist(747.52, 102.4);
        }
        else if (mSensor_id == 1) { // ecg
            mGeneratedData = DataGenerator.generateDataByNormalDist(409.6, 102.4);
        }
        else { // oximetro
            mGeneratedData = DataGenerator.generateDataByNormalDist(972.8, 52.2);
        }

        // Atribui dados à estrutura de dados especifica
        RawData rawdata(mGeneratedData);

        // Encapsula dado
        Container container(rawdata);

        // Envia pacote
        getConference().send(container);

        std::cout << "Dado " << mGeneratedData << " gerado e enviado pelo sensor: " << mSensor_id << std::endl;
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
