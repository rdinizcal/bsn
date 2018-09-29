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


// Retorna os parametros necessários à distribuição normal
std::tuple<double,double> DataCollectorModule::get_normal_params(string sensorType) {
    tuple<double,double> result;
    if (sensorType == "thermometer") { // termometro
        result = std::make_tuple(36.5, 4.0);
    }
    else if (sensorType == "ecg") { // ecg
        result = std::make_tuple(80.0, 30.0);
    }
    else if (sensorType == "oximeter") { // oximetro
        result = std::make_tuple(95, 5);
    }
    else if (sensorType == "bpms") { // monitor de pressao sistolica
        result = std::make_tuple(120.0, 50.0);
    }
    else if (sensorType == "bpmd") { // monitor de pressao diastolica
        result = std::make_tuple(80.0, 50.0);
    }    
    return result;
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataCollectorModule::body(){
    DataGenerator dataGenerator;
    std::string sensorType = getKeyValueConfiguration().getValue<std::string>("datacollectormodule.type");
    timespec ts;
    double mode,variance;

    // Retorna os argumentos necessários
    std::tie(mode,variance) = get_normal_params(sensorType);    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        clock_gettime(CLOCK_REALTIME, &ts);

        mGeneratedData = dataGenerator.generateDataByNormalDist(mode, variance);

        RawData rawdata(mGeneratedData, sensorType, ts);
        Container container(rawdata);
        getConference().send(container);

        std::cout << "Dado " << mGeneratedData << " gerado e enviado pelo sensor: " << sensorType << std::endl;
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
