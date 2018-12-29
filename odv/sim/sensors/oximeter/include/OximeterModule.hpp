#ifndef OXIMETER_MODULE_HPP
#define OXIMETER_MODULE_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/generator/Markov.hpp"
#include "bsn/filters/MovingAverage.hpp"
#include "bsn/operation/Operation.hpp"

#include "bsn/msg/data/SpecData.h"
#include "bsn/msg/control/OximeterControlCommand.hpp"

class OximeterModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	OximeterModule(const OximeterModule & /*obj*/);
    	OximeterModule &operator=(const OximeterModule & /*obj*/);
    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	OximeterModule(const int32_t &argc, char **argv);
    	virtual ~OximeterModule();

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