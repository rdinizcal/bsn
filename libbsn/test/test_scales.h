#include <cxxtest/TestSuite.h>
#include "bsn/scales/ScaleConverter.hpp"

#include <stdint.h>
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace bsn::scales;

class ScalesTestSuite : public CxxTest::TestSuite{    
    public:
        void test_simple_conversion(){
            cout << "\n     Testando se 0 realmente retorna 0";
            double s = 0;
            ScaleConverter conv;
            conv.setLowerBound(0);
            conv.setUpperBound(50);
            TS_ASSERT_EQUALS(0,conv.to_MeasureUnit(s));
        }

        void test_midle_conversion(){
            cout << "\n     Testando se 768 retorna 37.5";
            double s = 768;
            ScaleConverter conv;
            conv.setLowerBound(0);
            conv.setUpperBound(50);
            TS_ASSERT_EQUALS(37.5, conv.to_MeasureUnit(s));
        }

        void test_out_bounds(){
            cout << "\n     Testando fora dos limites";
            ScaleConverter conv;
            conv.setLowerBound(0);
            conv.setUpperBound(50);
            TS_ASSERT_THROWS(conv.to_MeasureUnit(-1.0), std::invalid_argument);
            TS_ASSERT_THROWS(conv.to_MeasureUnit(1025), std::invalid_argument);
        }

        void test_bounds(){
            cout << "\n     Testando dentro limites";        
            double a = 0;
            double b = 1024;
            ScaleConverter conv;
            conv.setLowerBound(0);
            conv.setUpperBound(100);
            TS_ASSERT_EQUALS(0, conv.to_MeasureUnit(a));
            TS_ASSERT_EQUALS(100, conv.to_MeasureUnit(b));
        }

        void test_convertingBPM(){
            cout << "\n     Testando conversão para BPM a partir do ECG";
            ScaleConverter conv;
            conv.setLowerBound(0);
            conv.setUpperBound(200);
            TS_ASSERT_EQUALS(80, conv.to_MeasureUnit(409.6));
            TS_ASSERT_EQUALS(20, conv.to_MeasureUnit(102.4));
        }

        void test_convertingOXM(){
            cout << "\n     Testando conversão para porcentagem a partir do Oximetro";
            ScaleConverter conv;
            conv.setLowerBound(0);
            conv.setUpperBound(100);
            TS_ASSERT_EQUALS(95, conv.to_MeasureUnit(972.8));
            TS_ASSERT_EQUALS(5, conv.to_MeasureUnit(51.2));
        }

};