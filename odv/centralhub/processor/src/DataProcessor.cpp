#include "../include/DataProcessor.hpp"
#define maximum_list_size 6

using namespace bsn::data;
using namespace odcore::base;
using namespace odcore::base::module;

//FIXME: remover essa lista
std::list<double> packets_list;
std::vector<sensor_configuration> sensor_configs;


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

// Recebe uma linha com os intervalos e retorna uma sensor_configuration
sensor_configuration adapt_line(string line){
	// Contem as tres intervalos 
	vector<range> ranges_vet;
	// Pega id
	int this_id = stoi(line.substr(0,line.find('=')));
	// Pega tudo a partir do id		
	line = line.substr(line.find('=') + 1);
	auto substr = split(line,';');
	
	//loop de 3 pq são tres intervalos: bom medio e ruim
	for (int i=0 ; i < 3 ; i++){
		// String vet contem os dois intervalos(menor em 0 e maior em 1)		
		auto string_vet = split(substr[i],',');
		double lb = stod(string_vet[0]);
		double ub = stod(string_vet[1]);
		range this_range(lb,ub);
		ranges_vet.push_back(this_range);
	}
	// Retorna a classe feita com os intervalos
	sensor_configuration result(this_id, ranges_vet[0], ranges_vet[1], ranges_vet[2]);

	return result;
}

vector<sensor_configuration> load_file(string path){
	string line;
	vector<sensor_configuration> result;
	ifstream myfile (path);	
	
	if (myfile.is_open()) {		
		while ( getline (myfile,line) ) {			
			auto c = adapt_line(line);
			result.push_back(c);
		}
		myfile.close();		
	}
	else 
		cout << "Unable to find configurations file"; 
	
	return result;
}

DataProcessor::DataProcessor(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "DataProcessor"),
    data_buffer() {}

DataProcessor::~DataProcessor() {}

void DataProcessor::setUp() {
    sensor_configs = load_file("../../configs/sensor_configurations.txt");
    addDataStoreFor(873, data_buffer);
}

void DataProcessor::tearDown(){}


void data_fuse() {	
	// Inicialmente considerando uma media simples dos elementos
    // TODO: RENAMING sensors -> configurations
    // TODO: Criar o vector de lista
	// TODO: implementar média com pesos futuramente
	// TODO: tirar a média dos três sinais ao invés de todos os recebidos
	// Para tanto criar 3 listas, um para cada sinal(ou um vector de listas)
	// E tirar a média dos primeiros sinais de cada uma(retirando o define max...)
	// TODO: Colocar arquivo de configuração do OpendaVinci	    
	double average, risk_status;	
	average = 0;

	// Se tiver mais que o maximo definido, elimina os elementos
	while(packets_list.size() >= maximum_list_size){		
		packets_list.pop_front();
	}

	for(auto element : packets_list){		
		average += element;
	}	
	// Status de risco do paciente em porcentagem
	risk_status = 100.0 * (average / packets_list.size());
	// 85.0 é um número totalmente ARBITRARIO
	if(risk_status > 85.0){
		cout << "============EMERGÊNCIA============(" << risk_status << ")" << endl;
	}
	else{
		cout << "General risk status: " << risk_status << '%' << endl;
	}
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode DataProcessor::body(){
    Container container;    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while(!data_buffer.isEmpty()){
            // retira o dado da FIFO
            container = data_buffer.leave();
            string packet_raw = container.getData<SensorData>().getSensorStatus();
            
            int sensor_id = stoi(packet_raw.substr(0,packet_raw.find('-')));				 
            double packet = stod(packet_raw.substr(packet_raw.find('-') + 1));
            double evaluated_packet = sensor_configs[sensor_id].evaluate_number(packet);

            // Se o pacote for válido...
            if(evaluated_packet != -1){
                packets_list.push_back(evaluated_packet);

                data_fuse();
            }

        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
