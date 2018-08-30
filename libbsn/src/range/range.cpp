#include "bsn/range/range.hpp"

using namespace std;

range::range(){ }

range::range(double l, double u) {
	if(l > u) {
		throw std::invalid_argument("Invalid range(lower bound is higher than uppper bound)");
	}

	this->lower_bound = l;
	this->upper_bound = u;			
}

// Verifica se um elemento esta dentro do intervalo estipulado
bool range::in_range(double element){
	if(element >= lower_bound && element <= upper_bound)
		return true;
	else
		return false;        
}

string range::to_print() {
	return '(' + to_string(lower_bound) + " - " + to_string(upper_bound) + ')';			
}