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

    void setUp(){
      //inicializa a instância a ser tratada
      arg0    = (char *) "main";
      arg1    = (char *) "--cid=111";
      arg2    = (char *) "--id=1";
      argv[0] = &arg0[0];
      argv[1] = &arg1[0];
      argv[2] = &arg2[0];
      argv[3] = NULL;
      argc    = (int32_t)(sizeof(argv) / sizeof(argv[0])) - 1;
    }

    void testConstructor(){
        SensorNodeModule snm(argc, argv);

        TS_ASSERT_EQUALS(snm.getIdentifier(), 1);
        TS_ASSERT_EQUALS(snm.getName(), "sensornode");
        TS_ASSERT_EQUALS(snm.getCID(), (uint32_t) 111);
    }
};