#ifndef PERSISTENCE_MODULE_H
#define PERSISTENCE_MODULE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <map>

using namespace std;

namespace bsn {
    namespace persistence {

        class SensorPersistence{
            public:
                //SensorPersistence(uint32_t s_id);
                SensorPersistence();
                virtual ~SensorPersistence();
                void persist_sensor_status(timespec ts, string sensor_status, timespec s_ref);
                void persist_packages_sent(uint32_t id, string status);
            private:
                ofstream sensor_status_log;
                ofstream sensor_packages_file;
        };

        class BodyHubPersistence{
            public:
                BodyHubPersistence();
                virtual ~BodyHubPersistence();
                void persistHealthStatus(uint32_t sensor_id, timespec t_sen, timespec t_rec, timespec m_ref,
                                        map<uint32_t, string> m_sensor, string m_health_status);
                void persistDataReceived(uint32_t a, string b);
            private:
                ofstream bodyhub_status_log;
                ofstream bodyhub_packages_file;
        };

        timespec elapsedTime(timespec &now, timespec &ref);

    }
}

#endif