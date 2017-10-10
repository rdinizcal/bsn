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
        
        SensorNodeData::SensorNodeData() : m_sensornode_id(), m_sensor_type(), m_sensor_data() {}
        
        SensorNodeData::SensorNodeData(const uint32_t &sensornode_id, const int32_t &sensor_type, const double &sensor_data) : 
            m_sensornode_id(sensornode_id),
            m_sensor_type(sensor_type),
            m_sensor_data(sensor_data) {}
        
        SensorNodeData::~SensorNodeData() {}
        
        SensorNodeData::SensorNodeData(const SensorNodeData &obj) :
            SerializableData(),
            m_sensornode_id(obj.getSensorNodeID()),
            m_sensor_type(obj.getSensorType()),
            m_sensor_data(obj.getSensorData()) {}
        
        SensorNodeData& SensorNodeData::operator=(const SensorNodeData &obj) {
            m_sensornode_id = obj.getSensorNodeID();
            m_sensor_type = obj.getSensorType();
            m_sensor_data = obj.getSensorData();
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

        void SensorNodeData::setSensorType(const int32_t &sensor_type) {
            m_sensor_type = sensor_type;
        }
        
        int32_t SensorNodeData::getSensorType() const {
            return m_sensor_type;
        }

        void SensorNodeData::setSensorData(const double &sensor_data) {
            m_sensor_data = sensor_data;
        }
        
        double SensorNodeData::getSensorData() const {
            return m_sensor_data;
        }
        
        
        ostream& SensorNodeData::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, m_sensornode_id);
            s->write(2, m_sensor_type);
            s->write(3, m_sensor_data);

            return out;
        }
        
        istream& SensorNodeData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            d->read(1, m_sensornode_id);
            d->read(2, m_sensor_type);
            d->read(3, m_sensor_data);
        
            return in;
        }
        
        const string SensorNodeData::toString() const {
            stringstream sstr;

            sstr << "SensorNode#" << m_sensornode_id << " - ";
            sstr << " SensorType#" << m_sensor_type << " : " << m_sensor_data << " |";

            return sstr.str();
        }
    }
}