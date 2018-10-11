#include <cxxtest/TestSuite.h>
#include <iostream>

#include "bsn/configuration/sensor_configuration.hpp"

using namespace std;
using namespace bsn::range;

class SensoConfigurationTestSuite: public CxxTest::TestSuite{    
  	public:

  	void test_constructor() {
        cout << "Testando configurações do sensor" << endl;
        cout << "\tTestando construtor:\n";
        Range r1(1.0,2.0);
        Range r2(3.0,4.0);
        Range r3(5.0,6.0);
        
        sensor_configuration s(0,r1,r2,r3);
        TS_ASSERT_EQUALS(s.id, 0);
    }

    void test_unknow(){
        cout << "\tTestando avaliação sinal unknow\n";
        Range r1(0.0,2.9);
        Range r2(3.0,4.9);
        Range r3(5.0,6.9);

        sensor_configuration s(0,r1,r2,r3);
        TS_ASSERT_EQUALS(s.evaluate_number(15.0), -1.0);
    }

    void test_low(){        
        cout << "\tTestando avaliação sinal baixo\n"; 
        Range r1(0.0,4.0);
        Range r2(4.0,5.0);
        Range r3(5.0,7.0);

        sensor_configuration s(0,r1,r2,r3);
        TS_ASSERT_EQUALS(s.evaluate_number(0.0), 1.0);
        TS_ASSERT_EQUALS(s.evaluate_number(1.5), 0.75);
    }

    void test_medium(){
        cout << "\tTestando avaliação sinal medium\n";
        Range r1(1.0,2.0);
        Range r2(3.0,4.0);
        Range r3(5.0,6.0);

        sensor_configuration s(0,r1,r2,r3);        
        TS_ASSERT_EQUALS(s.evaluate_number(3.5),0.0);
    }

    void test_high(){
        cout << "\tTestando avaliação sinal high\n"; 
        Range r1(1.0,2.0);
        Range r2(3.0,4.0);
        Range r3(5.0,6.0);

        sensor_configuration s(0,r1,r2,r3);
        TS_ASSERT_EQUALS(s.evaluate_number(5.625), 0.75);
        TS_ASSERT_EQUALS(s.evaluate_number(6.0), 1.0);        
    }
};