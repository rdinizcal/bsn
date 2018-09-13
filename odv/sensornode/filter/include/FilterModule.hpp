#ifndef FILTER_MODULE_HPP
#define FILTER_MODULE_HPP

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"
#include "bsn/data/FilteredData.h"
#include "bsn/data/ConvertedData.h"
#include "bsn/filters/MovingAverage.hpp"
#include "bsn/time/TimeData.hpp"

class FilterModule : public odcore::base::module::TimeTriggeredConferenceClientModule {
    private:
        FilterModule(const FilterModule & /*obj*/);
        FilterModule &operator=(const FilterModule & /*obj*/);
        virtual void setUp();
        virtual void tearDown();

    public:
        /**
         * Construtor e destrutor da classe.
        */
        FilterModule(const int32_t &argc, char **argv);
        virtual ~FilterModule();

        /**
         * Método efetivamente executado.
        */
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        odcore::base::FIFOQueue data_buffer;
};

#endif 