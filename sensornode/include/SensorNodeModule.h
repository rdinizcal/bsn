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
    private:
        /**
         * Controlador da execução do nó sensor.
         * 
         * @param número de ciclos desde a última execução
         * @return booleano com autorização ou não da
         *         execução do módulo
         */
        bool controllerFSM(int /*cycles*/);

        /**
         * Gerador de dados classificados do sensor
         * 
         * @param estado atual do nó sensor
         * @return um novo estado
         */
        string generateData(string /*actual status*/);

        /**
         * Faz a análise do novo dado gerado para garantir 
         * confiabilidade dos dados gerados.
         * 
         * @param novo estado gerado na iteração, estado atual do nó sensor 
         * @return estado que será enviado pelo nó
         */
        string statusAnalysis(string /*categorized data*/, string /*actual status*/);

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

    // Atributos da classe
    private:
        uint32_t m_id; // identificador
        int32_t m_sensor_type; // tipo do sensor associado
        string m_status; // estado atual
        deque<string> m_data_queue; // fila de dados
        ofstream m_status_log; // arquivo de log
        timespec m_ref; // referencia temporal
};

#endif