#ifndef ECG_COLLECT_MODULE_HPP
#define ECG_COLLECT_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/generator/Markov.hpp"
#include "bsn/operation/Operation.hpp"

#include "bsn/msg/data/SensorData.h"

class ECGCollectModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	ECGCollectModule(const ECGCollectModule &);
    	ECGCollectModule &operator=(const ECGCollectModule &);

    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	ECGCollectModule(const int32_t &argc, char **argv);
    	virtual ~ECGCollectModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;
		bool active;
		std::map<std::string,double> params;
		bsn::generator::Markov markov;
};

#endif 