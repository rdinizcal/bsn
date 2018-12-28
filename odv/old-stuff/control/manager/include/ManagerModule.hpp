#ifndef MANAGER_MODULE_HPP
#define MANAGER_MODULE_HPP

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

class ManagerModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
      	ManagerModule(const ManagerModule & /*obj*/);
    	ManagerModule &operator=(const ManagerModule & /*obj*/);
    	virtual void setUp();
    	virtual void tearDown();

  public:
    	ManagerModule(const int32_t &argc, char **argv);
    	virtual ~ManagerModule();

    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:

};

#endif 