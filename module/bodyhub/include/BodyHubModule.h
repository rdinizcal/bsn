/*
 * Módulo do Nó Sensor
 * 
 * @author Ricardo Diniz Caldas
 * @version v1.0
 */

#ifndef BODYHUB_MODULE_H
#define BODYHUB_MODULE_H

#include "opendavinci/odcore/base/FIFOQueue.h"
#include "opendavinci/odcore/base/module/TimeTriggeredConferenceClientModule.h"

#include "../../../libbsn/include/data/SensorData.h"
#include "../../../libbsn/include/entity/BodyHubEntity.h"
#include "../../../libbsn/include/persistence/PersistenceModule.h"

#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;
using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data::dmcp;
using namespace bsn::data;
using namespace bsn::entity;
using namespace bsn::persistence;

class BodyHubModule : public TimeTriggeredConferenceClientModule {

    // Boas práticas pelo manual do OpenDaVINCI.
    private:      
        BodyHubModule(const BodyHubModule &/*obj*/);
        BodyHubModule& operator=(const BodyHubModule &/*obj*/); 

        virtual void setUp();
        virtual void tearDown();

    // Declaração dos métodos abstratos da classe pai.
    public:
        ModuleExitCodeMessage::ModuleExitCode body();
    
        // Construtor e Destrutor da classe.
        BodyHubModule(const int32_t &argc, char **argv);
        virtual ~BodyHubModule();
        
    // Atributos da classe
    private:
        uint32_t m_id;          // identificador
        FIFOQueue m_buffer;     // fila de containers
        ofstream m_status_log;  // arquivo para log
        timespec m_ref;         // referencia temporal
        ofstream packages_file; // arquivo para escrita dos pacotes recebidos
        BodyHub bodyhub;
        BodyHubPersistence bhPersist;
};

#endif