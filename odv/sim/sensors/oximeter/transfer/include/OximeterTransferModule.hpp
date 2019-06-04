#ifndef OXIMETER_TRANSFER_MODULE_HPP
#define OXIMETER_TRANSFER_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/operation/Operation.hpp"
#include "bsn/configuration/SensorConfiguration.hpp"

#include "bsn/msg/data/SensorData.h"

#include "bsn/msg/taskMsg/oximeter/OximeterFilterTaskMsg.hpp"
#include "bsn/msg/taskMsg/oximeter/OximeterTransferTaskMsg.hpp"

#include "bsn/msg/MessageQueueCodes.hpp"
#include "bsn/sensorfault/RandomSensorFault.hpp"


class OximeterTransferModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	OximeterTransferModule(const OximeterTransferModule &);
    	OximeterTransferModule &operator=(const OximeterTransferModule &);

    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	OximeterTransferModule(const int32_t &argc, char **argv);
    	virtual ~OximeterTransferModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;
		bool active;
		std::map<std::string,double> params;
		bsn::configuration::SensorConfiguration sensorConfig;
		bsn::sensorfault::RandomSensorFault falhaRand;
};

#endif 