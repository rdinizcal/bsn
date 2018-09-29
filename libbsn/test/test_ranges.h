#include <cxxtest/TestSuite.h>
#include <iostream>

#include "bsn/range/Range.hpp"

using namespace std;

class RangesTestSuite: public CxxTest::TestSuite{    
  	public:
  	void test_constructor() {
		  cout << "Testando intervalos" << endl;
		  cout << "\tTestando construtor:\n";
		  range r(2.5,5.0);
		  TS_ASSERT_EQUALS(r.lower_bound,2.5);
		  TS_ASSERT_EQUALS(r.upper_bound,5.0);
    }
	void test_illegal_construction(){
		cout << "\tTestando construtor com argumentos ilegais:\n";
		TS_ASSERT_THROWS(range r(7.5,5.0), std::invalid_argument);
	}
	void test_in_range(){
		cout << "\tTestando dentro do intervalo:\n";
		range r(2.5,5.0);
		TS_ASSERT_EQUALS(true, r.in_range(3.0));
	}
	void test_out_range(){
		cout << "\tTestando fora do intervalo:\n";
		range r(2.5,5.0);
		TS_ASSERT_EQUALS(false, r.in_range(8.0));
	}
};