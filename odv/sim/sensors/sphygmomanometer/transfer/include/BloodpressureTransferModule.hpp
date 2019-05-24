#ifndef BLOODPRESSURE_TRANSFER_MODULE_HPP
#define BLOODPRESSURE_TRANSFER_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/filters/MovingAverage.hpp"

#include "bsn/msg/data/SensorData.h"

class BloodpressureTransferModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	BloodpressureTransferModule(const BloodpressureTransferModule &);
    	BloodpressureTransferModule &operator=(const BloodpressureTransferModule &);
    	
		virtual void setUp();
    	virtual void tearDown();

  	public:
    	BloodpressureTransferModule(const int32_t &argc, char **argv);
    	virtual ~BloodpressureTransferModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;
		bool active;
		std::map<std::string,double> params;
		bsn::filters::MovingAverage filterSystolic;
		bsn::filters::MovingAverage filterDiastolic;
};

#endif 