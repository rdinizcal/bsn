#include "bsn/range/Range.hpp"

using namespace std;

namespace bsn {
	namespace range {

		Range::Range() : lower_bound(), upper_bound() {}

		Range::Range(const double &l, const double &u) {
			if(l > u) {
				throw std::invalid_argument("Invalid Range(lower bound is higher than uppper bound)");
			}

			this->lower_bound = l;
			this->upper_bound = u;			
		}

		Range::~Range() {}

		// Verifica se um elemento esta dentro do intervalo estipulado
		bool Range::in_range(double element) {
			return (element >= lower_bound && element <= upper_bound)? true:false;      
		} 

		double Range::convert(double new_lb, double new_ub, double input) {
			double result = ((input - lower_bound) / (upper_bound - lower_bound)) *
				(new_ub - new_lb) + new_lb;
				
			return result;				
		}

		string Range::to_print() {
			return '(' + to_string(lower_bound) + " - " + to_string(upper_bound) + ')';			
		}

		void Range::setLowerBound (const double &_lower_bound) {
			lower_bound = _lower_bound;
		}

		double Range::getLowerBound () const {
			return lower_bound;
		}

		void Range::setUpperBound (const double &_upper_bound) {
			upper_bound = _upper_bound;
		}

		double Range::getUpperBound () const {
			return upper_bound;
		}

	}
}