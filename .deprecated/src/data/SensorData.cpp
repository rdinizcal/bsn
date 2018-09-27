/*
 * Mensagem com dados do sensor remetente
 * 
 * @author Ricardo Diniz Caldas
 * @version v1.0
 */

#include "bsn/data/SensorData.h"

namespace bsn {
    namespace data {
        
        using namespace std;
        
        SensorData::SensorData() : m_sensor_id(), m_sensor_type(), m_sensor_status(), m_sent_ts() {}
        
        SensorData::SensorData(const uint32_t &sensor_id, const int32_t &sensor_type, const string &sensor_status, const timespec &sent_ts) : 
            m_sensor_id(sensor_id),
            m_sensor_type(sensor_type),
            m_sensor_status(sensor_status),
            m_sent_ts(sent_ts) {}
        
        SensorData::~SensorData() {}
        
        SensorData::SensorData(const SensorData &obj) :
            SerializableData(),
            m_sensor_id(obj.getSensorID()),
            m_sensor_type(obj.getSensorType()),
            m_sensor_status(obj.getSensorStatus()),
            m_sent_ts(obj.getSentTimespec()) {}
        
        SensorData& SensorData::operator=(const SensorData &obj) {
            m_sensor_id = obj.getSensorID();
            m_sensor_type = obj.getSensorType();
            m_sensor_status = obj.getSensorStatus();
            m_sent_ts = obj.getSentTimespec();            
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

        void SensorData::setSensorStatus(const string &sensor_status) {
            m_sensor_status = sensor_status;
        }
        
        string SensorData::getSensorStatus() const {
            return m_sensor_status;
        }
        
        void SensorData::setSentTimespec(const timespec &sent_ts){
            m_sent_ts = sent_ts;
        }

        timespec SensorData::getSentTimespec() const {
            return m_sent_ts;
        }

        ostream& SensorData::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, m_sensor_id);
            s->write(2, m_sensor_type);
            s->write(3, m_sensor_status);
            s->write(4, m_sent_ts.tv_sec);
            s->write(5, m_sent_ts.tv_nsec);

            return out;
        }
        
        istream& SensorData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            d->read(1, m_sensor_id);
            d->read(2, m_sensor_type);
            d->read(3, m_sensor_status);
            d->read(4, m_sent_ts.tv_sec);
            d->read(5, m_sent_ts.tv_nsec);
        
            return in;
        }
        
        const string SensorData::toString() const {
            stringstream sstr;

            sstr << "Sensor#" << m_sensor_id << " - ";
            sstr << " SensorType#" << m_sensor_type << " Status: " << m_sensor_status << "" << endl;

            return sstr.str();
        }
    }
}