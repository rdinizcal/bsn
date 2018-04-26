/*
 * Módulo do Nó Sensor
 * 
 * @author Ricardo Diniz Caldas
 * @version v1.0
 */

#include "SensorNodeModule.h"

SensorNodeModule::SensorNodeModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "sensornode"),
    m_id(getIdentifier()),
    m_sensor_type(),
    m_status("baixo"),
    m_data_queue(),
    m_status_log(),
    packages_file(),
    persist(m_id),
    m_ref() {}

SensorNodeModule::~SensorNodeModule() {}

// CONFIGURAÇÃO
void SensorNodeModule::setUp() {
    m_sensor_type = m_id+1; // configuração do tipo de sensor
    clock_gettime(CLOCK_REALTIME, &m_ref); // referência para medidas de tempo 
}

// DESTRUIÇÃO
void SensorNodeModule::tearDown() {}

void SensorNodeModule::sendSensorData(SensorData sensordata){
    Container container(sensordata);
    getConference().send(container);
    CLOG1 << sensordata.toString() << " sent at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
}

// CORPO
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorNodeModule::body() {

    timespec ts;        // timestamp do processador

    srand(time(NULL));  // raíz da função de randomização
    bool exe;           // variável do atuador
    int cycles = 0;     // contador de ciclos desde a ultima execução
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {        
        cycles++;
        exe = controllerFSM(cycles,m_status);    // para execução com controlador
        //exe = true;                            // para execuçao sem controlador

        if(exe){
            /*GERAR DADOS*/
            string categorized_data = generateData(m_status);

            /*CAPTURAR INSTANTE DO PROCESSADOR*/
            clock_gettime(CLOCK_REALTIME, &ts);

            /*ANALISAR DADOS*/
            m_status = statusAnalysis(categorized_data, m_status, m_data_queue);

            /*ENVIAR ESTADO*/
            SensorNodeModule::sendSensorData(SensorData(m_id, m_sensor_type, m_status, ts));
            persist.persist_packages_sent(m_id, m_status);
            cycles = 0;
            cout << "Actual status: " << m_status << " | Data sampled: " << categorized_data << " at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
            persist.persist_sensor_status(ts, categorized_data, m_status , m_ref, TimeStamp().getYYYYMMDD_HHMMSS());
        }
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}