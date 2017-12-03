#include "SensorNodeModule.h"

using namespace std;
using namespace std::chrono;

using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;
using namespace odcore::data::dmcp;

using namespace openbasn::data;

SensorNodeModule::SensorNodeModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "sensornode"),
    m_id(getIdentifier()),
    m_sensor_type(),
    m_status("baixo"),
    m_data_queue(),
    /**************** CODIGO USADO PARA VALIDACAO DO PROTOTIPO ************************/
    m_status_log(),
    m_ref() 
    /**************** CODIGO USADO PARA VALIDACAO DO PROTOTIPO ************************/
    {}

    SensorNodeModule::~SensorNodeModule() {}

/*
 * CONFIGURAÇÃO
 * */
void SensorNodeModule::setUp() {
    //CONFIGURAÇÃO DO TIPO DO SENSOR
    m_sensor_type = m_id+1;

    /**************** CODIGO USADO PARA VALIDACAO DO PROTOTIPO ************************/
    //REFERÊNCIA PARA MEDIDAS DE TEMPO NO NÓ SENSOR
    clock_gettime(CLOCK_REALTIME, &m_ref);

    string path = "output/";
    string filename = "sensornode" + to_string(m_id);
    /* m_sensornode_log.open(path + filename + "_log.csv");
    m_sensornode_log << "Cycle, Status, Sampled Data, is Emergency?, Monitor(us), Analyze(us), Plan&Execute(us), Total(us), Timestamp\n"; */

    m_status_log.open(path + filename + "_status_log_1.csv");
    m_status_log << "Elapsed Time(s), Sensor Status, Time Since Last (s)\n";
    /**************** CODIGO USADO PARA VALIDACAO DO PROTOTIPO ************************/
}

/*
 * DESTRUIÇÃO
 * */
void SensorNodeModule::tearDown() {
}

/*
 * Máquina de estados finitos do controlador
 * Recebe o calor do contador de ciclos
 * retorna verdadeiro ou falso habilitando ou não a execução nó sensor no atual ciclo
 * */
bool SensorNodeModule::controllerFSM(int t){
    bool exe = false;
    
    if(m_status=="baixo"){
        exe = (t>=10)?true:false;
    } else if (m_status=="moderado") {
        exe = (t>=5)?true:false;
    } else if (m_status=="alto") {
        exe = (t>=1)?true:false;
    } 

    return exe;
}

/*
 * Automato probabilistico para geração de dados já classificados
 * */
string SensorNodeModule::generateData(string actual_status){

    string category;
    int p = (rand() % 100) + 1;

    if(actual_status == "baixo"){
        if(1 <= p && p <= 5) {
            category = "alto";
        } else if (5 < p && p <= 30) {
            category = "moderado";
        } else {
            category = actual_status;
        }
    } else if(actual_status == "moderado") {
        if(1 <= p && p <= 15) {
            category = "alto";
        } else if (15 < p && p <= 30) {
            category = "baixo";
        } else {
            category = actual_status;
        }
    } else if(actual_status == "alto"){
        if(1 <= p && p <= 5) {
            category = "baixo";
        } else if (5 < p && p <= 30) {
            category = "moderado";
        } else {
            category = actual_status;
        }
    } else {
        category = actual_status;
    }

    return category;
}

/*
 * Análise do estado do nó sensor
 * */
string SensorNodeModule::statusAnalysis(string categorized_data, string actual_status) {

    string new_status;
    int l=0, m=0, h=0;

    if(m_data_queue.size()>=5){
        m_data_queue.pop_front();
    }

    m_data_queue.push_back(categorized_data);

    for(uint32_t i = 0; i < m_data_queue.size(); i++) {
        if(m_data_queue[i]=="baixo"){
            l++;
        } else if(m_data_queue[i]=="moderado") {
            m++;
        } else if (m_data_queue[i]=="alto"){
            h++;
        }
    }

    if(l>=3){
        new_status = "baixo";
    } else if (m>=3) {
        new_status = "moderado";
    } else if (h>=3) {
        new_status = "alto";
    } else {
        new_status = actual_status;
    }

    return new_status;
}

/*
 * Envia dados do sensor empacotados pelo container
 * */
void SensorNodeModule::sendSensorData(SensorData sensordata){
    Container container(sensordata);
    getConference().send(container);
    CLOG1 << sensordata.toString() << " sent at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
}

/*
 * Calcula a diferença de tempo entre a timestamp do primeiro parametro com o segundo
 * */
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

/*
 * CORPO
 * */
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorNodeModule::body() {

    
    /**************** CODIGO USADO PARA VALIDACAO DO PROTOTIPO ************************/
    timespec ts;        // timestamp do processador
    /**************** CODIGO USADO PARA VALIDACAO DO PROTOTIPO ************************/

    srand(time(NULL));  // raíz da função de randomização
    bool exe;           // variável do atuador
    int cycles = 0;     // contador de ciclos desde a ultima execução
    
    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        cycles++;
        exe = controllerFSM(cycles);
        //exe = true;

        if(exe){
            /*GERAR DADOS*/
            string categorized_data = generateData(m_status);

            /*CAPTURAR INSTANTE DO PROCESSADOR*/
            clock_gettime(CLOCK_REALTIME, &ts);

            /*ANALISAR DADOS*/
            m_status = statusAnalysis(categorized_data, m_status);

            /*ENVIAR ESTADO*/
            SensorNodeModule::sendSensorData(SensorData(m_id, m_sensor_type, m_status, ts));

            cycles = 0;

            /**************** CODIGO USADO PARA VALIDACAO DO PROTOTIPO ************************/
            cout << "Actual status: " << m_status << " | Data sampled: " << categorized_data << " at " << TimeStamp().getYYYYMMDD_HHMMSSms() << endl;
            timespec t_esy = elapsedTime(ts, m_ref);
            m_status_log << (double)((t_esy.tv_sec) + (t_esy.tv_nsec/1E9)) << ",";
            m_status_log << ((m_status=="baixo")?1:(m_status=="moderado")?2:3) << ",";
            m_status_log << " " << "\n";
            /**************** CODIGO USADO PARA VALIDACAO DO PROTOTIPO ************************/
        }
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}