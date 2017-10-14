#ifndef DATA_SENSOR_H_
#define DATA_SENSOR_H_

#include <map>

#include "opendavinci/odcore/data/SerializableData.h"

namespace openbasn {
    namespace data {
        
        using namespace std;
        using namespace odcore::data;
        
        class SensorData : public SerializableData {
        
            public:
                SensorData(const uint32_t &/*sensor_id*/, const int32_t &/*sensor_type*/, const double &/*sensor_data*/);
        
                SensorData();
                virtual ~SensorData();
        
                SensorData(const SensorData &/*obj*/);
                SensorData& operator=(const SensorData &/*obj*/);
        
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
        
            public:
                void setSensorID(const uint32_t &/*sensor_id*/);
                uint32_t getSensorID() const;

                void setSensorType(const int32_t &/*sensor_type*/);
                int32_t getSensorType() const;

                void setSensorData(const double &/*sensor_data*/);
                double getSensorData() const;

            
            private:
                uint32_t m_sensor_id;
                int32_t m_sensor_type;
                double m_sensor_data;
        };
    }
}

#endif