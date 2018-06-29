#ifndef DATAGENERATOR_HPP
#define DATAGENERATOR_HPP

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "bsn/data/RawData.h"
#include "bsn/statistics/VitalStatistics.h"

using namespace odcore::base::module;
using namespace bsn::data;
using namespace bsn::statistics;

class DataCollector : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
        DataCollector(const DataCollector & /*obj*/);
        DataCollector &operator=(const DataCollector & /*obj*/);
        virtual void setUp();
        virtual void tearDown();

    public:
        /**
         * Construtor e destrutor da classe.
        */
        DataCollector(const int32_t &argc, char **argv);
        virtual ~DataCollector();

        /**
         * Método efetivamente executado.
        */
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        double generated_data;
};

#endif 