#ifndef SENSORCONFIGURATION_HPP
#define SENSORCONFIGURATION_HPP

#include <string>
#include <math.h>
#include <iostream>
#include <array>

#include "bsn/range/Range.hpp"

namespace bsn {
	namespace configuration {

		class SensorConfiguration {
				// id é o id do sensor
				// {var}_percentage represanta o quanto cada intervalo deve valer
				// {var}_risk são os intervalos de cada risco(baixo medio ou alto)
				private:
				
				int32_t id;
				bsn::range::Range lowPercentage, midPercentage, highPercentage;
				bsn::range::Range lowRisk;	
				std::array<bsn::range::Range, 2> mediumRisk;
				std::array<bsn::range::Range, 2> highRisk;

				public:
				SensorConfiguration();

				SensorConfiguration(int32_t i, bsn::range::Range lowRisk,
									std::array<bsn::range::Range, 2> mediumRisk,
									std::array<bsn::range::Range, 2> highRisk,
									std::array<bsn::range::Range, 3> percentages);

				// Retorna o estado de risco a partir dos intervalos
				double evaluateNumber(double number);
				// Retorna o quão deslocado do meio um valor está
				double getDisplacement(bsn::range::Range, double, std::string );
				// Converte para o percentual final( {var}_percentage )
				double convertRealPercentage(bsn::range::Range range, double number);
				int32_t getId();
				void print();
		};


	}
}

#endif