#include <cxxtest/TestSuite.h>
#include <iostream>

#include "bsn/configuration/SensorConfiguration.hpp"

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

	SensorConfiguration s;

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

        percentages = {Range(0,0.2), Range(0.21,0.65), Range(0.66,1.0)};
        a1 = {m1, m2};
        a2 = {h1, h2};		        
    }  

  	void test_constructor() {
        cout << "Testando configurações do sensor" << endl;
        cout << "\tTestando construtor:\n";        

        SensorConfiguration s(1,l,a1,a2,percentages);
                
        TS_ASSERT_EQUALS(1, s.getId());
    }

    void test_unknow_value() {
        cout << "\tTestando avaliação sinal unknow\n";
        TS_ASSERT_EQUALS(s.evaluateNumber(44), -1);
        TS_ASSERT_EQUALS(s.evaluateNumber(10), -1);
    }

    void test_getDisplacement_normal() {
        cout << "\tTestando metodo getDisplacement em ordem\n";
        
        SensorConfiguration s(1,l,a1,a2,percentages);
        
        TS_ASSERT_EQUALS(s.getDisplacement( Range(0,10), 5, true), 0.5 );
        TS_ASSERT_EQUALS(s.getDisplacement( Range(0,10), 10, true), 1 );
        TS_ASSERT_EQUALS(s.getDisplacement( Range(0,10), 0, true), 0 );
        TS_ASSERT_EQUALS(s.getDisplacement( Range(0,10), 7.5, true), 0.75 );
        TS_ASSERT_EQUALS(s.getDisplacement( Range(0,10), 2.5, true), 0.25 );
    }

    void test_getDisplacement_inverse() {
        cout << "\tTestando metodo getDisplacement com logica inversa\n";
        
        SensorConfiguration s(1,l,a1,a2,percentages);
        
        TS_ASSERT_EQUALS(s.getDisplacement( Range(0,10), 5, false), 0.5 );
        TS_ASSERT_EQUALS(s.getDisplacement( Range(0,10), 10, false), 0 );
        TS_ASSERT_EQUALS(s.getDisplacement( Range(0,10), 0, false), 1 );
        TS_ASSERT_EQUALS(s.getDisplacement( Range(0,10), 7.5, false), 0.25 );
        TS_ASSERT_EQUALS(s.getDisplacement( Range(0,10), 2.5, false), 0.75 );
    }

    void test_low() {
        cout << "\tTestando avaliação sinal low\n";
        
        SensorConfiguration s(1,l,a1,a2,percentages);
        
        TS_ASSERT_EQUALS(s.evaluateNumber(37),0.1);

        TS_ASSERT_EQUALS(s.evaluateNumber(37.5), 0.2);
        TS_ASSERT_EQUALS(s.evaluateNumber(36.5), 0);

        TS_ASSERT_EQUALS(s.evaluateNumber(36.75), 0.05);
    }

    void test_medium0() {
        cout << "\tTestando avaliação sinal medium0\n";
        
        SensorConfiguration s(1,l,a1,a2,percentages);                

        TS_ASSERT_EQUALS(s.evaluateNumber(33), 0.65);
        TS_ASSERT_EQUALS(s.evaluateNumber(35), 0.21);
    }

    void test_medium1() {
        cout << "\tTestando avaliação sinal medium1\n";
        
        SensorConfiguration s(1,l,a1,a2,percentages);        

        TS_ASSERT_EQUALS(s.evaluateNumber(37.6), 0.21);
        TS_ASSERT_EQUALS(s.evaluateNumber(39), 0.65);
        
    }

    void test_high0() {
        cout << "\tTestando avaliação sinal high0\n";
        
        SensorConfiguration s(1,l,a1,a2,percentages);

        TS_ASSERT_EQUALS(s.evaluateNumber(20), 1);
        TS_ASSERT_EQUALS(s.evaluateNumber(32), 0.66);
        
    }

    void test_high1() {        
        cout << "\tTestando avaliação sinal high1\n";
        
        SensorConfiguration s(1,l,a1,a2,percentages);            

        TS_ASSERT_EQUALS(s.evaluateNumber(39.1), 0.66);
        TS_ASSERT_EQUALS(s.evaluateNumber(43), 1.0);
    }
};
