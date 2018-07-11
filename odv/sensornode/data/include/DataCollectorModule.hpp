#ifndef DATAGENERATORMODULE_HPP
#define DATAGENERATORMODULE_HPP

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

#include "bsn/data/RawData.h"
#include "bsn/filters/average.hpp"
#include "bsn/entity/DataCollector.h"
#include "bsn/scales/ScaleConverter.hpp"

using namespace odcore::base::module;
using namespace bsn::data;
using namespace bsn::entity;

class DataCollectorModule : public odcore::base::module::TimeTriggeredConferenceClientModule
{
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
    double mGeneratedData;
};

#endif 