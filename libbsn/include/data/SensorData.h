#ifndef DATA_SENSOR_H_
#define DATA_SENSOR_H_

#include <algorithm>
#include <vector>
#include <string>

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

#include <stdint.h>

namespace bsn {
    namespace data {
        
        class SensorData : public odcore::data::SerializableData {
        
            public:
                //construtor parametrizado
                SensorData(const std::array<std::string, 2> &, const std::array<double, 4> &, const std::array<std::string, 8> &);
 
                SensorData(); // construtor
                virtual ~SensorData();// destrutor 
                
                //Boas práticas do manual do OpenDaVINCI
                SensorData(const SensorData &/*obj*/);
                SensorData& operator=(const SensorData &/*obj*/);
            
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
                void setSensorType(const std::array<std::string, 2>);
                std::array<std::string, 2> getSensorType() const;

                void setSensorData(const std::array<double, 4>);
                std::array<double, 4> getSensorData() const;

                void setTimes(const std::array<std::string, 8>);
                std::array<std::string, 8> getTimes() const;
            
            private:
                std::array<std::string, 2> sensorType; // tipo do sensor
                std::array<double, 4> sensorData; // estado do sensor
                std::array<std::string, 8> times;  // instante de geração do dado
        };
    }
}

#endif