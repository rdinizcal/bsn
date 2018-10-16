#include <cxxtest/TestSuite.h>
#include <iostream>

#include "bsn/configuration/sensor_configuration.hpp"

using namespace std;
using namespace bsn::range;

class SensoConfigurationTestSuite: public CxxTest::TestSuite{    
    Range l;
    Range m1;
    Range m2;
    Range h1;
    Range h2;    

    array<Range,2> a1;
    array<Range,2> a2;
    array<Range,3> percentages;

	sensor_configuration s;

  	public:

    void setUp(){
        l.setLowerBound(36.5);
		l.setUpperBound(37.5);

		m1.setLowerBound(33);
		m1.setUpperBound(35);

        h1.setLowerBound(20);
		h1.setUpperBound(32);

        m2.setLowerBound(37.6);
		m2.setUpperBound(39.0);

        h2.setLowerBound(39.1);
		h2.setUpperBound(43.0);

        percentages = {Range(0,0.2), Range(0.31,0.65), Range(0.66,1.0)};
        a1 = {m1, m2};
        a2 = {h1, h2};		        
    }  

  	void test_constructor() {
        cout << "Testando configurações do sensor" << endl;
        cout << "\tTestando construtor:\n";        

        sensor_configuration s(1,l,a1,a2,percentages);
                
        TS_ASSERT_EQUALS(s.id, 1);
    }

    void test_unknow_value() {
        cout << "\tTestando avaliação sinal unknow\n";
        TS_ASSERT_EQUALS(s.evaluate_number(11), -1);
        TS_ASSERT_EQUALS(s.evaluate_number(-1), -1);
    }

    void test_get_displacement() {
        cout << "\tTestando metodo get_displacement\n";
        
        sensor_configuration s(1,l,a1,a2,percentages);
        
        TS_ASSERT_EQUALS(s.get_displacement(Range(0,10),5), 0);
        TS_ASSERT_EQUALS(s.get_displacement(Range(0,10),10), 1);
        TS_ASSERT_EQUALS(s.get_displacement(Range(0,10),10), 1);
        TS_ASSERT_EQUALS(s.get_displacement(Range(0,10),7.5), 0.5);
        TS_ASSERT_EQUALS(s.get_displacement(Range(0,10),2.5), 0.5);
    }

    void test_low() {
        cout << "\tTestando avaliação sinal low\n";
        
        sensor_configuration s(1,l,a1,a2,percentages);
        
        TS_ASSERT_EQUALS(s.evaluate_number(37),0);

        TS_ASSERT_EQUALS(s.evaluate_number(37.5), 0.20);
        TS_ASSERT_EQUALS(s.evaluate_number(36.5), 0.20);

        TS_ASSERT_EQUALS(s.evaluate_number(37.25), 0.10);
        TS_ASSERT_EQUALS(s.evaluate_number(36.75), 0.10);
    }

    void test_medium0() {
        cout << "\tTestando avaliação sinal medium0\n";
        
        sensor_configuration s(1,l,a1,a2,percentages);
        
        TS_ASSERT_EQUALS(s.evaluate_number(34),0.31);

        TS_ASSERT_EQUALS(s.evaluate_number(33), 0.65);
        TS_ASSERT_EQUALS(s.evaluate_number(35), 0.65);
    }

    void test_medium1() {
        cout << "\tTestando avaliação sinal medium1\n";
        
        sensor_configuration s(1,l,a1,a2,percentages);        

        TS_ASSERT_EQUALS(s.evaluate_number(37.6), 0.65);
        TS_ASSERT_EQUALS(s.evaluate_number(39), 0.65);
        
    }

    void test_high0() {
        cout << "\tTestando avaliação sinal high0\n";
        
        sensor_configuration s(1,l,a1,a2,percentages);

        TS_ASSERT_EQUALS(s.evaluate_number(26),0.66);

        TS_ASSERT_EQUALS(s.evaluate_number(20), 1.0);
        TS_ASSERT_EQUALS(s.evaluate_number(32), 1.0);
        
    }

    void test_high1() {        
        cout << "\tTestando avaliação sinal high1\n";
        
        sensor_configuration s(1,l,a1,a2,percentages);            

        TS_ASSERT_EQUALS(s.evaluate_number(39.1), 1.0);
        TS_ASSERT_EQUALS(s.evaluate_number(43), 1.0);
    }
};
