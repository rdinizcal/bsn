/*
 * Módulo do Nó Sensor
 * 
 * @author Ricardo Diniz Caldas
 * @version v1.0
 */

#ifndef SENSORNODE_MODULE_H
#define SENSORNODE_MODULE_H

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

#include "bsn/data/SensorData.h"
#include "bsn/entity/SensorNodeEntity.h"
#include "bsn/persistence/PersistenceModule.h"
#include "bsn/statistics/VitalStatistics.h"
#include "../../../simulation/communications/include/data_receiver.hpp"

#include <deque>
#include <fstream>
#include <sys/time.h>
#include <iostream>
#include <string>

using namespace std;
using namespace odcore::base::module;
using namespace odcore::data::dmcp;
using namespace bsn::data;
using namespace bsn::entity;
using namespace bsn::persistence;
using namespace bsn::statistics;

class SensorNodeModule : public TimeTriggeredConferenceClientModule {

    
    // Boas práticas pelo manual do OpenDaVINCI.
    private:     
        SensorNodeModule(const SensorNodeModule &/*obj*/);
        SensorNodeModule& operator=(const SensorNodeModule &/*obj*/);
    
        virtual void setUp();
        virtual void tearDown();
    
    // Declaração dos métodos abstratos da classe pai.
    public:
        ModuleExitCodeMessage::ModuleExitCode body();

        // Construtor e Destrutor da classe.
        SensorNodeModule(const int32_t &argc, char **argv); 
        virtual ~SensorNodeModule(); 
    
    
        // Métodos privados do funcionamento interno da classe.            

        /**
         * Envio do dado analisado
         * 
         * @param mensagem com o dado gerado
         * @return 
         */
        void sendSensorData(SensorData /*sensordata*/);

        /**
         * Cálculo da diferença de tempo entre dois timespecs
         * (agora - referencia)
         * 
         * @param agora, referencia 
         * @return resultado da diferença
         */        
        timespec elapsedTime(timespec &/*now*/, timespec &/*ref*/);

        void persist_packages_sent(uint32_t /*id*/, string /*status*/);

        void persist_sensor_status(timespec /*ts*/, string /*categorized_data*/);
        

    // Atributos da classe
        uint32_t m_id;              // identificador
        deque<string> m_data_queue; // fila de dados
        ofstream m_status_log;      // arquivo de log
        ofstream packages_file;     // log de pacotes enviados
        timespec m_ref;             // referencia temporal
        SensorNode sensornode;
        SensorPersistence snPersist;
};

#endif