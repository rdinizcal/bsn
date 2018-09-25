#include "bsn/data/ConvertedData.h"

namespace bsn {
    namespace data {
        
        using namespace std;
        
        ConvertedData::ConvertedData() : m_sensor_data(), sensorType() {}
        
        ConvertedData::ConvertedData(const double &sensor_data, const string &type, const std::array<timespec, 2> &lts) : 
            m_sensor_data(sensor_data),
            sensorType(type),
            time_v(lts) {}
        
        ConvertedData::~ConvertedData() {}
        
        ConvertedData::ConvertedData(const ConvertedData &obj) :
            SerializableData(),
            m_sensor_data(obj.getSensorData()),
            sensorType(obj.getSensorType()),
            time_v(obj.getTimespec()) {}
        
        ConvertedData& ConvertedData::operator=(const ConvertedData &obj) {
            m_sensor_data = obj.getSensorData();
            sensorType = obj.getSensorType();
            time_v = obj.getTimespec();          
            return (*this);
        }
        
        int32_t ConvertedData::ID() {
            return 875;
        }
        const string ConvertedData::ShortName() {
            return "ConvertedData";
        }
        const string ConvertedData::LongName() {
            return "data.ConvertedData";
        }
        
        int32_t ConvertedData::getID() const {
            return ConvertedData::ID();
        }

        const string ConvertedData::getShortName() const {
            return ConvertedData::ShortName();
        }

        const string ConvertedData::getLongName() const {
            return ConvertedData::LongName();
        }

        void ConvertedData::setSensorData(const double &sensor_data) {
            m_sensor_data = sensor_data;
        }
        
        double ConvertedData::getSensorData() const {
            return m_sensor_data;
        }
        
        const string ConvertedData::toString() const {
            stringstream sstr;

            sstr << "Converted Data: " << m_sensor_data << "" << endl;

            return sstr.str();
        }

        void ConvertedData::setSensorType(const string &type) {
            sensorType = type;
        }

        void ConvertedData::setTimespec(const std::array<timespec, 2> &ts) {
            time_v = ts;
        }

        std::array<timespec, 2> ConvertedData::getTimespec() const {
            return time_v;
        }

        ostream& ConvertedData::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, m_sensor_data);
            s->write(2, sensorType);
            s->write(3, time_v[0].tv_sec);
            s->write(4, time_v[0].tv_nsec);
            s->write(5, time_v[1].tv_sec);
            s->write(6, time_v[1].tv_nsec);

            return out;
        }
        
        istream& ConvertedData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            d->read(1, m_sensor_data);
            d->read(2, sensorType);
            d->read(3, time_v[0].tv_sec);
            d->read(4, time_v[0].tv_nsec);
            d->read(5, time_v[1].tv_sec);
            d->read(6, time_v[1].tv_nsec);

            return in;
        }
        

        string ConvertedData::getSensorType() const {
            return sensorType;
        }
        }
}