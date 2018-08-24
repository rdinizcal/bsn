#include "../include/ConverterModule.hpp"

using namespace odcore::base;
using namespace odcore::base::module;
using namespace bsn::data;
using namespace bsn::scales;

ConverterModule::ConverterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ConverterModule"),
    mSensor_id(getIdentifier()),
    rawdata_buffer() {}

ConverterModule::~ConverterModule() {}

void ConverterModule::setUp() {
    addDataStoreFor(874, rawdata_buffer);
}

void ConverterModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ConverterModule::body(){    
    ScaleConverter converter;
    double data, converted_data;
    converter.setLowerBound(0);
    if (mSensor_id == 0) {
        converter.setUpperBound(50);
    }
    else if (mSensor_id == 1) {
        converter.setUpperBound(200);
    }
    else if (mSensor_id == 2) {
        converter.setUpperBound(100);
    }

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        while(!rawdata_buffer.isEmpty()){            
            Container container = rawdata_buffer.leave();
            // Desencapsula
            data = container.getData<RawData>().getSensorData();

            // Converte
            converted_data = converter.to_MeasureUnit(data);
            std::cout << "Dado recebido pelo sensor" << mSensor_id << ": " << data << " convertido para " << converted_data << std::endl;                    

            // Encapsula
            ConvertedData encapsulated_data(converted_data);            
            Container packet(encapsulated_data);

            //Envia pacote
            getConference().send(packet);
        }       
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
