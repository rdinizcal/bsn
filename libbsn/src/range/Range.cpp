#include "bsn/range/Range.hpp"
#include <iostream>

using namespace std;

namespace bsn {
	namespace range {

		Range::Range(){ }

		Range::Range(double l, double u) {
			if(l > u) {
				throw std::invalid_argument("Invalid Range(lower bound is higher than uppper bound)");
			}

			this->lower_bound = l;
			this->upper_bound = u;			
		}

		// Verifica se um elemento esta dentro do intervalo estipulado
		bool Range::in_range(double element) {
			if(element >= lower_bound && element <= upper_bound)
				return true;
			else
				return false;        
		}

		double Range::convert(double new_lb, double new_ub, double input) {
			double result = ((input - lower_bound) / (upper_bound - lower_bound)) *
				(new_ub - new_lb) + new_lb;
		}

		string Range::to_print() {
			return '(' + to_string(lower_bound) + " - " + to_string(upper_bound) + ')';			
		}

	}
}