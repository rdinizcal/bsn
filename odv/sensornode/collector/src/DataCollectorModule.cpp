#include "../include/DataCollectorModule.hpp"

using namespace odcore::base::module;
using namespace bsn::data;
using namespace bsn::generator;
using namespace odcore::data;
using namespace bsn::time;

DataCollectorModule::DataCollectorModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "DataCollectorModule"),
    mGeneratedData(), 
    timeRef() {}

DataCollectorModule::~DataCollectorModule() {}

void DataCollectorModule::setUp() {}

void DataCollectorModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataCollectorModule::body(){
    DataGenerator dataGenerator;
    std::string sensorType = getKeyValueConfiguration().getValue<std::string>("datacollectormodule.type");
    timespec ts;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        clock_gettime(CLOCK_REALTIME, &ts);

        // Gera o dado de acordo com o id do sensor
        if (sensorType == "thermometer") { // termometro
            mGeneratedData = dataGenerator.generateDataByNormalDist(747.52, 102.4);
            ThermometerRawData rawdata(mGeneratedData, ts);
            Container container(rawdata);
            getConference().send(container);
        }
        else if (sensorType == "ecg") { // ecg
            mGeneratedData = dataGenerator.generateDataByNormalDist(409.6, 102.4);
            ECGRawData rawdata(mGeneratedData, ts);
            Container container(rawdata);
            getConference().send(container);
        }
        else if (sensorType == "oximeter") { // oximetro
            mGeneratedData = dataGenerator.generateDataByNormalDist(972.8, 52.2);
            OximeterRawData rawdata(mGeneratedData, ts);
            Container container(rawdata);
            getConference().send(container);
        }
        else if (sensorType == "bpms") { // monitor de pressao sistolica
            mGeneratedData = dataGenerator.generateDataByNormalDist(409.6, 34.13);
            SystolicRawData rawdata(mGeneratedData, ts);
            Container container(rawdata);
            getConference().send(container);
        }
        else if (sensorType == "bpmd") { // monitor de pressao diastolica
            mGeneratedData = dataGenerator.generateDataByNormalDist(327.68, 34.13);
            DiastolicRawData rawdata(mGeneratedData, ts);
            Container container(rawdata);
            getConference().send(container);
        }

        std::cout << "Dado " << mGeneratedData << " gerado e enviado pelo sensor: " << sensorType << std::endl;
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
