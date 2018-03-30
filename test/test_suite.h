#include <cxxtest/TestSuite.h>
#include <stdint.h>
#include "../bodyhub/include/BodyHubModule.h"
            
using namespace std;
  
class MyTestSuite : public CxxTest::TestSuite{
  public:

    void testOpd( void ){
      char arg0[] = "main";
      char  arg1[] = "--cid=111";
      char* argv[] = { &arg0[0], &arg1[0],NULL};
      int32_t   argc   = (int32_t)(sizeof(argv) / sizeof(argv[0])) - 1;
        
      BodyHubModule inst(argc, argv);
      inst.m_id = 1;
      TS_ASSERT_EQUALS(inst.m_id, 1);    
    }
};