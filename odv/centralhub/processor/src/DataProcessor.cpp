#include "../include/DataProcessor.hpp"

// ecg termomther oximeter and blood pressure
#define number_sensors 5

using namespace bsn::data;
using namespace bsn::range;
using namespace bsn::operation;
using namespace bsn::processor;
using namespace odcore::data;
using namespace odcore::base;
using namespace odcore::base::module;
using namespace bsn::configuration;

DataProcessor::DataProcessor(const int32_t &argc, char **argv) :
TimeTriggeredConferenceClientModule(argc, argv, "DataProcessor"),
	packets_received(number_sensors),
	configurations(0),
	data_buffer() {}
	
DataProcessor::~DataProcessor() {}

void DataProcessor::setUp() {
    addDataStoreFor(873, data_buffer);
}

void DataProcessor::tearDown(){}

void DataProcessor::print_packs(){
	int i = 0;
	for(auto l : packets_received){
		cout << i << ": ";
		i++;
		for(auto el : l){
			cout << el << ' ';
		}
		cout << endl;
	}
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataProcessor::body(){
    Container container;    
    array<string, 2> types;
    array<double, 4> data;
    array<string, 8> times;
    int sensor_id;
    double evaluated_packet;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING){

        while(!data_buffer.isEmpty()){
            // Retira o dado da FIFO
            container = data_buffer.leave();
            types = container.getData<SensorData>().getSensorType();
            data = container.getData<SensorData>().getSensorData();
            times = container.getData<SensorData>().getTimes();

            sensor_id = get_sensor_id(types[0]);

            if (types[0] == "bpms" or types[0] == "bpmd") {
         	// O mais discrepante é o que conta(Guideline brasileiro)
         		evaluated_packet = max(data[1],data[3]);
                if (evaluated_packet == data[3])
                    sensor_id = get_sensor_id(types[1]); 
            }
            else {
         	// Para os sensores que não são de pressão
        		evaluated_packet = data[1];
            }

            // Se o pacote for válido...
            if (evaluated_packet != -1) {                
		        packets_received[sensor_id].push_back(evaluated_packet);
		        print_packs();

                data_fuse(packets_received);
            }

			/*
             * Para cada execução do loop, contabilizar e enviar duas unidades de bateria consumida
             */
            ResourceUpdate rUpdate(-1);
            Container rUpdContainer(rUpdate);
            getConference().send(rUpdContainer);

        }

    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
