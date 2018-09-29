#include <cxxtest/TestSuite.h>
#include <iostream>
#include <array>

#include "bsn/generator/Markov.hpp"
#include "bsn/range/range.hpp"


using namespace std;

class RangesTestSuite: public CxxTest::TestSuite{    
  	public:  	
	void test_next_states(){
		cout << "\tTestando passagem de estados:\n";
        array<float,9> transitions{0,100,0,
                                   0,0,100,
                                   100,0,0};
        array<range,3>ranges{range(1,3),range(4,6),range(7,9)};
        Markov m(transitions, ranges, 2);
        TS_ASSERT_EQUALS(2, m.current_state);
        m.next_state();
        TS_ASSERT_EQUALS(0, m.current_state);
        m.next_state();
        TS_ASSERT_EQUALS(1, m.current_state);
		
	}
	void test_state_calculations(){
		cout << "\tTestando intervalos dos estados:\n";

		array<float,9> transitions{0,100,0,
                                   0,0,100,
                                   100,0,0};


        array<range,3>ranges{range(1,3), range(4,6), range(7,9)};
        Markov m(transitions, ranges, 0);
        // Testando se o valor realmente corresponde ao intervalo do estado      
		TS_ASSERT_EQUALS(true, m.ranges[m.current_state].in_range(m.calculate_state()));
        m.next_state();
        TS_ASSERT_EQUALS(true, m.ranges[m.current_state].in_range(m.calculate_state()));
        m.next_state();
        TS_ASSERT_EQUALS(true, m.ranges[m.current_state].in_range(m.calculate_state()));
	}
};