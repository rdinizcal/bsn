#include <cxxtest/TestSuite.h>
#include "bsn/scales/ScaleConverter.hpp"

#include <stdint.h>
#include <iostream>
#include <stdexcept>

using namespace std;

class ScalesTestSuite : public CxxTest::TestSuite{    
  public:
    void test_simple_conversion(){
        cout << "\n     Testando se 0 realmente retorna 0";
        double s = 0;
        TS_ASSERT_EQUALS(0,to_celsius(s));
    }
    void test_midle_conversion(){
        cout << "\n     Testando se 768 retorna 37.5";
        double s = 768;
        TS_ASSERT_EQUALS(37.5, to_celsius(s));
    }

    void test_out_bounds(){
        cout << "\n     Testando limites";        
        
        TS_ASSERT_THROWS(to_celsius(-1.0),std::invalid_argument);
        TS_ASSERT_THROWS(to_celsius(1025),std::invalid_argument);
    }

};