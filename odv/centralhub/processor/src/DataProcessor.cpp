#include "../include/DataProcessor.hpp"

// ecg termomther and oximeter
#define number_sensors 4

using namespace bsn::data;
using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;
using namespace bsn::operation;
using namespace bsn::range;

DataProcessor::DataProcessor(const int32_t &argc, char **argv) :
	packets_received(number_sensors),
    TimeTriggeredConferenceClientModule(argc, argv, "DataProcessor"),
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

// Retorna true se as tres listas são não vazias
bool DataProcessor::available_to_process(){
	bool available = true;

	for(auto packet_list : packets_received){
		if(packet_list.empty()){
			available = false;
			break;
		}
	}

	return available;
}

void DataProcessor::data_fuse() {	
	double average, risk_status;
	int count = 0;
	average = 0;

	// Se não existiver disponível não processa
	if(!available_to_process())
		return;
	
	for(auto &packet_list : packets_received){
		if(!packet_list.empty()) {
			// Soma à média e retira da fila
			average += packet_list.front();
			
			// Descarta o pacote processado se existem
			// Mais outros para serem processados
			if(packet_list.size() > 1) {
				packet_list.pop_front();
			}			
			count++;
		}
	}	
	// Calcula a media partir da soma dividida pelo número de pacotes lidos
	risk_status = 100.0 * (average / count);
	// Status de risco do paciente dado em porcentagem

	// 85.0 é um número totalmente ARBITRARIO
	if(risk_status > 85.0){
		cout << "============EMERGÊNCIA============(" << risk_status << ")" << endl;
	}
	else{
		cout << "General risk status: " << risk_status << '%' << endl;
	}
}
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

// Retorna o id baseado no tipo
int DataProcessor::get_sensor_id(std::string type) {
	if (type == "thermometer")
		return 0;
	else if (type == "ecg")
		return 1;
	else if (type == "oximeter")
		return 2;
	else if (type == "bpms")
		return 3;
	else if (type == "bpmd")		
		return 4;
	else {
		cout << "UNKNOWN TYPE";
		return -1;
	}

}

double DataProcessor::get_value(string packet){
	double ret = stod(packet.substr(packet.find('-')+1,packet.length()));
	return ret;
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

                data_fuse();
            }

			/*
             * Para cada execução do loop, contabilizar e enviar duas unidades de bateria consumida
             * */
            ResourceUpdate rUpdate(1);
            Container rUpdContainer(rUpdate);
            getConference().send(rUpdContainer);

        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
