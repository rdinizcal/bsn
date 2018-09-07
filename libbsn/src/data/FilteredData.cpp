#include "bsn/data/FilteredData.h"

namespace bsn {
    namespace data {
        
        using namespace std;
        
        FilteredData::FilteredData() : m_sensor_data(), sensorType() {}
        
        FilteredData::FilteredData(const double &sensor_data, const std::string &type) : 
            m_sensor_data(sensor_data),
            sensorType(type) {}
        
        FilteredData::~FilteredData() {}
        
        FilteredData::FilteredData(const FilteredData &obj) :
            SerializableData(),
            m_sensor_data(obj.getSensorData()),
            sensorType(obj.getSensorType()) {}
        
        FilteredData& FilteredData::operator=(const FilteredData &obj) {
            m_sensor_data = obj.getSensorData();
            sensorType = obj.getSensorType();          
            return (*this);
        }
        
        int32_t FilteredData::ID() {
            return 876;
        }
        const string FilteredData::ShortName() {
            return "FilteredData";
        }
        const string FilteredData::LongName() {
            return "data.FilteredData";
        }
        
        int32_t FilteredData::getID() const {
            return FilteredData::ID();
        }

        const string FilteredData::getShortName() const {
            return FilteredData::ShortName();
        }

        const string FilteredData::getLongName() const {
            return FilteredData::LongName();
        }

        void FilteredData::setSensorData(const double &sensor_data) {
            m_sensor_data = sensor_data;
        }
        
        double FilteredData::getSensorData() const {
            return m_sensor_data;
        }
        
        ostream& FilteredData::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, m_sensor_data);
            s->write(2, sensorType);

            return out;
        }
        
        istream& FilteredData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            d->read(1, m_sensor_data);
            d->read(2, sensorType);
            
            return in;
        }
        
        const string FilteredData::toString() const {
            stringstream sstr;

            sstr << "Filtered Data: " << m_sensor_data << "" << endl;

            return sstr.str();
        }

        void FilteredData::setSensorType(const string &type) {
            sensorType = type;
        }

        string FilteredData::getSensorType() const {
            return sensorType;
        }

    }
}