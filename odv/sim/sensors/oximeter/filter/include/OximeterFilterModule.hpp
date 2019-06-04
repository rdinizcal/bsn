#ifndef OXIMETER_FILTER_MODULE_HPP
#define OXIMETER_FILTER_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/filters/MovingAverage.hpp"

#include "bsn/msg/data/SensorData.h"

#include "bsn/msg/MessageQueueCodes.hpp"
#include "bsn/msg/taskMsg/oximeter/OximeterCollectTaskMsg.hpp"
#include "bsn/msg/taskMsg/oximeter/OximeterFilterTaskMsg.hpp"
#include "bsn/sensorfault/RandomSensorFault.hpp"

class OximeterFilterModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	OximeterFilterModule(const OximeterFilterModule &);
    	OximeterFilterModule &operator=(const OximeterFilterModule &);

    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	OximeterFilterModule(const int32_t &argc, char **argv);
    	virtual ~OximeterFilterModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;
		bool active;
		std::map<std::string,double> params;
		bsn::filters::MovingAverage filter;
		bsn::sensorfault::RandomSensorFault falhaRand;
};

#endif 