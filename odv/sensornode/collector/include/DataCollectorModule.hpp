#ifndef DATAGENERATORMODULE_HPP
#define DATAGENERATORMODULE_HPP

#include <iostream>
#include <string>
#include <random>
#include <tuple>
#include <unistd.h>
#include <sys/time.h>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/Thread.h"

#include "bsn/data/RawData.h"
#include "bsn/generator/DataGenerator.h"
#include "bsn/time/TimeData.hpp"
#include "bsn/operation/Operation.hpp"
#include "bsn/range/Range.hpp"

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

		// Retorna os parametros necessários à distribuição normal
		std::tuple<double,double> get_normal_params(std::string);
    	/**
      	* Método efetivamente executado.
      	*/
    	odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

  	private:
	  	u_int32_t mSensor_id;
    	double mGeneratedData;
		timespec timeRef;
		std::array<int, 9> markov_transitions;
		std::vector<bsn::range::Range> ranges_vector;
};

#endif 