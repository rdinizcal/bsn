#include "bsn/data/RawData.h"

namespace bsn {
    namespace data {
        
        using namespace std;
        
        RawData::RawData() : mSensorData(), mSensorType(), mTime() {}
        
        RawData::RawData(const double &sensorData,const std::string &sensorType ,const std::string &ts): 
            mSensorData(sensorData),
            mSensorType(sensorType),
            mTime(ts) {}
        
        RawData::~RawData() {}
        
        RawData::RawData(const RawData &obj) :
            SerializableData(),
            mSensorData(obj.getSensorData()),
            mSensorType(obj.getSensorType()),
            mTime(obj.getTime()) {}
        
        RawData& RawData::operator=(const RawData &obj) {
            mSensorData = obj.getSensorData();
            mSensorType = obj.getSensorType();
            mTime = obj.getTime();          
            return (*this);
        }
        
        int32_t RawData::ID() {
            return 880;
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

        void RawData::setSensorData(const double &sensorData) {
            mSensorData = sensorData;
        }
        
        double RawData::getSensorData() const {
            return mSensorData;
        }

        void RawData::setTime(const std::string &ts) {
            mTime = ts;
        }

        std::string RawData::getTime() const {
            return mTime;
        }

        void RawData::setSensorType(const string &type) {
            mSensorType = type;
        }

        string RawData::getSensorType() const {
            return mSensorType;
        }
        
        ostream& RawData::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, mSensorData);
            s->write(2, mSensorType);
            s->write(3, mTime);

            return out;
        }
        
        istream& RawData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            d->read(1, mSensorData);
            d->read(2, mSensorType);
            d->read(3, mTime);

            return in;
        }
        
        const string RawData::toString() const {
            stringstream sstr;

            sstr << "Raw Data: " << mSensorData << "" << endl;

            return sstr.str();
        }
    }
}