#ifndef CONTROLLERAPI_RECEIVER_MODULE_HPP
#define CONTROLLERAPI_RECEIVER_MODULE_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/communication/TCPReceive.hpp"

class ControllerAPIReceiverModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
      	ControllerAPIReceiverModule(const ControllerAPIReceiverModule &/*obj*/);
    	ControllerAPIReceiverModule &operator=(const ControllerAPIReceiverModule & /*obj*/);
    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	ControllerAPIReceiverModule(const int32_t &argc, char **argv);
    	virtual ~ControllerAPIReceiverModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
        odcore::base::FIFOQueue buffer;
};

#endif 