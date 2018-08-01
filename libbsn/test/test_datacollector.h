#include <cxxtest/TestSuite.h>
#include "bsn/generator/DataGenerator.h"

#include <stdint.h>
#include <iostream>

using namespace std;
using namespace bsn::generator;

class DataCollectorTestSuite : public CxxTest::TestSuite{    
  public:

    // TODO: 
    // teste do metodo generateDataByMarkovChain

    void test_generateDataByNormalDist(){
        cout << "\n     Testando metodo de gerar dados pela normal...";

        DataGenerator DataGenerator;

        double genValue = DataGenerator.generateDataByNormalDist(500, 100);

        TS_ASSERT_LESS_THAN(0, genValue);
        TS_ASSERT_LESS_THAN(genValue, 1000);
    }

};