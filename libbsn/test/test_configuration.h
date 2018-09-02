#include <cxxtest/TestSuite.h>
#include <iostream>

#include "bsn/configuration/sensor_configuration.hpp"

using namespace std;

class SensoConfigurationTestSuite: public CxxTest::TestSuite{    
  	public:

  	void test_constructor() {
        cout << "Testando configurações do sensor" << endl;
        cout << "\tTestando construtor:\n";
        int id = 0;
        range r1(1.0,2.0);
        range r2(3.0,4.0);
        range r3(5.0,6.0);
        
        sensor_configuration s(0,r1,r2,r3);
        TS_ASSERT_EQUALS(s.id, 0);
    }

    void test_unknow(){
        cout << "\tTestando avaliação sinal unknow\n";
        int id = 0;
        range r1(0.0,2.9);
        range r2(3.0,4.9);
        range r3(5.0,6.9);

        sensor_configuration s(0,r1,r2,r3);
        TS_ASSERT_EQUALS(s.evaluate_number(15.0), -1.0);
    }

    void test_low(){        
        cout << "\tTestando avaliação sinal baixo\n";
        int id = 0;
        range r1(0.0,4.0);
        range r2(4.0,5.0);
        range r3(5.0,7.0);

        sensor_configuration s(0,r1,r2,r3);
        TS_ASSERT_EQUALS(s.evaluate_number(0.0), 1.0);
        TS_ASSERT_EQUALS(s.evaluate_number(1.5), 0.75);
    }

    void test_medium(){
        cout << "\tTestando avaliação sinal medium\n";
        int id = 0;
        range r1(1.0,2.0);
        range r2(3.0,4.0);
        range r3(5.0,6.0);

        sensor_configuration s(0,r1,r2,r3);        
        TS_ASSERT_EQUALS(s.evaluate_number(3.5),0.0);
    }

    void test_high(){
        cout << "\tTestando avaliação sinal high\n";
        int id = 0;
        range r1(1.0,2.0);
        range r2(3.0,4.0);
        range r3(5.0,6.0);

        sensor_configuration s(0,r1,r2,r3);
        TS_ASSERT_EQUALS(s.evaluate_number(5.625), 0.75);
        TS_ASSERT_EQUALS(s.evaluate_number(6.0), 1.0);        
    }
};