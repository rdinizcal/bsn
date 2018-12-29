#ifndef SENSOR_MODULE_HPP
#define SENSOR_MODULE_HPP

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>
#include <map>

#include "opendavinci/odcore/base/Thread.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/generator/Markov.hpp"
#include "bsn/filters/MovingAverage.hpp"
#include "bsn/operation/Operation.hpp"

#include "bsn/msg/data/SpecData.h"
#include "bsn/msg/control/ControlSignal.hpp"

class SensorModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	SensorModule(const SensorModule & /*obj*/);
    	SensorModule &operator=(const SensorModule & /*obj*/);
    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	SensorModule(const int32_t &argc, char **argv);
    	virtual ~SensorModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		
		double battery;

		bool active;
		std::map<std::string,double> params;

		bsn::generator::Markov markov;
		bsn::filters::MovingAverage filter;
};

#endif 