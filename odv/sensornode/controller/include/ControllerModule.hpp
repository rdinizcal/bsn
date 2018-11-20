#ifndef CONTROLLER_MODULE_HPP
#define CONTROLLER_MODULE_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/operation/Operation.hpp"
#include "bsn/data/SensorStatusInfo.h"
#include "bsn/data/FreqUpdate.h"

#include <iostream>

class ControllerModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
      	ControllerModule(const ControllerModule & /*obj*/);
    	ControllerModule &operator=(const ControllerModule & /*obj*/);
    	virtual void setUp();
    	virtual void tearDown();

  public:
    	ControllerModule(const int32_t &argc, char **argv);
    	virtual ~ControllerModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
		odcore::base::FIFOQueue dataBuffer;
		std::array<bsn::range::Range,5> rangesArray;

};

#endif 