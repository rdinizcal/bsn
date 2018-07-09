#include "bsn/persistence/PersistenceModule.h"

using namespace std;

/*
Sensor Persistence methods
*/

namespace bsn {
    namespace persistence {

        SensorPersistence::SensorPersistence(){
            sensor_packages_file.open("../../persistence/output/packages_sent.txt");
            string path = "../../persistence/output/";
            string filename = "sensornode";
            sensor_status_log.open(path + filename + "_status_log.csv");
            sensor_status_log << "Elapsed Time(s), Sensor Status, Time Since Last (s)\n";
        }

        /*SensorPersistence::SensorPersistence(uint32_t s_id){
            sensor_packages_file.open("../../persistence/output/packages_sent.txt");
            string path = "../../persistence/output/";
            string filename = "sensornode" + to_string(s_id);
            sensor_status_log.open(path + filename + "_status_log.csv");
            sensor_status_log << "Elapsed Time(s), Sensor Status, Time Since Last (s)\n";
        }*/

        SensorPersistence::~SensorPersistence(){
            sensor_packages_file.close();
            sensor_status_log.close();
        }

        void SensorPersistence::persist_sensor_status(timespec ts, string sensor_status, timespec s_ref){    
            timespec t_esy = elapsedTime(ts, s_ref);
            sensor_status_log << (double)((t_esy.tv_sec) + (t_esy.tv_nsec / 1E9)) << ",";
            sensor_status_log << ((sensor_status == "baixo") ? 1 : (sensor_status == "moderado") ? 2 : 3) << ",";
            sensor_status_log << " "
                        << "\n";
        }

        void SensorPersistence::persist_packages_sent(uint32_t id, string status){
            sensor_packages_file << id << ' ' << status << "\n";
        }

        /*
        BodyHub Persistence methods
        */

        BodyHubPersistence::BodyHubPersistence(){
            bodyhub_packages_file.open("../../persistence/output/packages_received.txt");
            string path = "../../persistence/output/";    
            bodyhub_status_log.open(path+"bodyhub_status_log.csv");
            bodyhub_status_log << "ID do sensor, Estado do Termômetro, Estado do ECG, Estado do Oxímetro, Estado do Paciente, Enviado às (s), Recebido às (s), Processado às (s), Diff (s)\n";
        }

        BodyHubPersistence::~BodyHubPersistence(){
            bodyhub_packages_file.close();
            bodyhub_status_log.close();
        }

        void BodyHubPersistence::persistHealthStatus(uint32_t sensor_id, timespec t_sen, timespec t_rec, timespec m_ref, 
                                                        map<uint32_t, string> m_sensor, string m_health_status){
            timespec t_proc;
            clock_gettime(CLOCK_REALTIME, &t_proc);

            timespec sent = elapsedTime(t_sen, m_ref);
            timespec received = elapsedTime(t_rec, m_ref);
            timespec processed = elapsedTime(t_proc, m_ref);

            bodyhub_status_log << sensor_id << ",";

            for (uint32_t i = 0; i < m_sensor.size(); i++){
                bodyhub_status_log << m_sensor[i] << ",";
            }

            bodyhub_status_log << m_health_status << ",";
            bodyhub_status_log << (double)((sent.tv_sec) + (sent.tv_nsec / 1E9)) << ",";
            bodyhub_status_log << (double)((received.tv_sec) + (received.tv_nsec / 1E9)) << ",";
            bodyhub_status_log << (double)((processed.tv_sec) + (processed.tv_nsec / 1E9)) << ",";

            timespec result = elapsedTime(t_rec, t_sen);
            bodyhub_status_log << (result.tv_sec) + (result.tv_nsec / 1E9) << "\n";
        }

        void BodyHubPersistence::persistDataReceived(uint32_t a, string b){
            bodyhub_packages_file << a << " " << b << "\n";
        }

        timespec elapsedTime(timespec &now, timespec &ref) {
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
    
    }
}
