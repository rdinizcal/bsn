#ifndef ACTUATOR_MODULE_HPP
#define ACTUATOR_MODULE_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

class ActuatorModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
        ActuatorModule(const ActuatorModule & /*obj*/);
        ActuatorModule &operator=(const ActuatorModule & /*obj*/);
        virtual void setUp();
        virtual void tearDown();

    public:
        ActuatorModule(const int32_t &argc, char **argv);
        virtual ~ActuatorModule();

        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
};

#endif 