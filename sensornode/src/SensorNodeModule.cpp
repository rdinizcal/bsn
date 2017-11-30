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
    m_sensornode_log(),
    n(0) {}

    SensorNodeModule::~SensorNodeModule() {}

void SensorNodeModule::setUp() {

    m_sensor_type = m_id+1;

    string path = "output/";
    string filename = "sensornode" + to_string(m_id);
    /* m_sensornode_log.open(path + filename + "_log.csv");
    m_sensornode_log << "Cycle, Status, Sampled Data, is Emergency?, Monitor(us), Analyze(us), Plan&Execute(us), Total(us), Timestamp\n"; */

    m_status_log.open(path + filename + "_status_log.csv");
    m_status_log << "Sensor Status, Timestamp (s)\n";
}

void SensorNodeModule::tearDown() {

    /* m_sensornode_log << "-,-,-,-," << "=AVERAGE(E2:E"+to_string((n+1))+")," << "=AVERAGE(F2:F"+to_string((n+1))+")," << "=AVERAGE(G2:G"+to_string((n+1))+")," << "=AVERAGE(H2:H"+to_string((n+1))+")," << "AVG" << "\n";
    m_sensornode_log << "-,-,-,-," << "=MAX(E2:E"+to_string((n+1))+")," << "=MAX(F2:F"+to_string((n+1))+")," << "=MAX(G2:G"+to_string((n+1))+")," << "=MAX(H2:H"+to_string((n+1))+")," << "MAX"  << "\n";
    m_sensornode_log << "-,-,-,-," << "=MIN(E2:E"+to_string((n+1))+")," << "=MIN(F2:F"+to_string((n+1))+")," << "=MIN(G2:G"+to_string((n+1))+")," << "=MIN(H2:H"+to_string((n+1))+")," << "MIN"  << "\n";
    m_sensornode_log << "-,-,-,-," << "=STDEV(E2:E"+to_string((n+1))+")," << "=STDEV(F2:F"+to_string((n+1))+")," << "=STDEV(G2:G"+to_string((n+1))+")," << "=STDEV(H2:H"+to_string((n+1))+")," << "STDEV"  << "\n";
    m_sensornode_log << "-,-,-,-," << "=E"+to_string((n+2))+"/SQRT("+to_string((n))+")," << "=F"+to_string((n+2))+"/SQRT("+to_string((n))+")," << "=G"+to_string((n+2))+"/SQRT("+to_string((n))+")," << "=H"+to_string((n+2))+"/SQRT("+to_string((n))+")," << "ERROR"  << "\n";

    m_sensornode_log.close(); */
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

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode SensorNodeModule::body() {

    string categorized_data, new_m_status;
    int counter = 0, freq = 10;
    /* bool is_emergency=false; */

    timespec ts; 

    /* high_resolution_clock::time_point t1,t2,t3,t4; */

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        counter++;

        if(counter==freq){
            
            /* t1 = high_resolution_clock::now(); */
            
            counter=0;

            /* 
             * MONITOR
             * Sample Data - obter novo dado do sensor
             * Process Data - processar dado do sensor
             * Categorize Data - categorizar dado do sensor de acordo com conhecimento de domínio
             */
            categorized_data = generateData(m_status);
            cout << "Actual status: " << m_status << " | Data sampled: " << categorized_data << " at " << TimeStamp().getYYYYMMDD_HHMMSSms() << endl;
            /* t2 = high_resolution_clock::now(); */

            /* 
             * ANALYZE
             * Analyze sensornode status 
             */
            new_m_status = statusAnalysis(categorized_data, m_status);
            /* t3 = high_resolution_clock::now(); */
            
            /* 
             * PLAN AND EXECUTE
             * alterar a frequencia de sampling e enviar estado atual
             */
            if(m_status != new_m_status) {
                m_status = new_m_status;

                if(m_status=="low"){
                    freq = 10;
                    /* is_emergency=false; */
                } else if (m_status=="moderate"){
                    freq = 5;
                    /* is_emergency=false; */
                } else if (m_status=="high"){
                    freq = 1;
                    /* is_emergency=true; */
                }

                clock_gettime(CLOCK_REALTIME, &ts);
                SensorNodeModule::sendSensorData(SensorData(m_id, m_sensor_type, m_status, ts));

                m_status_log << m_status << ",";
                m_status_log << (ts.tv_sec) + (ts.tv_nsec/1E9) << "\n";
            }

            /* t4 = high_resolution_clock::now();
        
            auto duration = duration_cast<microseconds>(t4-t1).count();
            CLOG1 << "Execution time elapsed: " << duration << endl;

            m_sensornode_log << cycle << ",";
            m_sensornode_log << m_status  << ",";
            m_sensornode_log << categorized_data  << ",";
            m_sensornode_log << is_emergency  << ",";
            m_sensornode_log << duration_cast<microseconds>(t2-t1).count() << ",";
            m_sensornode_log << duration_cast<microseconds>(t3-t2).count() << ",";
            m_sensornode_log << duration_cast<microseconds>(t4-t3).count() << ",";
            m_sensornode_log << duration_cast<microseconds>(t4-t1).count() << ",";
            m_sensornode_log << TimeStamp().getYYYYMMDD_HHMMSSms() << "\n";

            n++; */
        }
        
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}