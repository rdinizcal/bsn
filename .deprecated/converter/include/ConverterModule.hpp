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
#include "bsn/data/ThermometerRawData.h"
#include "bsn/data/ECGRawData.h"
#include "bsn/data/OximeterRawData.h"
#include "bsn/data/SystolicRawData.h"
#include "bsn/data/DiastolicRawData.h"
#include "bsn/time/TimeData.hpp"

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
        odcore::base::FIFOQueue rawdata_buffer;
        std::string sensorType;
        bsn::scales::ScaleConverter converter;
};

#endif 