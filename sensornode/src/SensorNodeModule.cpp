#include "SensorNodeModule.h"

#include <sys/time.h>

using namespace std;
using namespace std::chrono;

using namespace odcore::base;
using namespace odcore::base::module;
using namespace odcore::data;
using namespace odcore::data::dmcp;

using namespace openbasn::data;
using namespace openbasn::model::sensor;


SensorNodeModule::SensorNodeModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "sensornode"),
    m_id(getIdentifier()),
    m_sensor_type(),
    m_status("low"),
    m_data_queue(),
    m_status_log(),
    m_ref() {}

    SensorNodeModule::~SensorNodeModule() {}

void SensorNodeModule::setUp() {
    m_sensor_type = m_id+1;

    clock_gettime(CLOCK_REALTIME, &m_ref);

    string path = "output/";
    string filename = "sensornode" + to_string(m_id);
    /* m_sensornode_log.open(path + filename + "_log.csv");
    m_sensornode_log << "Cycle, Status, Sampled Data, is Emergency?, Monitor(us), Analyze(us), Plan&Execute(us), Total(us), Timestamp\n"; */

    m_status_log.open(path + filename + "_status_log_1.csv");
    m_status_log << "Elapsed Time(s), Sensor Status, Time Since Last (s)\n";
}

void SensorNodeModule::tearDown() {
}

bool SensorNodeModule::controllerFSM(int t){
    bool exe = false;
    
    if(m_status=="low"){
        exe = (t>=10)?true:false;
    } else if (m_status=="moderate") {
        exe = (t>=5)?true:false;
    } else if (m_status=="high") {
        exe = (t>=1)?true:false;
    } 

    return exe;
}

string SensorNodeModule::generateData(string actual_status){

    string category;
    int p = (rand() % 100) + 1;

    if(actual_status == "low"){
        if(1 <= p && p <= 5) {
            category = "high";
        } else if (5 < p && p <= 30) {
            category = "moderate";
        } else {
            category = actual_status;
        }
    } else if(actual_status == "moderate") {
        if(1 <= p && p <= 15) {
            category = "high";
        } else if (15 < p && p <= 30) {
            category = "low";
        } else {
            category = actual_status;
        }
    } else if(actual_status == "high"){
        if(1 <= p && p <= 5) {
            category = "low";
        } else if (5 < p && p <= 30) {
            category = "moderate";
        } else {
            category = actual_status;
        }
    } else {
        category = actual_status;
    }

    return category;
}

string SensorNodeModule::statusAnalysis(string categorized_data, string actual_status) {

    string new_status;
    int l=0, m=0, h=0;

    if(m_data_queue.size()>=5){
        m_data_queue.pop_front();
    }

    m_data_queue.push_back(categorized_data);

    for(uint32_t i = 0; i < m_data_queue.size(); i++) {
        if(m_data_queue[i]=="low"){
            l++;
        } else if(m_data_queue[i]=="moderate") {
            m++;
        } else if (m_data_queue[i]=="high"){
            h++;
        }
    }

    if(l>=3){
        new_status = "low";
    } else if (m>=3) {
        new_status = "moderate";
    } else if (h>=3) {
        new_status = "high";
    } else {
        new_status = actual_status;
    }

    return new_status;
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

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorNodeModule::body() {

    srand(time(NULL));
    timespec ts; 

    bool exe;
    int cycles = 0;
    
    int ccc=0;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        cycles++;
        ccc++;
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

            cout << "Actual status: " << m_status << " | Data sampled: " << categorized_data << " at " << TimeStamp().getYYYYMMDD_HHMMSSms() << endl;
            timespec t_esy = elapsedTime(ts, m_ref);
            m_status_log << (double)((t_esy.tv_sec) + (t_esy.tv_nsec/1E9)) << ",";
            m_status_log << ((m_status=="low")?1:(m_status=="moderate")?2:3) << ",";
            m_status_log << " " << "\n";
        }
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}