#include "../include/DataProcessor.hpp"

// ecg termomther and oximeter
#define number_sensors 3

using namespace bsn::data;
using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;

const vector<string> split(const string& s, const char& c) {    
	string buff{""};
	vector<string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

DataProcessor::DataProcessor(const int32_t &argc, char **argv) :
	packets_received(3),
    TimeTriggeredConferenceClientModule(argc, argv, "DataProcessor"),
    data_buffer() {}

DataProcessor::~DataProcessor() {}

void DataProcessor::setUp() {
	vector<string> sensorTypes = {"thermometer", "ecg", "oximeter"};
	vector<string> low, mid, high;
	vector<range> ranges;
	
	for (string str : sensorTypes) {		
		low = split(getKeyValueConfiguration().getValue<string>("dataprocessor." + str + "LowRange"), ',');
		mid = split(getKeyValueConfiguration().getValue<string>("dataprocessor." + str + "MidRange"), ',');
		high = split(getKeyValueConfiguration().getValue<string>("dataprocessor." + str + "HighRange"), ',');

		range lowRange(stod(low[0]), stod(low[1]));
		range midRange(stod(mid[0]), stod(mid[1]));
		range highRange(stod(high[0]), stod(high[1]));

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

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataProcessor::body(){
    Container container;    
    int sensor_id;
	string type, packet_raw;
	double packet;
	double evaluated_packet;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while(!data_buffer.isEmpty()){
            // retira o dado da FIFO
            container = data_buffer.leave();
            packet_raw = container.getData<SensorData>().getSensorStatus();
            
			type = packet_raw.substr(0,packet_raw.find('-'));				 
            packet = stod(packet_raw.substr(packet_raw.find('-') + 1));
            evaluated_packet = configurations[sensor_id].evaluate_number(packet);

			if (type == "thermometer")
				sensor_id = 0;
			else if (type == "ecg")
				sensor_id = 1;
			else if (type == "oximeter")
				sensor_id = 2;

            // Se o pacote for válido...
            if(evaluated_packet != -1){                
				packets_received[sensor_id].push_back(evaluated_packet);
				print_packs();

                data_fuse();
            }

        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
