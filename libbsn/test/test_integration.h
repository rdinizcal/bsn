#include <cxxtest/TestSuite.h>
#include "bsn/range/Range.hpp"
#include "bsn/generator/Markov.hpp"
#include "bsn/configuration/SensorConfiguration.hpp"

#include <stdint.h>
#include <iostream>

using namespace std;
using namespace bsn::range;
using namespace bsn::configuration;

class IntegrationTestSuite : public CxxTest::TestSuite{    
  public:
    Range l;
    Range m1;
    Range m2;
    Range h1;
    Range h2;    

    array<Range,2> a1;
    array<Range,2> a2;
    array<Range,3> percentages;

	SensorConfiguration s;
    array<float,25> transitions;
    array<Range, 5> ranges{Range(30.0, 33.0), Range(33.1, 36.4), Range(36.5, 37.5), Range(37.6, 39.0), Range(39.1, 42.0)};
    void setUp(){
        transitions =
        {
            0,100,0,0,0,
            0,0,100,0,0,
            0,0,0,100,0,
            0,0,0,0,100,
            100,0,0,0,0
        };

        array<Range, 5> ranges{Range(30.0, 33.0), Range(33.1, 36.4), Range(36.5, 37.5), Range(37.6, 39.0), Range(39.1, 42.0)};

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
    // Integração total do sistema
    void test_integration(){
        Markov m(transitions, ranges, 0);
        TS_ASSERT_EQUALS(m.current_state,0);
        double state = m.calculate_state();
        TS_ASSERT_LESS_THAN_EQUALS(state, 33.0);
        TS_ASSERT_LESS_THAN_EQUALS(30.0, state);
        
        SensorConfiguration s(1,l,a1,a2,percentages);
        TS_ASSERT_LESS_THAN_EQUALS(s.evaluateNumber(state), 0.2);
    }

};