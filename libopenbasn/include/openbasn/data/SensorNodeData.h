#ifndef DATA_SENSOR_H_
#define DATA_SENSOR_H_

#include <map>

#include "opendavinci/odcore/data/SerializableData.h"

namespace openbasn {
    namespace data {
        
        using namespace std;
        using namespace odcore::data;
        
        class SensorNodeData : public SerializableData {
        
            public:
                SensorNodeData(const uint32_t &/*sensornode_id*/, const int32_t &/*sensor_type*/, const double &/*sensor_data*/);
        
                SensorNodeData();
                virtual ~SensorNodeData();
        
                SensorNodeData(const SensorNodeData &/*obj*/);
                SensorNodeData& operator=(const SensorNodeData &/*obj*/);
        
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
                void setSensorNodeID(const uint32_t &/*sensornode_id*/);
                uint32_t getSensorNodeID() const;

                void setSensorType(const int32_t &/*sensor_type*/);
                int32_t getSensorType() const;

                void setSensorData(const double &/*sensor_data*/);
                double getSensorData() const;

            
            private:
                uint32_t m_sensornode_id;
                int32_t m_sensor_type;
                double m_sensor_data;
        };
    }
}

#endif