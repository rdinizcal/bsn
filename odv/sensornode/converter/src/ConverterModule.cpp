#include "../include/ConverterModule.hpp"

using namespace odcore::base;
using namespace odcore::base::module;
using namespace bsn::data;
using namespace bsn::scales;
using namespace odcore::data;
using namespace bsn::time;

ConverterModule::ConverterModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "ConverterModule"),
    rawdata_buffer() {}

ConverterModule::~ConverterModule() {}

void ConverterModule::setUp() {
    sensorType = getKeyValueConfiguration().getValue<std::string>("convertermodule.type");
    converter.setLowerBound(0);
    if (sensorType == "thermometer") {
        converter.setUpperBound(50);
        addDataStoreFor(881, rawdata_buffer);
    }
    else if (sensorType == "ecg") {
        converter.setUpperBound(200);
        addDataStoreFor(882, rawdata_buffer);
    }
    else if (sensorType == "oximeter") {
        converter.setUpperBound(100);
        addDataStoreFor(883, rawdata_buffer);
    }
    else if (sensorType == "bpms") {
        converter.setUpperBound(30);
        addDataStoreFor(884, rawdata_buffer);
    }
    else if (sensorType == "bpmd") {
        converter.setUpperBound(25);
        addDataStoreFor(885, rawdata_buffer);
    }
}

void ConverterModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ConverterModule::body(){    
    double data, converted_data;
    array<timespec, 2> ts_v;
    timespec now_time, back_time;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        while(!rawdata_buffer.isEmpty()){
            // ts_v.clear();
            clock_gettime(CLOCK_REALTIME, &now_time);
            
            Container container = rawdata_buffer.leave();
          
            // Desencapsula
            data = container.getData<RawData>().getSensorData();
            back_time = container.getData<RawData>().getTimespec();

            // Converte
            converted_data = converter.to_MeasureUnit(data);
            std::cout << "Dado recebido pelo " << sensorType << ": " << data << " convertido para " << converted_data << std::endl;

            ts_v[0] = back_time;
            ts_v[1] = now_time;

            // Encapsula
            ConvertedData encapsulated_data(converted_data, sensorType, ts_v);
            
            Container packet(encapsulated_data);

            //Envia pacote
            getConference().send(packet);
        }       
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
