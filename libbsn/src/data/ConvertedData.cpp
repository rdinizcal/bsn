#include "bsn/data/ConvertedData.h"

namespace bsn {
    namespace data {
        
        using namespace std;
        
        ConvertedData::ConvertedData() : m_sensor_data(), sensorType() {}
        
        ConvertedData::ConvertedData(const double &sensor_data, const std::string &type) : 
            m_sensor_data(sensor_data),
            sensorType(type) {}
        
        ConvertedData::~ConvertedData() {}
        
        ConvertedData::ConvertedData(const ConvertedData &obj) :
            SerializableData(),
            m_sensor_data(obj.getSensorData()),
            sensorType(obj.getSensorType()) {}
        
        ConvertedData& ConvertedData::operator=(const ConvertedData &obj) {
            m_sensor_data = obj.getSensorData();
            sensorType = obj.getSensorType();          
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
        
        ostream& ConvertedData::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, m_sensor_data);
            s->write(2, sensorType);

            return out;
        }
        
        istream& ConvertedData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            d->read(1, m_sensor_data);
            d->read(2, sensorType);
            
            return in;
        }
        
        const string ConvertedData::toString() const {
            stringstream sstr;

            sstr << "Converted Data: " << m_sensor_data << "" << endl;

            return sstr.str();
        }

        void ConvertedData::setSensorType(const string &type) {
            sensorType = type;
        }

        string ConvertedData::getSensorType() const {
            return sensorType;
        }
        
    }
}