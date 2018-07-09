#ifndef DataSender_HPP
#define DataSender_HPP

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "bsn/data/RawData.h"
#include "bsn/communication/TCPSend.hpp"

using namespace odcore::base;
using namespace odcore::base::module;
using namespace bsn::data;

class DataSender : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
        DataSender(const DataSender & /*obj*/);
        DataSender &operator=(const DataSender & /*obj*/);
        virtual void setUp();
        virtual void tearDown();

    public:
        /**
         * Construtor e destrutor da classe.
        */
        DataSender(const int32_t &argc, char **argv);
        virtual ~DataSender();

        /**
         * Método efetivamente executado.
        */
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
       // RawData received_data;
        FIFOQueue m_buffer;        
};

#endif 