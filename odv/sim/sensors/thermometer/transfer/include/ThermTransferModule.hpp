#ifndef THERM_TRANSFER_MODULE_HPP
#define THERM_TRANSFER_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/operation/Operation.hpp"
#include "bsn/configuration/SensorConfiguration.hpp"

#include "bsn/msg/data/SensorData.h"

class ThermTransferModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	ThermTransferModule(const ThermTransferModule &);
    	ThermTransferModule &operator=(const ThermTransferModule &);
		
    	virtual void setUp();
    	virtual void tearDown();
		
  	public:
    	ThermTransferModule(const int32_t &argc, char **argv);
    	virtual ~ThermTransferModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;
		bool active;
		std::map<std::string,double> params;
		bsn::configuration::SensorConfiguration sensorConfig;
};

#endif 