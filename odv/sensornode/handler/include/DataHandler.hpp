#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "bsn/data/RawData.h"

using namespace odcore::base;
using namespace odcore::base::module;
using namespace bsn::data;

class DataHandler : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
        DataHandler(const DataHandler & /*obj*/);
        DataHandler &operator=(const DataHandler & /*obj*/);
        virtual void setUp();
        virtual void tearDown();

    public:
        /**
         * Construtor e destrutor da classe.
        */
        DataHandler(const int32_t &argc, char **argv);
        virtual ~DataHandler();

        /**
         * Método efetivamente executado.
        */
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
       // RawData received_data;
        FIFOQueue m_buffer;
};

#endif 