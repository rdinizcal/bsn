#include "bsn/data/ConvertedData.h"

namespace bsn {
    namespace data {
        
        using namespace std;
        
        ConvertedData::ConvertedData() : m_sensor_data() {}
        
        ConvertedData::ConvertedData(const double &sensor_data) : 
            m_sensor_data(sensor_data) {}
        
        ConvertedData::~ConvertedData() {}
        
        ConvertedData::ConvertedData(const ConvertedData &obj) :
            SerializableData(),
            m_sensor_data(obj.getSensorData()) {}
        
        ConvertedData& ConvertedData::operator=(const ConvertedData &obj) {
            m_sensor_data = obj.getSensorData();          
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

            return out;
        }
        
        istream& ConvertedData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            d->read(1, m_sensor_data);
            
            return in;
        }
        
        const string ConvertedData::toString() const {
            stringstream sstr;

            sstr << "Converted Data: " << m_sensor_data << "" << endl;

            return sstr.str();
        }
    }
}