#include "bsn/configuration/SensorConfiguration.hpp"

using namespace std;
using namespace bsn::range;

namespace bsn {
	namespace configuration {

		SensorConfiguration::SensorConfiguration(){}

		SensorConfiguration::SensorConfiguration(int32_t i, Range l, 
			array<Range, 2> m, array<Range, 2> h, array<Range, 3> p ){
				
			this->id 		 	  = i;		
			this->low_risk 	 	  = l;
			this->medium_risk	  = m;
			this->high_risk  	  = h;
			this->low_percentage  = p[0];
			this->mid_percentage  = p[1];
			this->high_percentage = p[2];
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
				double medium_value = (ub + lb)/2.0;				
				result = fabs(number - medium_value);
				result /= (ub - medium_value);
						
			}
			else { 
				throw std::invalid_argument("Invalid operation given to getDisplacement");
				result = -1;
			}

			return result;
		}

		double SensorConfiguration::convertRealPercentage(Range new_range, double number) {
			Range old_range(0.0, 1.0);
			double lb = new_range.getLowerBound();
			double ub = new_range.getUpperBound();
			
			return old_range.convert(lb,ub,number);
		}

		double SensorConfiguration::evaluateNumber(double number) {
			double displacement;
			Range per_cent(0,100);

			if(low_risk.in_range(number)) {				
				displacement = getDisplacement(low_risk, number, "medium");
				return convertRealPercentage(low_percentage, displacement);
			}	

			else if(medium_risk[0].in_range(number)) {
				displacement = getDisplacement(medium_risk[0], number, "decrescent");	
				return convertRealPercentage(mid_percentage, displacement);	
			}

			else if(medium_risk[1].in_range(number)) {
				displacement = getDisplacement(medium_risk[1], number);
				return convertRealPercentage(mid_percentage, displacement);	
			}

			else if(high_risk[0].in_range(number)) {
				displacement = getDisplacement(high_risk[0], number, "decrescent");
				return convertRealPercentage(high_percentage, displacement);			
			}

			else if(high_risk[1].in_range(number)) {
				displacement = getDisplacement(high_risk[1], number);		
				return convertRealPercentage(high_percentage, displacement);	
			}
			else {
				return -1;
			}	
		}

		void SensorConfiguration::print(){
			cout << "Sensor id(" << id << ") with ranges: " << endl;
			cout << "Low: "    << low_risk.to_print() << endl;
			cout << "Medium: " << medium_risk[0].to_print() << medium_risk[1].to_print() << endl;
			cout << "High: "   << high_risk[0].to_print() << high_risk[1].to_print() << endl;
			
			cout << endl;
		}
		
	}
}
