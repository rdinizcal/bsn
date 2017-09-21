#ifndef DATA_SENSOR_H_
#define DATA_SENSOR_H_

#include "opendavinci/odcore/data/SerializableData.h"

namespace openbasn {
    namespace data {
        
        using namespace std;
        using namespace odcore::data;
        
        class SensorData : public SerializableData {
        
            public:
                SensorData(uint32_t /*snode_id*/, int32_t /*sensor_type*/, double /*data*/);
        
                SensorData();
                virtual ~SensorData();
        
                SensorData(const SensorData &/*obj*/);
                SensorData& operator=(const SensorData &/*obj*/);
        
            public:
                virtual int32_t getID() const;
                virtual const string getShortName() const;
                virtual const string getLongName() const;
        
            public:
                virtual ostream& operator<<(ostream &out) const;
                virtual istream& operator>>(istream &in);
        
                virtual const string toString() const;
        
            public:
                uint32_t getSensorNodeID() const;
                int32_t getSensorType() const;
                double getData() const;
            
            public:
                static int32_t ID();
                static const string ShortName();
                static const string LongName();
        
            private:
                uint32_t m_snode_id;
                int32_t m_sensor_type;
                double m_data;
                
        
        };
    }
}

#endif