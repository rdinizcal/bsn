/*
 * Módulo do Nó Sensor
 * 
 * @author Ricardo Diniz Caldas
 * @version v1.0
 */

#include "SensorNodeModule.h"

SensorNodeModule::SensorNodeModule(const int32_t &argc, char **argv) : TimeTriggeredConferenceClientModule(argc, argv, "sensornode"),
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
void SensorNodeModule::setUp(){
    m_sensor_type = m_id + 1;              // configuração do tipo de sensor
    clock_gettime(CLOCK_REALTIME, &m_ref); // referência para medidas de tempo
}

// DESTRUIÇÃO
void SensorNodeModule::tearDown(){
    delete &persist;
}

void SensorNodeModule::sendSensorData(SensorData sensordata){
    Container container(sensordata);
    getConference().send(container);
    CLOG1 << sensordata.toString() << " sent at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
}

template <typename Out>
void split(const string &s, char delim, Out result){
    //Função de dividir uma string a partir de um char delimitador.
    //algoritmo retirado do stack overflow
    //link: https://stackoverflow.com/questions/236129/most-elegant-way-to-split-a-string
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim))
    {
        *(result++) = item;
    }
}

vector<string> split(const string &s, char delim){
    //retorna um vetor com as strings delimitadas.
    //algoritmo retirado do stack overflow
    //referencia: https://stackoverflow.com/questions/236129/most-elegant-way-to-split-a-string
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

// CORPO
odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorNodeModule::body(){

    timespec ts; // timestamp do processador

    srand(time(NULL));        // raíz da função de randomização
    bool exe;                 // variável do atuador
    int cycles = 0;           // contador de ciclos desde a ultima execução
    data_receiver connection; //conexão com o sensor

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING){
        cycles++;
        exe = controllerFSM(cycles, m_status); // para execução com controlador
        //exe = true;                            // para execuçao sem controlador

        if (exe){
            /*GERAR DADOS*/

            string packages = connection.receive();
            // Recebe vários pacotes concatenados com *
            // Organiza em um vetor
            auto packages_vector = split(packages, '*');

            for (int j = 0; j < packages_vector.size(); ++j) {
                string package = packages_vector[j];
                auto data = split(package, '-');
                // Recebe somente o dado enviado
                int id = stoi(data[0]);
                string categorized_data = data[1];                

                /*CAPTURAR INSTANTE DO PROCESSADOR*/
                clock_gettime(CLOCK_REALTIME, &ts);

                /*ANALISAR DADOS*/
                m_status = statusAnalysis(categorized_data, m_status, m_data_queue);

                /*ENVIAR ESTADO*/
                SensorNodeModule::sendSensorData(SensorData(id, id + 1, m_status, ts));
                persist.persist_packages_sent(id, m_status);
                cycles = 0;
                cout << "Actual status: " << m_status << " | Data sampled: " << categorized_data << " at " << TimeStamp().getYYYYMMDD_HHMMSS() << endl;
                persist.persist_sensor_status(ts, categorized_data, m_status, m_ref, TimeStamp().getYYYYMMDD_HHMMSS());
            }
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}