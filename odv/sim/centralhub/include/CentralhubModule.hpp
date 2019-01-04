#ifndef CENTRALHUB_MODULE_HPP
#define CENTRALHUB_MODULE_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/processor/Processor.hpp"

#include "bsn/msg/info/TaskInfo.hpp"
#include "bsn/msg/data/SensorData.h"
#include "bsn/msg/info/PatientStatusInfo.h"


class CentralhubModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
        CentralhubModule(const CentralhubModule & /*obj*/);
        CentralhubModule &operator=(const CentralhubModule & /*obj*/);
        virtual void setUp();
        virtual void tearDown();   

		void sendTaskInfo(const std::string &/*task_id*/, const double &/*cost*/, const double &/*reliability*/, const double &/*frequency*/);

    public:
        CentralhubModule(const int32_t &argc, char **argv);
        virtual ~CentralhubModule();

        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        odcore::base::FIFOQueue buffer;
};

#endif 