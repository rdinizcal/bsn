#ifndef BLOODPRESSURE_MODULE_HPP
#define BLOODPRESSURE_MODULE_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/generator/Markov.hpp"
#include "bsn/filters/MovingAverage.hpp"
#include "bsn/operation/Operation.hpp"

#include "bsn/msg/data/SpecData.h"
#include "bsn/msg/control/BloodpressureControlCommand.hpp"

class BloodpressureModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	BloodpressureModule(const BloodpressureModule & /*obj*/);
    	BloodpressureModule &operator=(const BloodpressureModule & /*obj*/);
    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	BloodpressureModule(const int32_t &argc, char **argv);
    	virtual ~BloodpressureModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		
		std::string type;
		double battery;

		bool active;
		std::map<std::string,double> params;

		bsn::generator::Markov markovSystolic;
		bsn::generator::Markov markovDiastolic;
		bsn::filters::MovingAverage filterSystolic;
		bsn::filters::MovingAverage filterDiastolic;
};

#endif 