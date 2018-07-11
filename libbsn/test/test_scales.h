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
        ScaleConverter conv(0,50);
        TS_ASSERT_EQUALS(0,conv.to_celsius(s));
    }
    void test_midle_conversion(){
        cout << "\n     Testando se 768 retorna 37.5";
        double s = 768;
        ScaleConverter conv(0,50);
        TS_ASSERT_EQUALS(37.5, conv.to_celsius(s));
    }

    void test_out_bounds(){
        cout << "\n     Testando fora dos limites";        
        ScaleConverter conv(0,50);
        TS_ASSERT_THROWS(conv.to_celsius(-1.0),std::invalid_argument);
        TS_ASSERT_THROWS(conv.to_celsius(1025),std::invalid_argument);
    }
    void test_bounds(){
        cout << "\n     Testando dentro limites";        
        double a = 0;
        double b = 1024;
        ScaleConverter conv(0,100);
        TS_ASSERT_EQUALS(0, conv.to_celsius(a));
        TS_ASSERT_EQUALS(100, conv.to_celsius(b));
    }

};