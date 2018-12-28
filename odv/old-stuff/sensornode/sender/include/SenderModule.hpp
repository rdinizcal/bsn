#ifndef SENDER_MODULE_HPP
#define SENDER_MODULE_HPP

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include <stdint.h>

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "bsn/msg/data/FilteredData.h"
#include "bsn/msg/data/SensorStatusInfo.h"
#include "bsn/msg/data/ResourceUpdate.h"
#include "bsn/msg/control/SenderModuleControlCommand.hpp"
#include "bsn/communication/TCPSend.hpp"
#include "bsn/time/TimeData.hpp"
#include "bsn/configuration/SensorConfiguration.hpp"
#include "bsn/operation/Operation.hpp"

class SenderModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
        SenderModule(const SenderModule & /*obj*/);
        SenderModule &operator=(const SenderModule & /*obj*/);
        virtual void setUp();
        virtual void tearDown();

    public:
        /**
         * Construtor e destrutor da classe.
        */
        SenderModule(const int32_t &argc, char **argv);
        virtual ~SenderModule();

        /**
         * Método efetivamente executado.
        */
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        odcore::base::FIFOQueue buffer;

        bool active;
		std::map<std::string,double> params;

        std::string ip;
        std::map<std::string, std::map<std::string, bsn::range::Range>> configsMap;
        std::vector<bsn::configuration::SensorConfiguration> configsVet;
};

#endif 