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
            mGeneratedData = dataGenerator.generateDataByNormalDist(36.5, 4.0);
        }
        else if (sensorType == "ecg") { // ecg
            mGeneratedData = dataGenerator.generateDataByNormalDist(80.0, 30.0);
        }
        else if (sensorType == "oximeter") { // oximetro
            mGeneratedData = dataGenerator.generateDataByNormalDist(95, 5);
        }
        else if (sensorType == "bpms") { // monitor de pressao sistolica
            mGeneratedData = dataGenerator.generateDataByNormalDist(120.0, 50.0);
        }
        else if (sensorType == "bpmd") { // monitor de pressao diastolica
            mGeneratedData = dataGenerator.generateDataByNormalDist(80.0, 50.0);
        }

        RawData rawdata(mGeneratedData, sensorType, ts);
        Container container(rawdata);
        getConference().send(container);

        std::cout << "Dado " << mGeneratedData << " gerado e enviado pelo sensor: " << sensorType << std::endl;
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
