#ifndef DATA_PROCESSOR_HPP
#define DATA_PROCESSOR_HPP

#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <list>
#include <fstream>
#include <unistd.h>

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "opendavinci/odcore/base/FIFOQueue.h"

#include "bsn/configuration/sensor_configuration.hpp"
#include "bsn/data/SensorData.h"
#include "bsn/operation/Operation.hpp"
#include "bsn/range/Range.hpp"

class DataProcessor : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
        DataProcessor(const DataProcessor & /*obj*/);
        DataProcessor &operator=(const DataProcessor & /*obj*/);
        virtual void setUp();
        virtual void tearDown();
        void data_fuse();
        void print_packs();
        // Retorna true se existe pelo menos um pacote para cada sensor
        bool available_to_process();
        // Retorna o id do sensor correspondente
        int get_sensor_id(std::string);
        double get_value(std::string);

    public:
        /**
         * Construtor e destrutor da classe.
        */
        DataProcessor(const int32_t &argc, char **argv);
        virtual ~DataProcessor();

        /**
         * Método efetivamente executado.
        */
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        std::vector<std::list<double>> packets_received;
        std::vector<sensor_configuration> configurations;
        odcore::base::FIFOQueue data_buffer;
};

#endif 