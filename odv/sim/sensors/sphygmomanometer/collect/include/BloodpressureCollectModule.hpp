#ifndef BLOODPRESSURE_COLLECT_MODULE_HPP
#define BLOODPRESSURE_COLLECT_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/operation/Operation.hpp"
#include "bsn/configuration/SensorConfiguration.hpp"

#include "bsn/msg/control/BloodpressureControlCommand.hpp"

#include "bsn/msg/taskMsg/ECG/BloodpressureCollectTaskMsg.hpp"
#include "bsn/msg/MessageQueueCodes.hpp"


class BloodpressureCollectModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	BloodpressureCollectModule(const BloodpressureCollectModule &);
    	BloodpressureCollectModule &operator=(const BloodpressureCollectModule &);
    	
		virtual void setUp();
    	virtual void tearDown();

  	public:
    	BloodpressureCollectModule(const int32_t &argc, char **argv);
    	virtual ~BloodpressureCollectModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		
		std::string type;
		bool available;

		bool active;
		std::map<std::string,double> params;

		
		bsn::configuration::SensorConfiguration sensorConfigSystolic;
		bsn::configuration::SensorConfiguration sensorConfigDiastolic;
};

#endif 