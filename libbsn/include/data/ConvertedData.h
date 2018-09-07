#ifndef CONVERTED_DATA_H_
#define CONVERTED_DATA_H_

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

namespace bsn {
    namespace data {

        class ConvertedData : public odcore::data::SerializableData{
          public:
            //construtor parametrizado
            ConvertedData(const double & /*converted data*/, const std::string &);

            ConvertedData();          // construtor
            virtual ~ConvertedData(); // destrutor

            //Boas práticas do manual do OpenDaVINCI
            ConvertedData(const ConvertedData & /*obj*/);
            ConvertedData &operator=(const ConvertedData & /*obj*/);
            
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
                virtual std::istream &operator>>(std::istream &in);

                virtual const std::string toString() const;
                
            // setters e getters
            public:
                void setSensorData(const double &/*sensor_data*/);
                double getSensorData() const;
                void setSensorType(const std::string &);
                std::string getSensorType() const;
            
            private:
                double m_sensor_data; // estado do sensor
                std::string sensorType; 
        };
    }
}

#endif