#include "msg/data/SpecData.h"

namespace bsn {
    namespace msg { 
        namespace data {
        
            using namespace std;
            
            SpecData::SpecData() : data(), type() {}
            
            SpecData::SpecData(const double &_data, const std::string &_type) : 
                data(_data),
                type(_type){}

            SpecData::~SpecData() {}
            
            SpecData::SpecData(const SpecData &obj) :
                SerializableData(),
                data(obj.getData()),
                type(obj.getType()){}
            
            SpecData& SpecData::operator=(const SpecData &obj) {
                data = obj.getData();
                type = obj.getType();
                return (*this);
            }
            
            int32_t SpecData::ID() {
                return 800;
            }
            const string SpecData::ShortName() {
                return "SpecData";
            }
            const string SpecData::LongName() {
                return "data.SpecData";
            }
            
            int32_t SpecData::getID() const {
                return SpecData::ID();
            }

            const string SpecData::getShortName() const {
                return SpecData::ShortName();
            }

            const string SpecData::getLongName() const {
                return SpecData::LongName();
            }

            void SpecData::setData(const double &_data) {
                data = _data;
            }
            
            double SpecData::getData() const {
                return data;
            }
            
            void SpecData::setType(const string &_type) {
                type = _type;
            }

            string SpecData::getType() const {
                return type;
            }
            
            ostream& SpecData::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                s->write(2, type);

                return out;
            }
            
            istream& SpecData::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                d->read(2, type);
                return in;
            }
            
            const string SpecData::toString() const {
                stringstream sstr;

                sstr << "---------------------" << endl;
                sstr << "type: " << type << endl;
                sstr << "data: " << data << endl;
                sstr << "---------------------" << endl;

                return sstr.str();
            }
        }
    }
}