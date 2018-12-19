#ifndef DATACOLLECTORMODULE_HPP
#define DATACOLLECTORMODULE_HPP

#include <iostream>
#include <string>
#include <random>
#include <tuple>
#include <unistd.h>
#include <sys/time.h>
#include <map>

#include "opendavinci/odcore/base/Thread.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/msg/data/RawData.h"
#include "bsn/range/Range.hpp"
#include "bsn/time/TimeData.hpp"
#include "bsn/generator/Markov.hpp"
#include "bsn/operation/Operation.hpp"
#include "bsn/msg/data/ResourceUpdate.h"

class DataCollectorModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    
	private:
      	DataCollectorModule(const DataCollectorModule & /*obj*/);
    	DataCollectorModule &operator=(const DataCollectorModule & /*obj*/);
    	virtual void setUp();
    	virtual void tearDown();

		void updateParameters(const std::map<std::string, double> &_params);

  	public:
    	DataCollectorModule(const int32_t &argc, char **argv);
    	virtual ~DataCollectorModule();

		// Retorna os parametros necessários à distribuição normal
		std::tuple<double,double> get_normal_params(std::string);
    	
    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	    odcore::base::FIFOQueue buffer;

		bool active;
		std::map<std::string,double> params;

    	double mGeneratedData;
		timespec timeRef;
		std::array<float, 25> markovTransitions;
		std::array<bsn::range::Range,5> rangesArray;
};

#endif 