#ifndef SENSORCONFIGURATION_HPP
#define SENSORCONFIGURATION_HPP

#include <string>
#include <math.h>
#include <iostream>

#include "bsn/range/Range.hpp"

class sensor_configuration {
	public:
		int id;
		bsn::range::Range low, medium, high;

		sensor_configuration();

		sensor_configuration(int id, bsn::range::Range b, bsn::range::Range m, bsn::range::Range h);

		double evaluate_number(double number) ;

		void print();
};

#endif