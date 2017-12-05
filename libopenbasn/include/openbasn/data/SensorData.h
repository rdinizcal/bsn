/*
 * Mensagem com dados do sensor remetente
 * 
 * @author Ricardo Diniz Caldas
 * @version v1.0
 */

#ifndef DATA_SENSOR_H_
#define DATA_SENSOR_H_

#include "opendavinci/odcore/data/SerializableData.h"

namespace openbasn {
    namespace data {
        
        using namespace std;
        using namespace odcore::data;
        
        class SensorData : public SerializableData {
        
            public:
                //construtor parametrizado
                SensorData(const uint32_t &/*sensor_id*/, const int32_t &/*sensor_type*/, const string &/*sensor_status*/, const timespec &/*sent_ts*/);
 
                SensorData(); // construtor
                virtual ~SensorData();// destrutor 
                
                //Boas práticas do manual do OpenDaVINCI
                SensorData(const SensorData &/*obj*/);
                SensorData& operator=(const SensorData &/*obj*/);
            
            // Métodos abstratos
            public:
                virtual int32_t getID() const;
                virtual const string getShortName() const;
                virtual const string getLongName() const;

                static int32_t ID();
                static const string ShortName();
                static const string LongName();
        
            public:
                virtual ostream& operator<<(ostream &out) const;
                virtual istream& operator>>(istream &in);
        
                virtual const string toString() const;
                
            // setters e getters
            public:
                void setSensorID(const uint32_t &/*sensor_id*/);
                uint32_t getSensorID() const;

                void setSensorType(const int32_t &/*sensor_type*/);
                int32_t getSensorType() const;

                void setSensorStatus(const string &/*sensor_status*/);
                string getSensorStatus() const;

                void setSentTimespec(const timespec &/*sent_ts*/);
                timespec getSentTimespec() const;
            
            private:
                uint32_t m_sensor_id; // identificador do sensor
                int32_t m_sensor_type; // tipo do sensor
                string m_sensor_status; // estado do sensor 
                timespec m_sent_ts; // instante de geração do dado
        };
    }
}

#endif