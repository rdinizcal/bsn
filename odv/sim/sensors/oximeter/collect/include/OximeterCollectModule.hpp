#ifndef OXIMETER_COLLECT_MODULE_HPP
#define OXIMETER_COLLECT_MODULE_HPP

#include <fstream>
#include <chrono>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/range/Range.hpp"
#include "bsn/generator/Markov.hpp"
#include "bsn/operation/Operation.hpp"

#include "bsn/msg/control/OximeterControlCommand.hpp"
#include "bsn/msg/taskMsg/ECG/OximeterCollectTaskMsg.hpp"

#include "bsn/msg/MessageQueueCodes.hpp"


class OximeterCollectModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	OximeterCollectModule(const OximeterCollectModule &);
    	OximeterCollectModule &operator=(const OximeterCollect
	Module &);

    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	OximeterCollectModule(const int32_t &argc, char **argv);
    	virtual ~OximeterCollectModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
		std::string type;
		bool active;
		std::map<std::string,double> params;
		bsn::generator::Markov markov;

};

#endif 