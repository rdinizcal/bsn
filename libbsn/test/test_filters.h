#include <cxxtest/TestSuite.h>

#include <stdint.h>
#include <iostream>

#include "bsn/filters/MovingAverage.hpp"

using namespace std;
using namespace bsn::filters;

class FiltersTestSuite: public CxxTest::TestSuite{    
  	public:
	
  	void test_initial_avg() {
    	cout << "Testando os Filtros\n";
		MovingAverage avg(1);
		cout << "\tTestando uma condição inicial 0\n";
    	TS_ASSERT_EQUALS(avg.getValue(),0.0);       
  	}
  	void test_1_avg() {
    	cout << "\tTestando com 1 valor\n";
		MovingAverage avg(1);
		avg.insert(1);
    	TS_ASSERT_EQUALS(avg.getValue(),1.0);
  	}
  	void test_avg() {
    	cout << "\tTestando de forma geral com 5 valores\n";
    	MovingAverage avg(5);
    	avg.insert(32.0);
    	avg.insert(35.0);
    	avg.insert(36.0);
    	avg.insert(34.0);
    	avg.insert(35.0);
   		TS_ASSERT_EQUALS(avg.getValue(),34.4);
  	}

	void test_avg_with_less_values() {
		cout << "\tTestando com um numero menor de valores fornecidos\n";
		MovingAverage avg(5);
		avg.insert(32.0);
		avg.insert(35.0);
		avg.insert(36.0);
		avg.insert(34.0);
		TS_ASSERT_EQUALS(avg.getValue(), 34.0);
	}

	void test_avg_with_more_values() {
		cout << "\tTestando com um numero maior de valores fornecidos\n";
		MovingAverage avg(5);
		avg.insert(200);
		avg.insert(32.0);
		avg.insert(35.0);
		avg.insert(36.0);
		avg.insert(34.0);
		avg.insert(35.0);
		avg.insert(37.0);
		TS_ASSERT_EQUALS(avg.getValue(), 35.4);
	}

};