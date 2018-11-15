#include "bsn/configuration/SensorConfiguration.hpp"

using namespace std;
using namespace bsn::range;

namespace bsn {
	namespace configuration {

		SensorConfiguration::SensorConfiguration(){}

		SensorConfiguration::SensorConfiguration(int32_t i, Range l, 
			array<Range, 2> m, array<Range, 2> h, array<Range, 3> p ){
				
			this->id 		 	  = i;		
			this->lowRisk 	 	  = l;
			this->mediumRisk	  = m;
			this->highRisk  	  = h;
			this->lowPercentage  = p[0];
			this->midPercentage  = p[1];
			this->highPercentage = p[2];
		}

		int32_t SensorConfiguration::getId() {
			return this->id;
		}

		// Retorna o quão deslocado do meio um valor está
		double SensorConfiguration::getDisplacement(Range range, double number, string logic = "crescent") {
			// Crescent indica o quão deslocado um elemento está do começo
			// inverse indica o quão deslocado um elemento está do fim
			// medium indica o quão deslocado um elemento está do meio
			double lb = range.getLowerBound();
			double ub = range.getUpperBound();
			double result;

			if(logic == "decrescent") {
				result = fabs(number - ub);
				result /= (ub - lb);
			}
			else if(logic == "crescent") {
				result = number - lb;
				result /= (ub - lb);
			}
			else if(logic == "medium") {				
				double mediumValue = (ub + lb)/2.0;				
				result = fabs(number - mediumValue);
				result /= (ub - mediumValue);
						
			}
			else { 
				throw std::invalid_argument("Invalid operation given to getDisplacement");
				result = -1;
			}

			return result;
		}

		double SensorConfiguration::convertRealPercentage(Range newRange, double number) {
			Range oldRange(0.0, 1.0);
			double lb = newRange.getLowerBound();
			double ub = newRange.getUpperBound();
			
			return oldRange.convert(lb,ub,number);
		}

		double SensorConfiguration::evaluateNumber(double number) {
			double displacement;
			Range per_cent(0,100);

			if(lowRisk.in_range(number)) {				
				displacement = getDisplacement(lowRisk, number, "medium");
				return convertRealPercentage(lowPercentage, displacement);
			}	

			else if(mediumRisk[0].in_range(number)) {
				displacement = getDisplacement(mediumRisk[0], number, "decrescent");	
				return convertRealPercentage(midPercentage, displacement);	
			}

			else if(mediumRisk[1].in_range(number)) {
				displacement = getDisplacement(mediumRisk[1], number);
				return convertRealPercentage(midPercentage, displacement);	
			}

			else if(highRisk[0].in_range(number)) {
				displacement = getDisplacement(highRisk[0], number, "decrescent");
				return convertRealPercentage(highPercentage, displacement);			
			}

			else if(highRisk[1].in_range(number)) {
				displacement = getDisplacement(highRisk[1], number);		
				return convertRealPercentage(highPercentage, displacement);	
			}
			else {
				return -1;
			}	
		}

		void SensorConfiguration::print(){
			cout << "Sensor id(" << id << ") with ranges: " << endl;
			cout << "Low: "    << lowRisk.to_print() << endl;
			cout << "Medium: " << mediumRisk[0].to_print() << mediumRisk[1].to_print() << endl;
			cout << "High: "   << highRisk[0].to_print() << highRisk[1].to_print() << endl;
			
			cout << endl;
		}
		
	}
}
