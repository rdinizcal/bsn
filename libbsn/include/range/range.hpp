#ifndef RANGE_HPP
#define RANGE_HPP

#include <string>
#include <stdexcept>

class range {			
	public:
		double lower_bound, upper_bound;
		range();
		range(double l, double u);
		// Verifica se um elemento esta dentro do intervalo estipulado
		bool in_range(double element);
		std::string to_print();
};
#endif 