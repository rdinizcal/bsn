#ifndef RANGE_HPP
#define RANGE_HPP

#include <string>
#include <stdexcept>
#include <iostream>
#include <stdint.h>

namespace bsn {
	namespace range {

		class Range {	
					
			public:
				Range();
				Range(const double &/*lower bound*/, const double &/*upper bound*/);
				~Range();
 
				// Verifica se um elemento esta dentro do intervalo estipulado
				bool in_range(double element);
				double convert(double new_lb, double new_ub, double input);
				std::string to_print();

				void setLowerBound (const double &/*_lower_bound*/);
				double getLowerBound () const;

				void setUpperBound (const double &/*_upper_bound*/);
				double getUpperBound () const;

			private:
				double lower_bound;
				double upper_bound;

		};

	}
}

#endif 