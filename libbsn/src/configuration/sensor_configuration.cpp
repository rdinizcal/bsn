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
	double conversion, result;
	if(low.in_range(number)) {		
		conversion = low.convert(1.0,3.9,number);
		// Calcula a distância entre a conversão e 5(numero médio) em por cento
		result = (fabs(5.0 - conversion)) / 5.0;
	}		
	else if(medium.in_range(number)) {		
		conversion = medium.convert(4.0,6.9,number);
		result = (fabs(5.0 - conversion)) / 5.0;
	}		
	else if(high.in_range(number)) {		
		conversion = high.convert(7.0,9.9,number);
		result = (fabs(5.0 - conversion)) / 5.0;
	}		
	else {
		// Representação do unknow
		cout << "Unknown value(" << number << ") came from sensor and will not be processed\n";
		result = -1;
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