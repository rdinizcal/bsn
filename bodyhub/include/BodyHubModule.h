#ifndef BODYHUB_MODULE_H_
#define BODYHUB_MODULE_H_

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

#include "openbasn/message/Request.h"
#include "openbasn/data/SensorData.h"

#include <sys/time.h>
#include <iostream>
#include <fstream>

#include <map>
#include <string>
#include <iterator>
#include <algorithm>
#include <chrono>

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
        timespec elapsedTime(timespec &/*now*/, timespec &/*ref*/);
        string calculateHealthStatus();
        void updateHealthStatus(SensorData /*sensordata*/);
        void persistHealthStatus(uint32_t /*sensor_id*/, timespec /*sent_timespec*/, timespec /*received_timestamp*/);
        void printHealthStatus();
            
    private:
        uint32_t m_id;
        FIFOQueue m_buffer;
        string m_health_status;
        map<uint32_t, string> m_sensor;

        ofstream m_status_log;
        ofstream m_bodyhub_log;

        timespec m_ref;
};

#endif