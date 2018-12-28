#include <opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h>

#include "bsn/communication/TCPReceive.hpp"

using namespace bsn::communication;

class SensorModule : public odcore::base::module::TimeTriggeredConferenceClientModule {
    
    private:
        SensorModule(const SensorModule &/*obj*/);
        SensorModule& operator=(const SensorModule &/*obj*/);

    public:
        SensorModule(const int32_t &argc, char **argv);
        virtual ~SensorModule();
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        virtual void setUp();
        virtual void tearDown();
};