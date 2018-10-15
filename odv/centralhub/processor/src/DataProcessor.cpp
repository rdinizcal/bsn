#include "../include/DataProcessor.hpp"

// ecg termomther and oximeter
#define number_sensors 4

using namespace bsn::data;
using namespace bsn::range;
using namespace bsn::operation;
using namespace bsn::processor;
using namespace odcore::data;
using namespace odcore::base;
using namespace odcore::base::module;


DataProcessor::DataProcessor(const int32_t &argc, char **argv) :
TimeTriggeredConferenceClientModule(argc, argv, "DataProcessor"),
	packets_received(number_sensors),
	configurations(0),
	data_buffer() {}
	
DataProcessor::~DataProcessor() {}

void DataProcessor::setUp() {
	vector<string> sensorTypes = {"thermometer", "ecg", "oximeter", "bpms", "bpmd"};
	vector<string> low, mid, high;
	vector<Range> ranges;
	Operation operation;
	
	for (string str : sensorTypes) {
		low = operation.split(getKeyValueConfiguration().getValue<string>("dataprocessor." + str + "LowRange"), ',');
		mid = operation.split(getKeyValueConfiguration().getValue<string>("dataprocessor." + str + "MidRange"), ',');
		high = operation.split(getKeyValueConfiguration().getValue<string>("dataprocessor." + str + "HighRange"), ',');

		Range lowRange(stod(low[0]), stod(low[1]));
		Range midRange(stod(mid[0]), stod(mid[1]));
		Range highRange(stod(high[0]), stod(high[1]));				

		sensor_configuration aux_config(0 /* aqui tem que ser passado o tipo*/, lowRange, midRange, highRange);
		configurations.push_back(aux_config);
	}

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
    int sensor_id;
	string type, packet_raw;
	double packet;
	double evaluated_packet;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while(!data_buffer.isEmpty()){
            // Retira o dado da FIFO
            container = data_buffer.leave();
            packet_raw = container.getData<SensorData>().getSensorStatus();
            
			// Pega o tipo do pacote
			type = packet_raw.substr(0,packet_raw.find('-'));

			sensor_id = get_sensor_id(type);			

			if(type == "bpms" or type == "bpmd") {
				string diastolyc_packet, systolic_packet;
				double systolic_value, diastolic_value;	
				double eval_sys, eval_dia;			

				systolic_packet  = packet_raw.substr(0,packet_raw.find('/'));
    			diastolyc_packet = packet_raw.substr(packet_raw.find('/')+1,packet_raw.length());

				// Retira os valores a partir do pacote recebido
				systolic_value  = get_value(systolic_packet);
				diastolic_value = get_value(diastolyc_packet);
				
				eval_sys = configurations[sensor_id].evaluate_number(systolic_value);				
				eval_dia = configurations[sensor_id+1].evaluate_number(diastolic_value);
				
				// O mais discrepante é o que conta(Guideline brasileiro)
				evaluated_packet = max(eval_dia,eval_sys);
			}
			else {
				// Para os sensores que não são de pressão
				packet = get_value(packet_raw);
				evaluated_packet = configurations[sensor_id].evaluate_number(packet);
			}

            // Se o pacote for válido...
            if(evaluated_packet != -1){                
				packets_received[sensor_id].push_back(evaluated_packet);
				print_packs();

                data_fuse(packets_received);
            }

			/*
             * Para cada execução do loop, contabilizar e enviar duas unidades de bateria consumida
             * */
            ResourceUpdate rUpdate(-1);
            Container rUpdContainer(rUpdate);
            getConference().send(rUpdContainer);

        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
