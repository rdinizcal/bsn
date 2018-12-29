#ifndef THERMOMETER_MODULE_HPP
#define THERMOMETER_MODULE_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/generator/Markov.hpp"
#include "bsn/filters/MovingAverage.hpp"
#include "bsn/operation/Operation.hpp"

#include "bsn/msg/data/SpecData.h"
#include "bsn/msg/control/ThermometerControlCommand.hpp"

class ThermometerModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	ThermometerModule(const ThermometerModule & /*obj*/);
    	ThermometerModule &operator=(const ThermometerModule & /*obj*/);
    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	ThermometerModule(const int32_t &argc, char **argv);
    	virtual ~ThermometerModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		
		std::string type;
		double battery;

		bool active;
		std::map<std::string,double> params;

		bsn::generator::Markov markov;
		bsn::filters::MovingAverage filter;
};

#endif 