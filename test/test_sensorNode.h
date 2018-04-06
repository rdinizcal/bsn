#include <cxxtest/TestSuite.h>
#include <stdint.h>
#include "../sensornode/include/SensorNodeModule.h"

class SensorNodeTestSuite : public CxxTest::TestSuite {
  public:
    char* arg0;
    char* arg1;
    char* arg2;
    char* argv[4];
    int32_t   argc;

    void initialize_sn_instance(){
      //inicializa a instância a ser tratada
      arg0    = (char *) "main";
      arg1    = (char *) "--cid=111";
      arg2    = (char *) "--id=0";
      argv[0] = &arg0[0];
      argv[1] = &arg1[0];
      argv[2] = &arg2[0];
      argv[3] = NULL;
      argc    = (int32_t)(sizeof(argv) / sizeof(argv[0])) - 1;
    }

    void test_class_variables(){      
      cout << "\n\nTestando Sensor node";
      cout << "\n    Testando acessibilidade de variaveis de classe: ";
      initialize_sn_instance();
      SensorNodeModule inst(argc,argv);
      inst.m_id = 1;
      inst.m_sensor_type = 2;
      TS_ASSERT_EQUALS(inst.m_id, 1);
      TS_ASSERT_EQUALS(inst.m_sensor_type, 2);
    }

    void test_controllerFSM(){
      cout << "\n    Testando metodo 'controllerFSM'";
      initialize_sn_instance();
      SensorNodeModule inst(argc, argv);
      inst.m_status = "baixo";
      cout << "\n     'm_status' = baixo";
      TS_ASSERT_EQUALS(inst.controllerFSM(10), true);
      TS_ASSERT_EQUALS(inst.controllerFSM(8), false);
      inst.m_status = "moderado";
      cout << "\n     'm_status' = moderado";
      TS_ASSERT_EQUALS(inst.controllerFSM(5), true);
      TS_ASSERT_EQUALS(inst.controllerFSM(3), false);
      inst.m_status = "alto";
      cout << "\n     'm_status' = alto";
      TS_ASSERT_EQUALS(inst.controllerFSM(1), true);
      TS_ASSERT_EQUALS(inst.controllerFSM(0), false);
    }

    void test_statusAnalysis(){
      cout << "\n    Testando metodo 'statusAnalysis'";
      initialize_sn_instance();
      SensorNodeModule inst(argc, argv);
      
      for(int i = 0; i < 4; i++){
        inst.m_data_queue.push_back("baixo");
      }
      cout << "\n     Teste de status baixo, com historico em baixo";
      TS_ASSERT_EQUALS(inst.statusAnalysis("baixo", "moderado"), "baixo");

      inst.m_data_queue.clear();
      for(int i = 0; i < 4; i++){
        inst.m_data_queue.push_back("moderado");
      }
      cout << "\n     Teste de status moderado, com historico em moderado";
      TS_ASSERT_EQUALS(inst.statusAnalysis("moderado", "alto"), "moderado");

      inst.m_data_queue.clear();
      for(int i = 0; i < 4; i++){
        inst.m_data_queue.push_back("alto");
      }
      cout << "\n     Teste de status alto, com historico em alto";
      TS_ASSERT_EQUALS(inst.statusAnalysis("moderado", "baixo"), "alto");
    
      inst.m_data_queue.clear();
      inst.m_data_queue.push_back("moderado");
      inst.m_data_queue.push_back("moderado");
      inst.m_data_queue.push_back("baixo");
      inst.m_data_queue.push_back("alto");
      inst.m_data_queue.push_back("alto");
      cout << "\n     Teste de status atual, com historico variando";
      TS_ASSERT_EQUALS(inst.statusAnalysis("baixo", "moderado"), "moderado");
    }
};