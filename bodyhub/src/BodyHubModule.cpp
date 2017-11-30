#include "BodyHubModule.h"

#include "opendavinci/odcore/data/TimeStamp.h"

using namespace std;
using namespace std::chrono;

using namespace odcore::base;
using namespace odcore::data;
using namespace odcore::base::module;
using namespace odcore::data::dmcp;

using namespace openbasn::data;
using namespace openbasn::message;

BodyHubModule::BodyHubModule(const int32_t &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "bodyhub"),
    m_id(getIdentifier()),
    m_buffer(),
    m_health_status("-"),
    m_sensor(),
    m_status_log(),
    m_bodyhub_log(),
    m_ref() {}

BodyHubModule::~BodyHubModule() {}

void BodyHubModule::setUp() {
    addDataStoreFor(873, m_buffer);

    clock_gettime(CLOCK_REALTIME, &m_ref);

    string path = "output/";
    /* m_bodyhub_log.open(path + "bodyhub_log.csv");
    m_bodyhub_log << "Ciclo, Estado do paciente, Detectou emergência?, Sensor, Consume pacote(us), Atualiza estado(us), Detecta emergência(us), Persiste(us), Imprime(us), Total(us), Packages consumed(#) , Timestamp\n"; */
    
    m_status_log.open(path+"bodyhub_status_log-2.csv");
    m_status_log << "Sender Sensor ID, Temperature, Heart Rate, Blood oxigenation, Patient status, Sent at (s), Received at (s), Processed at (s), Diff (s)\n";
}

void BodyHubModule::tearDown() {
    m_status_log.close();

    /* m_bodyhub_log << "-,-,-,-," << "=AVERAGE(E2:E"+to_string((n+1))+")," << "=AVERAGE(F2:F"+to_string((n+1))+")," << "=AVERAGE(G2:G"+to_string((n+1))+")," << "=AVERAGE(H2:H"+to_string((n+1))+")," << "=AVERAGE(I2:I"+to_string((n+1))+")," << "=AVERAGE(J2:J"+to_string((n+1))+")," << "=AVERAGE(K2:K"+to_string((n+1))+")," << "AVG" <<  "\n";
    m_bodyhub_log << "-,-,-,-," << "=MAX(E2:E"+to_string((n+1))+")," << "=MAX(F2:F"+to_string((n+1))+")," << "=MAX(G2:G"+to_string((n+1))+")," << "=MAX(H2:H"+to_string((n+1))+")," << "=MAX(I2:I"+to_string((n+1))+")," << "=MAX(J2:J"+to_string((n+1))+")," << "=MAX(K2:K"+to_string((n+1))+"),"  << "MAX"  << "\n";
    m_bodyhub_log << "-,-,-,-," << "=MIN(E2:E"+to_string((n+1))+")," << "=MIN(F2:F"+to_string((n+1))+")," << "=MIN(G2:G"+to_string((n+1))+")," << "=MIN(H2:H"+to_string((n+1))+")," << "=MIN(I2:I"+to_string((n+1))+")," << "=MIN(J2:J"+to_string((n+1))+")," << "=MIN(K2:K"+to_string((n+1))+")," << "MIN"  << "\n";
    m_bodyhub_log << "-,-,-,-," << "=STDEV(E2:E"+to_string((n+1))+")," << "=STDEV(F2:F"+to_string((n+1))+")," << "=STDEV(G2:G"+to_string((n+1))+")," << "=STDEV(H2:H"+to_string((n+1))+")," << "=STDEV(I2:I"+to_string((n+1))+")," << "=STDEV(J2:J"+to_string((n+1))+")," << "=STDEV(K2:K"+to_string((n+1))+")," << "STDEV"  << "\n";
    m_bodyhub_log << "-,-,-,-," << "=E"+to_string((n+2))+"/SQRT("+to_string((n))+")," << "=F"+to_string((n+2))+"/SQRT("+to_string((n))+")," << "=G"+to_string((n+2))+"/SQRT("+to_string((n))+")," << "=H"+to_string((n+2))+"/SQRT("+to_string((n))+")," << "=I"+to_string((n+2))+"/SQRT("+to_string((n))+")," << "=J"+to_string((n+2))+"/SQRT("+to_string((n))+")," << "=K"+to_string((n+2))+"/SQRT("+to_string((n))+")," << "ERROR"  << "\n";
    
    m_bodyhub_log.close(); */
}

string BodyHubModule::calculateHealthStatus(){
    double hr = 0;
    for(uint32_t i = 0; i < m_sensor.size(); i++){

        if (m_sensor[i] == "low") {
            hr += 0.1;
        } else if (m_sensor[i] == "moderate") {
            hr += 1;
        } else if (m_sensor[i] == "high") {
            hr += 5;
        } 
    }
    
    return (hr<=0)?"unknown":(hr<1)?"Good":(hr<5)?"Moderate":(hr<20)?"Bad":"unknown";
}

