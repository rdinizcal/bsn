#ifndef BODYHUB_MODULE_H_
#define BODYHUB_MODULE_H_

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

using namespace odcore::base::module;

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
        uint32_t m_id;
        map <uint32_t, string> sensornode_risk;
};

#endif