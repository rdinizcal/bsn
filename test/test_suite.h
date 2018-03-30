#include <cxxtest/TestSuite.h>
#include <stdint.h>
#include "../bodyhub/include/BodyHubModule.h"
            
using namespace std;

  class MyTestSuite : public CxxTest::TestSuite{
  public:
      void testAddition( void ){
          TS_ASSERT_EQUALS(1+4, 5);
      }

      void testMultiplication(void) {
        TS_TRACE("Starting multiplication test");
        TS_ASSERT_EQUALS(2 * 2, 5);
        TS_TRACE("Finishing multiplication test");
    }
    
      void testOpd(){
        int32_t argc; 
        char ** argv = NULL;
        BodyHubModule inst(argc, argv);
        inst.m_id = 1;
        TS_ASSERT_EQUALS(inst.m_id, 1);  
        
      }
  };