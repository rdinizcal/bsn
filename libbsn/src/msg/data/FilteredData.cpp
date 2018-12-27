#include "msg/data/FilteredData.h"

namespace bsn {
    namespace msg { 
        namespace data {
        
            using namespace std;
            
            FilteredData::FilteredData() : mSensorData(), sensorType(), myTime() {}
            
            FilteredData::FilteredData(const double &sensorData, const std::string &type, const std::string &ts) : 
                mSensorData(sensorData),
                sensorType(type),
                myTime(ts) {}

            FilteredData::~FilteredData() {}
            
            FilteredData::FilteredData(const FilteredData &obj) :
                SerializableData(),
                mSensorData(obj.getSensorData()),
                sensorType(obj.getSensorType()), 
                myTime(obj.getTime()){}
            
            FilteredData& FilteredData::operator=(const FilteredData &obj) {
                mSensorData = obj.getSensorData();
                sensorType = obj.getSensorType();
                myTime = obj.getTime();          
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

            void FilteredData::setSensorData(const double &sensorData) {
                mSensorData = sensorData;
            }
            
            double FilteredData::getSensorData() const {
                return mSensorData;
            }
            
            void FilteredData::setSensorType(const string &type) {
                sensorType = type;
            }

            string FilteredData::getSensorType() const {
                return sensorType;
            }

            void FilteredData::setTime(const std::string &ts) {
                myTime = ts;
            }

            std::string FilteredData::getTime() const {
                return myTime;
            }
            
            ostream& FilteredData::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, mSensorData);
                s->write(2, sensorType);
                s->write(3, myTime);

                return out;
            }
            
            istream& FilteredData::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, mSensorData);
                d->read(2, sensorType);
                d->read(3, myTime);
                return in;
            }
            
            const string FilteredData::toString() const {
                stringstream sstr;

                sstr << "Filtered Data: " << mSensorData << "" << endl;

                return sstr.str();
            }
        }
    }
}