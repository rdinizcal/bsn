#include <cxxtest/TestSuite.h>

#include <stdint.h>
#include <iostream>

#include "bsn/filters/Average.hpp"

using namespace std;
using namespace bsn::filters;

class FiltersTestSuite: public CxxTest::TestSuite{    
  	public:
  	void test_initial_avg(){
    	cout << "Testing filters\n";
    	Average avg;
    	cout << "\tTesting initial 0 condition\n";
    	TS_ASSERT_EQUALS(avg.get_value(),0.0);       
  	}
  	void test_1_avg(){
    	cout << "\tTesting initial average\n";
    	Average avg;
    	avg.insert(1);
    	TS_ASSERT_EQUALS(avg.get_value(),1.0);
  	}
  	void test_avg(){
    	cout << "\tTesting a random average\n";
    	Average avg;
    	avg.insert(35.0);
    	avg.insert(32.0);
    	avg.insert(36.0);
    	avg.insert(34.0);
    	avg.insert(35.0);
   		TS_ASSERT_EQUALS(avg.get_value(),34.4);
  	}

};