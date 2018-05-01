/*
 * Módulo do Nó Sensor
 * 
 * @author Ricardo Diniz Caldas
 * @version v1.0
 */

#include "BodyHubModule.h"

using namespace odcore::data::dmcp;

BodyHubModule::BodyHubModule(const int32_t &argc, char **argv) : TimeTriggeredConferenceClientModule(argc, argv, "bodyhub"), bodyhub(), bhPersist() {
    m_id = getIdentifier();
}

BodyHubModule::~BodyHubModule() {}

// CONFIGURAÇÃO
void BodyHubModule::setUp() {
    addDataStoreFor(SensorData::ID(), m_buffer); // "Avisa" ao buffer que vai receber dados do tipo SensorData
    clock_gettime(CLOCK_REALTIME, &m_ref); // referência para medidas de tempo  
}

// DESTRUIÇÃO
void BodyHubModule::tearDown() {}

// CORPO
ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 

    timespec ts; // timestamp

    bool is_emergency = false; // variável booleana utilizada para avisar sobre estados de emergencia
    uint32_t sensor_id = 0;    // varável utilizada para capturar e persistir o id do sensor que enviou os dados

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == ModuleStateMessage::RUNNING) {
        
        while(!m_buffer.isEmpty()){

            clock_gettime(CLOCK_REALTIME, &ts); //captura novo timestamp do processador para imprimir no registro

            // CONSOME DADO
            Container container = m_buffer.leave();

            if (container.getDataType() == SensorData::ID()) {

                // ATUALIZA ESTADO DO PACIENTE
                bodyhub.updateHealthStatus(container.getData<SensorData>());
                sensor_id= container.getData<SensorData>().getSensorID();

                // DETECTA EMERGÊNCIA
                is_emergency=(container.getData<SensorData>().getSensorStatus() == "alto")?true:false;
                CLOG1<<"Emergencia?"<<is_emergency<<endl;

                // PERSISTE
                bhPersist.persistHealthStatus(sensor_id, container.getData<SensorData>().getSentTimespec(), ts, m_ref, bodyhub.getSensorReg(), bodyhub.getHealthStatus());

                bodyhub.callPersist(container);
            }

            //imprime dados atuais
            bodyhub.printHealthStatus();
        }            
        
    }
    
    return ModuleExitCodeMessage::OKAY;
}
