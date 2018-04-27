/*
 * Módulo do Nó Sensor
 * 
 * @author Ricardo Diniz Caldas
 * @version v1.0
 */

#ifndef SENSORNODE_MODULE_H_
#define SENSORNODE_MODULE_H_

#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"
#include "openbasn/data/SensorData.h"
#include "../../VitalStatistics/include/VitalStatistics.h"
#include "../../persistence/include/persistenceModule.h"
#include "../../communications/include/data_receiver.hpp"

#include <deque>
#include <fstream>
#include <sys/time.h>
#include <iostream>
#include <string>

using namespace std;
using namespace odcore::base::module;
using namespace openbasn::data;

class SensorNodeModule : public TimeTriggeredConferenceClientModule {

    
    // Boas práticas pelo manual do OpenDaVINCI.
    private:     
        SensorNodeModule(const SensorNodeModule &/*obj*/);
        SensorNodeModule& operator=(const SensorNodeModule &/*obj*/);
    
    
    // Declaração dos métodos abstratos da classe pai.
    public:
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode body();

    private:
        virtual void setUp();
        virtual void tearDown();
    
    // Construtor e Destrutor da classe.
    public:
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
        int32_t m_sensor_type;      // tipo do sensor associado
        string m_status;            // estado atual
        deque<string> m_data_queue; // fila de dados
        ofstream m_status_log;      // arquivo de log
        ofstream packages_file;     // log de pacotes enviados
        timespec m_ref;             // referencia temporal
        SensorPersistence persist;
};

#endif