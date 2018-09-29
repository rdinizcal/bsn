#ifndef RANGE_HPP
#define RANGE_HPP

#include <string>
#include <stdexcept>

namespace bsn {
	namespace range {

		class Range {			
			public:
				double lower_bound, upper_bound;
				Range();
				Range(double l, double u);
				// Verifica se um elemento esta dentro do intervalo estipulado
				bool in_range(double element);
				double convert(double new_lb, double new_ub, double input);
				std::string to_print();
		};

	}
}

#endif 