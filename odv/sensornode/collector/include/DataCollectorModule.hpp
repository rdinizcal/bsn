#ifndef DATAGENERATORMODULE_HPP
#define DATAGENERATORMODULE_HPP

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>
#include <sys/time.h>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/Thread.h"

#include "bsn/data/ThermometerRawData.h"
#include "bsn/data/ECGRawData.h"
#include "bsn/data/OximeterRawData.h"
#include "bsn/data/DiastolicRawData.h"
#include "bsn/data/SystolicRawData.h"
#include "bsn/generator/DataGenerator.h"
#include "bsn/time/TimeData.hpp"

class DataCollectorModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
      	DataCollectorModule(const DataCollectorModule & /*obj*/);
    	DataCollectorModule &operator=(const DataCollectorModule & /*obj*/);
    	virtual void setUp();
    	virtual void tearDown();

  public:
    	/**
     	* Construtor e destrutor da classe.
     	*/
    	DataCollectorModule(const int32_t &argc, char **argv);
    	virtual ~DataCollectorModule();

    	/**
      	* Método efetivamente executado.
      	*/
    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	  	u_int32_t mSensor_id;
    	double mGeneratedData;
		timespec timeRef;
};

#endif 