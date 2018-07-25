#include "bsn/data/RawData.h"

namespace bsn {
    namespace data {
        
        using namespace std;
        
        RawData::RawData() : m_sensor_data() {}
        
        RawData::RawData(const double &sensor_data): 
            m_sensor_data(sensor_data) {}
        
        RawData::~RawData() {}
        
        RawData::RawData(const RawData &obj) :
            SerializableData(),
            m_sensor_data(obj.getSensorData()) {}
        
        RawData& RawData::operator=(const RawData &obj) {
            m_sensor_data = obj.getSensorData();          
            return (*this);
        }
        
        int32_t RawData::ID() {
            return 874;
        }
        const string RawData::ShortName() {
            return "RawData";
        }
        const string RawData::LongName() {
            return "data.RawData";
        }
        
        int32_t RawData::getID() const {
            return RawData::ID();
        }

        const string RawData::getShortName() const {
            return RawData::ShortName();
        }

        const string RawData::getLongName() const {
            return RawData::LongName();
        }

        void RawData::setSensorData(const double &sensor_data) {
            m_sensor_data = sensor_data;
        }
        
        double RawData::getSensorData() const {
            return m_sensor_data;
        }
        
        ostream& RawData::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, m_sensor_data);

            return out;
        }
        
        istream& RawData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            d->read(1, m_sensor_data);
            
            return in;
        }
        
        const string RawData::toString() const {
            stringstream sstr;

            sstr << "Raw Data: " << m_sensor_data << "" << endl;

            return sstr.str();
        }
    }
}