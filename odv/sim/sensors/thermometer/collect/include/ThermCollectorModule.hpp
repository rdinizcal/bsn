#ifndef THERM_COLLECT_MODULE_HPP
#define THERM_COLLECT_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/filters/MovingAverage.hpp"

#include "bsn/msg/data/SensorData.h"

class ThermCollectModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	ThermCollectModule(const ThermCollectModule &);
    	ThermCollectModule &operator=(const ThermCollectModule &);
		
    	virtual void setUp();
    	virtual void tearDown();
		
  	public:
    	ThermCollectModule(const int32_t &argc, char **argv);
    	virtual ~ThermCollectModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;
		bool active;
		std::map<std::string,double> params;
		bsn::filters::MovingAverage filter;
};

#endif 