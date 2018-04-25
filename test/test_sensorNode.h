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
};