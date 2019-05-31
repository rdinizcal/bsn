#ifndef ECG_TRANSFER_MODULE_HPP
#define ECG_TRANSFER_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/operation/Operation.hpp"
#include "bsn/configuration/SensorConfiguration.hpp"

#include "bsn/msg/data/SensorData.h"
#include "bsn/msg/MessageQueueCodes.hpp"
#include "bsn/msg/taskMsg/ECG/ECGTransferTaskMsg.hpp"

class ECGTransferModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	ECGTransferModule(const ECGTransferModule &);
    	ECGTransferModule &operator=(const ECGTransferModule &);

    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	ECGTransferModule(const int32_t &argc, char **argv);
    	virtual ~ECGTransferModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;
		bool active;
		std::map<std::string,double> params;
		bsn::configuration::SensorConfiguration sensorConfig;
};

#endif 