#ifndef JOKER_MODULE_HPP
#define JOKER_MODULE_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/msg/data/SensorData.h"

class JokerModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	JokerModule(const JokerModule &);
    	JokerModule &operator=(const JokerModule &);
		
    	virtual void setUp();
    	virtual void tearDown();

  	public:
    	JokerModule(const int32_t &argc, char **argv);
    	virtual ~JokerModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;
};

#endif 