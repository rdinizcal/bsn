#include "bsn/configuration/SensorConfiguration.hpp"

using namespace std;
using namespace bsn::range;

namespace bsn {
	namespace configuration {

		SensorConfiguration::SensorConfiguration(){}

		SensorConfiguration::SensorConfiguration(int i, Range l, 
			array<Range, 2> m, array<Range, 2> h, array<Range, 3> p ){
				
			this->id 		 	  = i;		
			this->low_risk 	 	  = l;
			this->medium_risk	  = m;
			this->high_risk  	  = h;
			this->low_percentage  = p[0];
			this->mid_percentage  = p[1];
			this->high_percentage = p[2];
		}

		int SensorConfiguration::getId() {
			return this->id;
		}

		// Retorna o quão deslocado do meio um valor está
		double SensorConfiguration::getDisplacement(Range range, double number, bool inverse = true) {
			// inverse indica se o range deve ser avaliado de forma contrária
			// (Para os intervalos abaixo do low risk(medium0 high0))
			double lb = range.getLowerBound();
			double ub = range.getUpperBound();
			double result;

			if(!inverse) {
				result = fabs(number - ub);
				result /= (ub - lb);
			}
			else {
				result = number - lb;
				result /= (ub - lb);
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
				displacement = getDisplacement(low_risk, number);
				return convertRealPercentage(low_percentage, displacement);
			}

			else if(medium_risk[0].in_range(number)) {
				displacement = getDisplacement(medium_risk[0], number, false);	
				return convertRealPercentage(mid_percentage, displacement);	
			}

			else if(medium_risk[1].in_range(number)) {
				displacement = getDisplacement(medium_risk[1], number);
				return convertRealPercentage(mid_percentage, displacement);	
			}

			else if(high_risk[0].in_range(number)) {
				displacement = getDisplacement(high_risk[0], number, false);
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
