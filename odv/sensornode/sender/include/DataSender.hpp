#ifndef DataSender_HPP
#define DataSender_HPP

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "bsn/data/FilteredData.h"
#include "bsn/communication/TCPSend.hpp"
#include "bsn/time/TimeData.hpp"
#include "bsn/data/ResourceUpdate.h"

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
        odcore::base::FIFOQueue m_buffer;
        std::string ip;
};

#endif 