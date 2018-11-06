#ifndef DataSender_HPP
#define DataSender_HPP

#include <iostream>
#include <string>
#include <random>
#include <unistd.h>
#include <algorithm>
#include <vector>

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "bsn/data/FilteredData.h"
#include "bsn/communication/TCPSend.hpp"
#include "bsn/time/TimeData.hpp"
#include "bsn/data/ResourceUpdate.h"
#include "bsn/data/SensorStatusInfo.h"
#include "bsn/configuration/SensorConfiguration.hpp"
#include "bsn/operation/Operation.hpp"

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
        std::map<std::string, std::map<std::string, bsn::range::Range>> configs_map;
        std::vector<bsn::configuration::SensorConfiguration> configs_vet;
};

#endif 