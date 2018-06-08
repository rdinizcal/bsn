/*
 * Mensagem com dados do sensor remetente
 * 
 * @author Ricardo Diniz Caldas
 * @version v1.0
 */

#ifndef RAW_DATA_H_
#define RAW_DATA_H_

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

namespace bsn {
    namespace data {
        
        using namespace std;
        using namespace odcore::data;
        
        class RawData : public SerializableData {
        
            public:
                //construtor parametrizado
                RawData(const double &/*raw data*/);

                RawData();            // construtor
                virtual ~RawData();   // destrutor

                //Boas práticas do manual do OpenDaVINCI
                RawData(const RawData & /*obj*/);
                RawData& operator=(const RawData &/*obj*/);
            
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
                void setSensorData(const double &/*sensor_data*/);
                double getSensorData() const;
            
            private:
                double m_sensor_data; // estado do sensor 
        };
    }
}

#endif