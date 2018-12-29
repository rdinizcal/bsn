#ifndef CENTRALHUB_MODULE_HPP
#define CENTRALHUB_MODULE_HPP

/*
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <list>
#include <fstream>
#include <unistd.h>
*/

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/configuration/SensorConfiguration.hpp"

class CentralhubModule : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
        CentralhubModule(const CentralhubModule & /*obj*/);
        CentralhubModule &operator=(const CentralhubModule & /*obj*/);
        virtual void setUp();
        virtual void tearDown();        
        void print_packs();

    public:
        CentralhubModule(const int32_t &argc, char **argv);
        virtual ~CentralhubModule();

        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        odcore::base::FIFOQueue buffer;
};

#endif 