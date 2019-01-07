#ifndef OXIMETER_MODULE_HPP
#define OXIMETER_MODULE_HPP

#include <fstream>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/generator/Markov.hpp"
#include "bsn/filters/MovingAverage.hpp"
#include "bsn/operation/Operation.hpp"
#include "bsn/configuration/SensorConfiguration.hpp"

#include "bsn/msg/data/SensorData.h"
#include "bsn/msg/info/TaskInfo.hpp"
#include "bsn/msg/info/ContextInfo.hpp"
#include "bsn/msg/control/OximeterControlCommand.hpp"

class OximeterModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	OximeterModule(const OximeterModule &);
    	OximeterModule &operator=(const OximeterModule &);

    	virtual void setUp();
    	virtual void tearDown();

		void sendTaskInfo(const std::string &/*task_id*/, const double &/*cost*/, const double &/*reliability*/);

  	public:
    	OximeterModule(const int32_t &argc, char **argv);
    	virtual ~OximeterModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		
		std::string type;
		double battery;
		bool available;
		double accuracy;

		bool active;
		std::map<std::string,double> params;

		bsn::generator::Markov markov;
		bsn::filters::MovingAverage filter;
		bsn::configuration::SensorConfiguration sensorConfig;

		int persist;
		std::string path;
		std::ofstream fp;
};

#endif 