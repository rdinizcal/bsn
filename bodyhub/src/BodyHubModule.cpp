/*
 * Módulo do Nó Sensor
 * 
 * @author Ricardo Diniz Caldas
 * @version v1.0
 */

#include "BodyHubModule.h"

BodyHubModule::BodyHubModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bodyhub"),
    m_id(getIdentifier()),
    m_buffer(),
    m_health_status("-"),
    m_sensor(),
    m_status_log(),
    persist(),
    m_ref() {}

BodyHubModule::~BodyHubModule() {}


// CONFIGURAÇÃO
void BodyHubModule::setUp() {
    addDataStoreFor(873, m_buffer); // "Avisa" ao buffer que vai receber dados do tipo SensorNodeData
    clock_gettime(CLOCK_REALTIME, &m_ref); // referência para medidas de tempo  
}

// DESTRUIÇÃO
void BodyHubModule::tearDown() {}

void BodyHubModule::updateHealthStatus(SensorData sensordata){
    m_sensor[sensordata.getSensorType()-1] = sensordata.getSensorStatus();
    m_health_status = calculateHealthStatus(m_sensor);
}

void BodyHubModule::printHealthStatus(){
    cout << "----------------------------------------"<<endl;
    for(uint32_t i = 0; i < 3; i++){
        cout << ((i==0)?"Thermometer: ":(i==1)?"ECG: ":"Oximeter: ");
        cout << m_sensor[i] << endl;
    }
    cout << "Health Status: " << m_health_status << endl;
    cout << "----------------------------------------"<<endl;
}

void BodyHubModule::call_persist(Container container){    
    persist.persist_data_received(container.getData<SensorData>().getSensorID(),container.getData<SensorData>().getSensorStatus());
}

// CORPO
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 
    
    timespec ts; // timestamp

    bool is_emergency = false; // variável booleana utilizada para avisar sobre estados de emergencia
    uint32_t sensor_id = 0;    // varável utilizada para capturar e persistir o id do sensor que enviou os dados

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        while(!m_buffer.isEmpty()){

            clock_gettime(CLOCK_REALTIME, &ts); //captura novo timestamp do processador para imprimir no registro

            // CONSOME DADO
            Container container = m_buffer.leave();

            if (container.getDataType() == SensorData::ID()) {

                // ATUALIZA ESTADO DO PACIENTE
                BodyHubModule::updateHealthStatus(container.getData<SensorData>());
                sensor_id= container.getData<SensorData>().getSensorID();

                // DETECTA EMERGÊNCIA
                is_emergency=(container.getData<SensorData>().getSensorStatus() == "alto")?true:false;
                CLOG1<<"Emergencia?"<<is_emergency<<endl;

                // PERSISTE
                persist.persistHealthStatus(sensor_id, container.getData<SensorData>().getSentTimespec(), ts, m_ref, m_sensor, m_health_status);

                call_persist(container);
            }

            //imprime dados atuais
            BodyHubModule::printHealthStatus();
        }            
        
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
