#include "bsn/configuration/sensor_configuration.hpp"

using namespace std; 

sensor_configuration::sensor_configuration(){}

sensor_configuration::sensor_configuration(int id, range b, range m, range h){
	this->id = id;
	this->low = b;
	this->medium = m;
	this->high = h;
}

double sensor_configuration::evaluate_number(double number) {
	// Primeiro converte para uma segunda escala(0-3, 3-6, 6-9) 
	// Para tratar intervalos com tamanhos originalmente diferentes
	// Da mesma forma.
	// Depois calcula-se a distância do centro(4.5) para saber o grau de risco 
	double conversion, result;
	double medium_value = 4.5;
	if(low.in_range(number)) {
		conversion = low.convert(0.0,3.0,number);
		// Calcula a distância entre a conversão e o num medio em por cento
		result = (fabs(medium_value - conversion)) / medium_value;
	}		
	else if(medium.in_range(number)) {		
		conversion = medium.convert(3.0,6.0,number);
		result = (fabs(medium_value - conversion)) / medium_value;
	}		
	else if(high.in_range(number)) {				
		conversion = high.convert(6.0,9.0,number);
		result = (fabs(medium_value - conversion)) / medium_value;
	}		
	else {
		// Representação do unknow
		cout << "Unknown value(" << number << ") came from sensor and will not be processed\n";
		result = -1.0;
	}

	return result;
		
}

void sensor_configuration::print(){
	cout << "Sensor id(" << id << ") with ranges: " << endl;
	cout << "Low: "  << low.to_print() << endl;
	cout << "med: "  << medium.to_print() << endl;
	cout << "high:"  << high.to_print() << endl;
	cout << endl;
}