#ifndef THERM_COLLECT_MODULE_HPP
#define THERM_COLLECT_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/operation/Operation.hpp"
#include "bsn/generator/Markov.hpp"

#include "bsn/msg/data/SensorData.h"

#include "bsn/msg/taskMsg/ECG/ThermCollectTaskMsg.hpp"

#include "bsn/msg/MessageQueueCodes.hpp"

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
		bsn::generator::Markov markov;
};

#endif 