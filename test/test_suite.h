#include <cxxtest/TestSuite.h>
#include <stdint.h>
#include "../bodyhub/include/BodyHubModule.h"
#include "../sensornode/include/SensorNodeModule.h"
            
using namespace std;
  
class BodyHubTestSuite : public CxxTest::TestSuite{
  public:
    char* arg0;
    char* arg1;
    char* argv[3];
    int32_t   argc;

    void initialize_bh_instance(){
      //inicializa a instância a ser tratada
      arg0    = (char *) "main";
      arg1    = (char *) "--cid=111";
      argv[0] = &arg0[0];
      argv[1] = &arg1[0];
      argv[2] = NULL;
      argc    = (int32_t)(sizeof(argv) / sizeof(argv[0])) - 1;
    }
    void test_class_variables( void ){     
      cout << "\n\nTestes do BodyHub";
      cout << "\n    Testando se variáveis de classe são acessáveis: ";     

      initialize_bh_instance();
      BodyHubModule inst(argc, argv);
      inst.m_id = 1;
      inst.m_health_status = "baixo";
      TS_ASSERT_EQUALS(inst.m_id, 1);
      TS_ASSERT_EQUALS(inst.m_health_status, "baixo");    
    }

    void test_CHS (void) {
      cout << "\n    Testando função 'calculateHealthStatus': ";   
      initialize_bh_instance();
      BodyHubModule inst(argc, argv);
      inst.m_sensor[0] = "baixo";
      inst.m_sensor[1] = "baixo";
      inst.m_sensor[2] = "baixo";
      TS_ASSERT_EQUALS(inst.calculateHealthStatus(), "bom");    

      inst.m_sensor[0] = "moderado";
      inst.m_sensor[1] = "moderado";
      inst.m_sensor[2] = "moderado";
      TS_ASSERT_EQUALS(inst.calculateHealthStatus(), "medio");    

      inst.m_sensor[0] = "alto";
      inst.m_sensor[1] = "alto";
      inst.m_sensor[2] = "alto";
      TS_ASSERT_EQUALS(inst.calculateHealthStatus(), "ruim");    
    }

    void test_UHS(void) {
      timespec ts;
      cout << "\n    Testando função 'updateHealthStatus': ";   

      initialize_bh_instance();
      BodyHubModule inst(argc, argv);    
      inst.m_sensor[0] = "medio";
      inst.m_sensor[1] = "baixo";
      inst.m_sensor[2] = "baixo";

      clock_gettime(CLOCK_REALTIME, &ts);
      inst.updateHealthStatus(SensorData(0,1,"baixo",ts));
      //Vê se recebe o mesmo status que foi enviado
      TS_ASSERT_EQUALS(inst.m_sensor[0], "baixo");
      //como os tres sinais sao baixos, o status deve ser bom
      TS_ASSERT_EQUALS(inst.m_health_status, "bom");
    }
};

class SensorNodeTestSuite : public CxxTest::TestSuite{

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

    void test_class_variables (void) {      
      cout << "\n\nTestando Sensor node";
      cout << "\n    Testando acessibilidade de variáveis de classe: ";
      initialize_sn_instance();
      SensorNodeModule inst(argc,argv);
      inst.m_id = 1;
      inst.m_sensor_type = 2;
      TS_ASSERT_EQUALS(inst.m_id, 1);
      TS_ASSERT_EQUALS(inst.m_sensor_type, 2);

    }

};