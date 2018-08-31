#ifndef SENSORCONFIGURATION_HPP
#define SENSORCONFIGURATION_HPP

#include <string>
#include <math.h>
#include <iostream>

#include "bsn/range/range.hpp"

class sensor_configuration {
	public:
		int id;
		range low,medium,high;

		sensor_configuration();

		sensor_configuration(int id, range b, range m, range h);

		double evaluate_number(double number) ;

		void print();
};

#endif