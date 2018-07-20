#include "../include/ConverterModule.hpp"


ConverterModule::ConverterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ConverterModule"),
    rawdata_buffer() {}

ConverterModule::~ConverterModule() {}

void ConverterModule::setUp() {
    addDataStoreFor(874, rawdata_buffer);
}

void ConverterModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ConverterModule::body(){    
    ScaleConverter converter(0,50);
    double data, converted_data;    

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        while(!rawdata_buffer.isEmpty()){            
            Container container = rawdata_buffer.leave();
            // Desencapsula
            data = container.getData<RawData>().getSensorData();
            // Converte
            converted_data = converter.to_celsius(data);
            std::cout << "Dado recebido: " << data << " convertido para " << converted_data << std::endl;                    

            // Encapsula
            ConvertedData encapsulated_data(converted_data);            
            Container packet(encapsulated_data);

            //Envia pacote
            getConference().send(packet);
        }       
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
