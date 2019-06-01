#ifndef THERM_FILTER_MODULE_HPP
#define THERM_FILTER_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/filters/MovingAverage.hpp"

#include "bsn/msg/data/SensorData.h"

#include "bsn/msg/taskMsg/thermometer/ThermometerCollectTaskMsg.hpp"
#include "bsn/msg/taskMsg/thermometer/ThermometerFilterTaskMsg.hpp"

#include "bsn/msg/MessageQueueCodes.hpp"

class ThermFilterModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	ThermFilterModule(const ThermFilterModule &);
    	ThermFilterModule &operator=(const ThermFilterModule &);
		
    	virtual void setUp();
    	virtual void tearDown();
		
  	public:
    	ThermFilterModule(const int32_t &argc, char **argv);
    	virtual ~ThermFilterModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;	
		bool active;
		std::map<std::string,double> params;
		bsn::filters::MovingAverage filter;
};

#endif 