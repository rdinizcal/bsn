#ifndef BODYHUB_MODULE_H_
#define BODYHUB_MODULE_H_

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

#include "openbasn/message/Request.h"
#include "openbasn/data/SensorNodeData.h"

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
        void processSensorNodeData(SensorNodeData /*obj*/, TimeStamp /*sentTS*/, TimeStamp /*receivedTS*/);
        void requestSensorNodeData(string /*health risk label*/);
        double evaluateSensorDataRisk(uint32_t /*type*/, double /*data*/);
            
    private:
        uint32_t m_id;
        int32_t m_clock;
        FIFOQueue m_buffer;
        
        map<uint32_t, string> m_sensornode;
        ofstream m_datalog;
};

#endif