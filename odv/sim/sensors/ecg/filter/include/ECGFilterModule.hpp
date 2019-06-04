#ifndef ECG_FILTER_MODULE_HPP
#define ECG_FILTER_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/filters/MovingAverage.hpp"

#include "bsn/msg/data/SensorData.h"
#include "bsn/msg/taskMsg/ECG/ECGCollectTaskMsg.hpp"
#include "bsn/msg/taskMsg/ECG/ECGFilterTaskMsg.hpp"

#include "bsn/msg/MessageQueueCodes.hpp"
#include "bsn/sensorfault/RandomSensorFault.hpp"

class ECGFilterModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	ECGFilterModule(const ECGFilterModule&);
    	ECGFilterModule &operator=(const ECGFilterModule&);

    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	ECGFilterModule(const int32_t &argc, char **argv);
    	virtual ~ECGFilterModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

		bool Oraculo(double);
		double noise(void);

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;
		bool active;
		std::map<std::string,double> params;
		bsn::filters::MovingAverage filter;
		bsn::sensorfault::RandomSensorFault falhaRand;
};

#endif 