void BodyHubModule::updateHealthStatus(SensorData sensordata){
    m_sensor[sensordata.getSensorType()-1] = sensordata.getSensorStatus();
    m_health_status = BodyHubModule::calculateHealthStatus();
}

timespec BodyHubModule::elapsedTime(timespec &now, timespec &ref) {

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

void BodyHubModule::persistHealthStatus(uint32_t sensor_id, timespec t_sen, timespec t_rec){
    
    timespec t_proc;
    clock_gettime(CLOCK_REALTIME, &t_proc);

    timespec sent = elapsedTime(t_sen, m_ref);
    timespec received = elapsedTime(t_rec, m_ref);
    timespec processed = elapsedTime(t_proc, m_ref);

    m_status_log << sensor_id << ",";

    //"Thermometer status, ECG status, Oximeter status, Patient Status, Sent at, Received at, Processed at"
    for(uint32_t i = 0; i < m_sensor.size(); i++){
        m_status_log << m_sensor[i] << ",";
    }

    m_status_log << m_health_status << ",";
    m_status_log << (double)((sent.tv_sec) + (sent.tv_nsec/1E9)) << ",";
    m_status_log << (double)((received.tv_sec) + (received.tv_nsec/1E9)) << ",";
    m_status_log << (double)((processed.tv_sec) + (processed.tv_nsec/1E9)) << ",";

    timespec result = elapsedTime(t_rec, t_sen);
    m_status_log << (result.tv_sec) + (result.tv_nsec/1E9) << "\n";
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

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BodyHubModule::body() { 
    
    int cycle = 0;
    int counter = 0;
    bool is_emergency = false;
    uint32_t sensor_id = 0;

    timespec ts;
    /* timespec  old_ts , rs , tk; */

    /* high_resolution_clock::time_point t1, t2, t3, t4, t5, t6, t7, t8; */

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
        
        counter=0;
        cycle++;
        
        /* clock_gettime(CLOCK_REALTIME, &tk);
        if ((tk.tv_nsec - old_ts.tv_nsec) < 0) {
            rs.tv_sec = tk.tv_sec - old_ts.tv_sec - 1;
            rs.tv_nsec = tk.tv_nsec - old_ts.tv_nsec + 1000000000L;
        } else {
            rs.tv_sec = tk.tv_sec - old_ts.tv_sec;
            rs.tv_nsec = tk.tv_nsec - old_ts.tv_nsec;
        }

        auto measured_cycle = std::chrono::seconds{rs.tv_sec} + std::chrono::nanoseconds{rs.tv_nsec};
        cout<< "cycle time: " << measured_cycle.count() << endl;
        old_ts=tk; */
        /* t1 = high_resolution_clock::now(); */

        while(!m_buffer.isEmpty()){

            clock_gettime(CLOCK_REALTIME, &ts);

            /* t2 = high_resolution_clock::now(); */
            //Consome dado
            Container container = m_buffer.leave();

            if (container.getDataType() == SensorData::ID()) {
                counter++;
                /* t3 = high_resolution_clock::now(); */
                //Atualiza estado do paciente
                BodyHubModule::updateHealthStatus(container.getData<SensorData>());
                sensor_id= container.getData<SensorData>().getSensorID();
                /* t4 = high_resolution_clock::now(); */
                //Detecta emergencia
                is_emergency=(container.getData<SensorData>().getSensorStatus() == "high")?true:false;
                CLOG1<<"Emergencia?"<<is_emergency<<endl;
                /* t5 = high_resolution_clock::now(); */
                //Persiste
                BodyHubModule::persistHealthStatus(sensor_id, container.getData<SensorData>().getSentTimespec(), ts);
                /* t6 = high_resolution_clock::now(); */
            }

            BodyHubModule::printHealthStatus();
            /* t7 = high_resolution_clock::now(); */
        }            
        
        /* 
        t8 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t8-t1).count(); 
        */
        
        /* 
        if(duration !=0 && counter>=1){
            n++;
            cout << "Execution time elapsed: " << duration << endl;

            m_bodyhub_log << cycle << ",";
            m_bodyhub_log << m_health_status  << ",";
            m_bodyhub_log << is_emergency  << ",";
            m_bodyhub_log << sensor_id  << ",";
            m_bodyhub_log << duration_cast<microseconds>(t3-t2).count() << ",";
            m_bodyhub_log << duration_cast<microseconds>(t4-t3).count() << ",";
            m_bodyhub_log << duration_cast<microseconds>(t5-t4).count() << ",";
            m_bodyhub_log << duration_cast<microseconds>(t6-t5).count() << ",";
            m_bodyhub_log << duration_cast<microseconds>(t7-t6).count() << ",";
            m_bodyhub_log << duration_cast<microseconds>(t8-t1).count() << ",";
            m_bodyhub_log << counter << ",";
            m_bodyhub_log << TimeStamp().getYYYYMMDD_HHMMSSms() << "\n";
        } 
        */
    }
    
    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
