#include "DataTriggeredReceiver.hpp"


using namespace std;

// We add some of OpenDaVINCI's namespaces for the sake of readability.
using namespace odcore::base::module;
using namespace odcore::data;
using namespace bsn::data;

vector<sensor_configuration> sensors;

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

DataTriggeredReceiver::DataTriggeredReceiver(const int32_t &argc, char **argv) :
    DataTriggeredConferenceClientModule(argc, argv, "DataTriggeredReceiver")
{}

DataTriggeredReceiver::~DataTriggeredReceiver() {}

void DataTriggeredReceiver::setUp() {
    sensors = load_file("../../configs/sensor_configurations.txt");
}

void DataTriggeredReceiver::tearDown() {}

void DataTriggeredReceiver::nextContainer(Container &c) {    
	if (c.getDataType() == 873){ 
		string packet_raw = c.getData<SensorData>().getSensorStatus();
		
		int sensor_id = stoi(packet_raw.substr(0,packet_raw.find('-')));				 
		double packet = stod(packet_raw.substr(packet_raw.find('-') + 1));

		cout << "Pacote processado: " << packet << '-' << sensor_id << '(' <<
			sensors[sensor_id].evaluate_number(packet) << ')' << endl;

	}
	else 
		usleep(100);
	
}