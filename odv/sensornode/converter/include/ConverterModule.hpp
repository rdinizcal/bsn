#ifndef ConverterModule_HPP
#define ConverterModule_HPP

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"
#include "bsn/scales/ScaleConverter.hpp"
#include "bsn/data/ConvertedData.h"
#include "bsn/data/RawData.h"

class ConverterModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
        ConverterModule(const ConverterModule & /*obj*/);
        ConverterModule &operator=(const ConverterModule & /*obj*/);
        virtual void setUp();
        virtual void tearDown();

    public:
        /**
         * Construtor e destrutor da classe.
        */
        ConverterModule(const int32_t &argc, char **argv);
        virtual ~ConverterModule();

        /**
         * Método efetivamente executado.
        */
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        uint32_t mSensor_id;
        odcore::base::FIFOQueue rawdata_buffer;
};

#endif 