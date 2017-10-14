#include "openbasn/data/SensorData.h"

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
        
        SensorData::SensorData() : m_sensor_id(), m_sensor_type(), m_sensor_data() {}
        
        SensorData::SensorData(const uint32_t &sensor_id, const int32_t &sensor_type, const double &sensor_data) : 
            m_sensor_id(sensor_id),
            m_sensor_type(sensor_type),
            m_sensor_data(sensor_data) {}
        
        SensorData::~SensorData() {}
        
        SensorData::SensorData(const SensorData &obj) :
            SerializableData(),
            m_sensor_id(obj.getSensorID()),
            m_sensor_type(obj.getSensorType()),
            m_sensor_data(obj.getSensorData()) {}
        
        SensorData& SensorData::operator=(const SensorData &obj) {
            m_sensor_id = obj.getSensorID();
            m_sensor_type = obj.getSensorType();
            m_sensor_data = obj.getSensorData();
            return (*this);
        }
        
        int32_t SensorData::ID() {
            return 873;
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
        
        void SensorData::setSensorID(const uint32_t &sensor_id) {
            m_sensor_id = sensor_id;
        }
        
        uint32_t SensorData::getSensorID() const {
            return m_sensor_id;
        }

        void SensorData::setSensorType(const int32_t &sensor_type) {
            m_sensor_type = sensor_type;
        }
        
        int32_t SensorData::getSensorType() const {
            return m_sensor_type;
        }

        void SensorData::setSensorData(const double &sensor_data) {
            m_sensor_data = sensor_data;
        }
        
        double SensorData::getSensorData() const {
            return m_sensor_data;
        }
        
        
        ostream& SensorData::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, m_sensor_id);
            s->write(2, m_sensor_type);
            s->write(3, m_sensor_data);

            return out;
        }
        
        istream& SensorData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            d->read(1, m_sensor_id);
            d->read(2, m_sensor_type);
            d->read(3, m_sensor_data);
        
            return in;
        }
        
        const string SensorData::toString() const {
            stringstream sstr;

            sstr << "Sensor#" << m_sensor_id << " - ";
            sstr << " SensorType#" << m_sensor_type << " : " << m_sensor_data << " |";

            return sstr.str();
        }
    }
}