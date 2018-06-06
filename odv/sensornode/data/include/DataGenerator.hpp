#ifndef DATAGENERATOR_HPP
#define DATAGENERATOR_HPP

#include <opendavinci/odcore/base/TimeTriggeredConferenceClientModule.h>
#include "bsn/data/SensorData.h"

class DataGenerator : public odcore::base::module::TimeTriggeredConferenceClientModule{
    private:
        DataGenerator(const DataGenerator & /*obj*/);
        DataGenerator &operator=(const DataGenerator & /*obj*/);
        virtual void setUp();
        virtual void tearDown();

    public:
        /**
         * Construtor e destrutor da classe.
        */
        DataGenerator(const int32_t &argc, char **argv);
        virtual ~DataGenerator();

        /**
         * Método efetivamente executado.
        */
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        uint32_t m_id; // identificador
        int32_t m_sensor_type;

};

#endif 