#include <cxxtest/TestSuite.h>
#include "bsn/entity/BodyHub.h"
#include "bsn/data/SensorData.h"


using namespace std;
using namespace bsn::data;
using namespace bsn::entity;

class BodyHubTestSuite : public CxxTest::TestSuite{
  public:

    void testConstructor(){
        BodyHub bh();

        TS_ASSERT(bh != NULL);
    }

    void testUpdateHealthStatus() {
        /*SetUp*/
        BodyHub bh;

        timespec ts;
        SensorData sd(10, 2, "alto", ts);

        map<uint32_t, string> sensorsMap = { 
            {0,"baixo"}, {1,"baixo"}, {2,"baixo"} 
        };
        bh.setSensorReg(sensorsMap);

        /*Operation*/
        bh.updateHealthStatus(sd);

        /*Assertion*/
        map<uint32_t, string> updatedSensorMap = bh.getSensorReg();
        TS_ASSERT_EQUALS(updatedSensorMap[0], "baixo");
        TS_ASSERT_EQUALS(updatedSensorMap[1], "alto");
        TS_ASSERT_EQUALS(updatedSensorMap[2], "baixo");
        TS_ASSERT_EQUALS(bh.getHealthStatus(), "ruim");
    }

};