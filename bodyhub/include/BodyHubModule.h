#ifndef BODYHUB_MODULE_H_
#define BODYHUB_MODULE_H_

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

#include "openbasn/message/Request.h"
#include "openbasn/data/SensorData.h"

#include <fstream>

using namespace std;

using namespace odcore::base;
using namespace odcore::base::module;

using namespace openbasn::data;
using namespace openbasn::message;

class BodyHubModule : public TimeTriggeredConferenceClientModule {

    private:      
        BodyHubModule(const BodyHubModule &/*obj*/);
        BodyHubModule& operator=(const BodyHubModule &/*obj*/); 

    public:
        BodyHubModule(const int32_t &argc, char **argv);
        virtual ~BodyHubModule();
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        virtual void setUp();
        virtual void tearDown();

    private:
        void processRequest(Request /*obj*/);
        void processSensorData(SensorData /*obj*/, TimeStamp /*sent_timestamp*/, TimeStamp /*received_timestamp*/);
        string categorizeSensorData(SensorData /*obj*/);
        int32_t evaluateSensorDataRisk(uint32_t /*type*/, double /*data*/);
        void persistSensorData(SensorData /*obj*/, TimeStamp /*sent_timestamp*/, TimeStamp /*received_timestamp*/);
        string calculateHealthRisk();
        
            
    private:
        uint32_t m_id;
        FIFOQueue m_buffer;
        
        string m_health_risk;
        map<uint32_t, string> m_sensor;

        ofstream m_sensordata_log;
        ofstream m_status_log;
};

#endif