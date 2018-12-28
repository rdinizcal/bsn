#ifndef CONTROLLERAPI_SENDER_MODULE_HPP
#define CONTROLLERAPI_SENDER_MODULE_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/communication/TCPSend.hpp"

class ControllerAPISenderModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
      	ControllerAPISenderModule(const ControllerAPISenderModule &/*obj*/);
    	ControllerAPISenderModule &operator=(const ControllerAPISenderModule & /*obj*/);
    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	ControllerAPISenderModule(const int32_t &argc, char **argv);
    	virtual ~ControllerAPISenderModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
        odcore::base::FIFOQueue buffer;
};

#endif 