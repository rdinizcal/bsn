#ifndef SENSORCONFIGURATION_HPP
#define SENSORCONFIGURATION_HPP

#include <string>
#include <math.h>
#include <iostream>
#include <array>

#include "bsn/range/Range.hpp"

class SensorConfiguration {
		// id é o id do sensor
		// {var}_percentage represanta o quanto cada intervalo deve valer
		// {var}_risk são os intervalos de cada risco(baixo medio ou alto)
		private:
		
		int id;
		bsn::range::Range low_percentage, mid_percentage, high_percentage;
		bsn::range::Range low_risk;	
		std::array<bsn::range::Range, 2> medium_risk;
		std::array<bsn::range::Range, 2> high_risk;

		public:
		SensorConfiguration();

		SensorConfiguration(int i, bsn::range::Range low_risk, 
		std::array<bsn::range::Range, 2> medium_risk,
		std::array<bsn::range::Range, 2> high_risk,
		std::array<bsn::range::Range, 3> percentages);

		// Retorna o estado de risco a partir dos intervalos
		double evaluateNumber(double number);
		// Retorna o quão deslocado do meio um valor está
		double getDisplacement(bsn::range::Range, double, bool );
		// Converte para o percentual final( {var}_percentage )
		double convertRealPercentage(bsn::range::Range range, double number);
		int getId();
		void print();
};

#endif