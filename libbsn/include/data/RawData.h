#ifndef RAW_DATA_H_
#define RAW_DATA_H_

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"
#include <sys/time.h>

namespace bsn {
    namespace data {

        class RawData : public odcore::data::SerializableData {

          public:
            //construtor parametrizado
            RawData(const double & /*raw data*/, const std::string &, const timespec &);

            RawData();          // construtor
            virtual ~RawData(); // destrutor

            //Boas práticas do manual do OpenDaVINCI
            RawData(const RawData & /*obj*/);
            RawData &operator=(const RawData & /*obj*/);
            
            // Métodos abstratos
            public:
                virtual int32_t getID() const;
                virtual const std::string getShortName() const;
                virtual const std::string getLongName() const;

                static int32_t ID();
                static const std::string ShortName();
                static const std::string LongName();
        
            public:
                virtual std::ostream& operator<<(std::ostream &out) const;
                virtual std::istream& operator>>(std::istream &in);
        
                virtual const std::string toString() const;
                
            // setters e getters
            public:
                void setSensorData(const double &/*sensor_data*/);
                double getSensorData() const;

                void setTimespec(const timespec & /*sent_ts*/);
                timespec getTimespec() const;

                void setSensorType(const std::string &);
                std::string getSensorType() const;

            private:
                double m_sensor_data; // estado do sensor
                std::string m_sensor_type;
                timespec m_time;
        };
    }
}

#endif