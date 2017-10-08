#include "openbasn/data/SensorNodeData.h"

#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <iterator>
#include <algorithm>

#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

namespace openbasn {
    namespace data {
        
        using namespace std;
        
        SensorNodeData::SensorNodeData() : m_sensornode_id(), m_sensor_data() {}
        
        SensorNodeData::SensorNodeData(const uint32_t &sensornode_id) : 
            m_sensornode_id(sensornode_id),
            m_sensor_data() {}
        
        SensorNodeData::~SensorNodeData() {}
        
        SensorNodeData::SensorNodeData(const SensorNodeData &obj) :
            SerializableData(),
            m_sensornode_id(obj.getSensorNodeID()),
            m_sensor_data(obj.getSensorDataMap()) {}
        
        SensorNodeData& SensorNodeData::operator=(const SensorNodeData &obj) {
            m_sensornode_id = obj.getSensorNodeID();
            m_sensor_data = obj.getSensorDataMap();
            return (*this);
        }
        
        int32_t SensorNodeData::ID() {
            return 873;
        }
        const string SensorNodeData::ShortName() {
            return "SensorNodeData";
        }
        const string SensorNodeData::LongName() {
            return "data.SensorNodeData";
        }
        
        int32_t SensorNodeData::getID() const {
            return SensorNodeData::ID();
        }
        const string SensorNodeData::getShortName() const {
            return SensorNodeData::ShortName();
        }
        const string SensorNodeData::getLongName() const {
            return SensorNodeData::LongName();
        }
        
        void SensorNodeData::setSensorNodeID(const uint32_t &sensornode_id) {
            m_sensornode_id = sensornode_id;
        }
        
        uint32_t SensorNodeData::getSensorNodeID() const {
            return m_sensornode_id;
        }
        
        void SensorNodeData::addSensorData(const int32_t &sensor_type, const double &sensor_data) {
            m_sensor_data.insert(pair<int32_t,double>(sensor_type,sensor_data));
        }

        map<int32_t,double> SensorNodeData::getSensorDataMap() const {
            return m_sensor_data;
        }
        
        ostream& SensorNodeData::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, m_sensornode_id);

            //Write the number of sensors
            uint32_t numberOfSensors = static_cast<uint32_t>(m_sensor_data.size());
            s->write(2, numberOfSensors);

            // map to sstream
            stringstream sstr;
            map<int32_t,double>::const_iterator it = m_sensor_data.begin();
            for (pair<int32_t,double> element : m_sensor_data) {
                sstr << element.first << "|";
                sstr << element.second << "|";
            }

            // Write the sstream
            if (numberOfSensors > 0) {
                s->write(3, sstr.str());
            }

            return out;
        }
        
        istream& SensorNodeData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            d->read(1, m_sensornode_id);

            //Read number of sensors
            uint32_t numberOfSensors = 0;
            d->read(2, numberOfSensors);

            //Read map stream
            string sd_map;
            if(numberOfSensors > 0) {
                d->read(3, sd_map);
            }

            // Stream to map
            stringstream sstr(sd_map);
            for (uint32_t i = 0; i < numberOfSensors; i++) {
                int32_t type;
                double data;
                char trash;

                sstr >> type;
                sstr >> trash;
                sstr >> data;
                sstr >> trash;

                m_sensor_data.insert(pair<int32_t,double>(type,data));
            }
        
            return in;
        }
        
        const string SensorNodeData::toString() const {
            stringstream sstr;

            sstr << "SensorNode#" << m_sensornode_id << " - ";
            map<int32_t,double>::const_iterator it = m_sensor_data.begin();
            for (pair<int32_t,double> element : m_sensor_data) {
                sstr << " SensorType#" << element.first << " : " << element.second << " |";
            }

            return sstr.str();
        }
    }
}