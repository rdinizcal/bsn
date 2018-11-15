#include "bsn/range/Range.hpp"

using namespace std;

namespace bsn {
	namespace range {

		Range::Range() : lowerBound(), upperBound() {}

		Range::Range(const double &l, const double &u) {
			if(l > u) {
				throw std::invalid_argument("Invalid Range(lower bound is higher than uppper bound)");
			}

			this->lowerBound = l;
			this->upperBound = u;			
		}

		Range::~Range() {}

		// Verifica se um elemento esta dentro do intervalo estipulado
		bool Range::in_range(double element) {
			return (element >= lowerBound && element <= upperBound)? true:false;      
		} 

		double Range::convert(double new_lb, double new_ub, double input) {
			double result = ((input - lowerBound) / (upperBound - lowerBound)) *
				(new_ub - new_lb) + new_lb;
				
			return result;				
		}

		string Range::to_print() {
			return '(' + to_string(lowerBound) + " - " + to_string(upperBound) + ')';			
		}

		void Range::setLowerBound (const double &_lowerBound) {
			lowerBound = _lowerBound;
		}

		double Range::getLowerBound () const {
			return lowerBound;
		}

		void Range::setUpperBound (const double &_upperBound) {
			upperBound = _upperBound;
		}

		double Range::getUpperBound () const {
			return upperBound;
		}

	}
}