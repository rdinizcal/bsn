#include <cxxtest/TestSuite.h>
#include "../include/bsn/statistics/VitalStatistics.h"

#include <stdint.h>
#include <iostream>

using namespace std;
using namespace bsn::statistics;

class VitalStatisticsTestSuite : public CxxTest::TestSuite{    
  public:
    void test_CHS (void) {
      cout << "\n\nTestes do módulo vital stats";
      cout << "\n    Testando metodo 'calculateHealthStatus': ";   
      map<uint32_t,string> test_map;
      test_map[0] = "baixo";
      test_map[1] = "baixo";
      test_map[2] = "baixo";
      TS_ASSERT_EQUALS(calculateHealthStatus(test_map), "bom");    

      test_map[0] = "moderado";
      test_map[1] = "moderado";
      test_map[2] = "moderado";
      TS_ASSERT_EQUALS(calculateHealthStatus(test_map), "medio");    

      test_map[0] = "alto";
      test_map[1] = "alto";
      test_map[2] = "alto";
      TS_ASSERT_EQUALS(calculateHealthStatus(test_map), "ruim");    
    }

    void test_controllerFSM(){
      cout << "\n    Testando metodo 'controllerFSM'";
      string test_status;
      test_status = "baixo";
      cout << "\n     'm_status' = baixo";
      TS_ASSERT_EQUALS(controllerFSM(10, test_status), true);
      TS_ASSERT_EQUALS(controllerFSM(8, test_status), false);
      test_status = "moderado";
      cout << "\n     'm_status' = moderado";
      TS_ASSERT_EQUALS(controllerFSM(5, test_status), true);
      TS_ASSERT_EQUALS(controllerFSM(3, test_status), false);
      test_status = "alto";
      cout << "\n     'm_status' = alto";
      TS_ASSERT_EQUALS(controllerFSM(1, test_status), true);
      TS_ASSERT_EQUALS(controllerFSM(0, test_status), false);
    }

    void test_statusAnalysis(){
      cout << "\n    Testando metodo 'statusAnalysis'";      
      deque<string> test_queue;
      for(int i = 0; i < 4; i++){
        test_queue.push_back("baixo");
      }
      cout << "\n     Teste de status baixo, com historico em baixo";
      TS_ASSERT_EQUALS(statusAnalysis("baixo", "moderado",test_queue), "baixo");

      test_queue.clear();
      for(int i = 0; i < 4; i++){
        test_queue.push_back("moderado");
      }
      cout << "\n     Teste de status moderado, com historico em moderado";
      TS_ASSERT_EQUALS(statusAnalysis("moderado", "alto", test_queue), "moderado");

      test_queue.clear();
      for(int i = 0; i < 4; i++){
        test_queue.push_back("alto");
      }
      cout << "\n     Teste de status alto, com historico em alto";
      TS_ASSERT_EQUALS(statusAnalysis("moderado", "baixo", test_queue), "alto");
    
      test_queue.clear();
      test_queue.push_back("moderado");
      test_queue.push_back("moderado");
      test_queue.push_back("baixo");
      test_queue.push_back("alto");
      test_queue.push_back("alto");
      cout << "\n     Teste de status atual, com historico variando";
      TS_ASSERT_EQUALS(statusAnalysis("baixo", "moderado", test_queue), "moderado");
    }
};