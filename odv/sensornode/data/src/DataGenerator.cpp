#include "../include/DataGenerator.hpp"

DataGenerator::DataGenerator(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "data"),
    m_id(getIdentifier()),
    m_sensor_type() {}

DataGenerator::~DataGenerator() {}

void DataGenerator::setUp() {
    m_sensor_type = m_id+1;
}

void DataGenerator::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataGenerator::body(){
    // aqui será gerado o dado pela normal e enviado por container
}
