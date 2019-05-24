#ifndef THERM_COLLECTOR_MODULE_HPP
#define THERM_COLLECTOR_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/filters/MovingAverage.hpp"

#include "bsn/msg/data/SensorData.h"

class ThermCollectorModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	ThermCollectorModule(const ThermCollectorModule &);
    	ThermCollectorModule &operator=(const ThermCollectorModule &);
		
    	virtual void setUp();
    	virtual void tearDown();
		
  	public:
    	ThermCollectorModule(const int32_t &argc, char **argv);
    	virtual ~ThermCollectorModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;
		bool active;
		std::map<std::string,double> params;
		bsn::filters::MovingAverage filter;
};

#endif 