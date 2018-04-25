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
    m_ref() {}

SensorNodeModule::~SensorNodeModule() {}

// CONFIGURAÇÃO
void SensorNodeModule::setUp() {
    m_sensor_type = m_id+1; // configuração do tipo de sensor

    clock_gettime(CLOCK_REALTIME, &m_ref); // referência para medidas de tempo 
    packages_file.open("output/packages_sent.txt");
    string path = "output/";
    string filename = "sensornode" + to_string(m_id);
    m_status_log.open(path + filename + "_status_log.csv");
    m_status_log << "Elapsed Time(s), Sensor Status, Time Since Last (s)\n";
}

// DESTRUIÇÃO
void SensorNodeModule::tearDown() {
    packages_file.close();
    m_status_log.close();
}

void SensorNodeModule::sendSensorData(SensorData sensordata){
    Container container(sensordata);
    getConference().send(container);
    CLOG1 << sensordata.toString() << " sent at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
}

timespec SensorNodeModule::elapsedTime(timespec &now, timespec &ref) {

    timespec diff;

    if ((now.tv_nsec - ref.tv_nsec) < 0) {
        diff.tv_sec = now.tv_sec - ref.tv_sec - 1;
        diff.tv_nsec = now.tv_nsec - ref.tv_nsec + 1000000000L;
    } else {
        diff.tv_sec = now.tv_sec - ref.tv_sec;
        diff.tv_nsec = now.tv_nsec - ref.tv_nsec;
    }

    return diff;
}

void SensorNodeModule::persist_sensor_status(timespec ts, string categorized_data){
    cout << "Actual status: " << m_status << " | Data sampled: " << categorized_data << " at " << TimeStamp().getYYYYMMDD_HHMMSSms() << endl;
    timespec t_esy = elapsedTime(ts, m_ref);
    m_status_log << (double)((t_esy.tv_sec) + (t_esy.tv_nsec/1E9)) << ",";
    m_status_log << ((m_status=="baixo")?1:(m_status=="moderado")?2:3) << ",";
    m_status_log << " " << "\n";
}

void SensorNodeModule::persist_packages_sent(uint32_t id, string status){
    packages_file << id << ' ' << status <<  "\n";
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
            persist_packages_sent(m_id,m_status);
            cycles = 0;    

            persist_sensor_status(ts,categorized_data);        
        }
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}