#include "bsn/configuration/sensor_configuration.hpp"

using namespace std; 

sensor_configuration::sensor_configuration(){}

sensor_configuration::sensor_configuration(int id, range b, range m, range h){
	this->id = id;
	this->low = b;
	this->medium = m;
	this->high = h;
}

string sensor_configuration::evaluate_number(double number) {				
	if(high.in_range(number))
		return "high";
	else if(medium.in_range(number))
		return "medium";
	else if(low.in_range(number))
		return "low";
	else
		return "unknow";
}

void sensor_configuration::print(){
	cout << "Sensor id(" << id << ") with ranges: " << endl;
	cout << "Low: "  << low.to_print() << endl;
	cout << "med: "  << medium.to_print() << endl;
	cout << "high:"  << high.to_print() << endl;
	cout << endl;
}