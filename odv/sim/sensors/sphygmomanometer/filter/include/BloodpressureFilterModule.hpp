#ifndef BLOODPRESSURE_FILTER_MODULE_HPP
#define BLOODPRESSURE_FILTER_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/filters/MovingAverage.hpp"

#include "bsn/msg/data/SensorData.h"

class BloodpressureFilterModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	BloodpressureFilterModule(const BloodpressureFilterModule &);
    	BloodpressureFilterModule &operator=(const BloodpressureFilterModule &);
    	
		virtual void setUp();
    	virtual void tearDown();

		
  	public:
    	BloodpressureFilterModule(const int32_t &argc, char **argv);
    	virtual ~BloodpressureFilterModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;
		bool active;
		std::map<std::string,double> params;
		
		bsn::filters::MovingAverage filterSystolic;
		bsn::filters::MovingAverage filterDiastolic;
};

#endif 