#include "openbasn/data/SensorData.h"

#include <ostream>
#include <string>

#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

namespace openbasn {
    namespace data {
        
        using namespace std;
        
        SensorData::SensorData() : 
        m_snode_id(0),
        m_sensor_type(0),
        m_data(0)
        {}
        
        SensorData::SensorData(uint32_t snode_id, int32_t sensor_type, double data) : 
            m_snode_id(snode_id),
            m_sensor_type(sensor_type),
            m_data(data)
        {}
        
        SensorData::~SensorData() {}
        
        SensorData::SensorData(const SensorData &obj) :
            SerializableData(),
            m_snode_id(obj.getSensorNodeID()),
            m_sensor_type(obj.getSensorType()),
            m_data(obj.getData()) {}
        
        SensorData& SensorData::operator=(const SensorData &obj) {
            m_snode_id = obj.getSensorNodeID();
            m_sensor_type = obj.getSensorType();
            m_data= obj.getData();
            return (*this);
        }
        
        int32_t SensorData::ID() {
            return 7;
        }
        const string SensorData::ShortName() {
            return "SensorData";
        }
        const string SensorData::LongName() {
            return "data.SensorData";
        }
        
        int32_t SensorData::getID() const {
            return SensorData::ID();
        }
        const string SensorData::getShortName() const {
            return SensorData::ShortName();
        }
        const string SensorData::getLongName() const {
            return SensorData::LongName();
        }
        
        uint32_t SensorData::getSensorNodeID() const {
            return m_snode_id;
        }
        
        int32_t SensorData::getSensorType() const {
            return m_sensor_type;
        }
        
        double SensorData::getData() const {
            return m_data;
        }
        
        ostream& SensorData::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
        
            s->write(1, m_snode_id);
            s->write(2, m_sensor_type);
            s->write(3, m_data);
        
            return out;
        }
        
        istream& SensorData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
        
            d->read(1, m_snode_id);
            d->read(2, m_sensor_type);
            d->read(3, m_data);
        
            return in;
        }
        
        const string SensorData::toString() const {
            stringstream sstr;
        
            sstr << SensorData::ShortName() << "#" << SensorData::ID() << " | ";
            sstr << "SensorNode#" << m_snode_id << " SensorType#" << m_sensor_type << " : " << m_data;
            
            return sstr.str();
        }
    }
}