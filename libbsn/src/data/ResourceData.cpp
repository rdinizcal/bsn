#include "bsn/data/ResourceData.h"

namespace bsn {
    namespace data {
        
        using namespace std;
        using namespace bsn::resource;

        ResourceData::ResourceData() : mResource() {}
        
        ResourceData::ResourceData(const Resource &resource) : 
            mResource(resource) {}
        
        ResourceData::~ResourceData() {}
        
        ResourceData::ResourceData(const ResourceData &obj) :
            SerializableData(),
            mResource(obj.getResource()){}
        
        ResourceData& ResourceData::operator=(const ResourceData &obj) {
            mResource = obj.getResource();        
            return (*this);
        }
        
        int32_t ResourceData::ID() {
            return 875;
        }
        const string ResourceData::ShortName() {
            return "ResourceData";
        }
        const string ResourceData::LongName() {
            return "data.ResourceData";
        }
        
        int32_t ResourceData::getID() const {
            return ResourceData::ID();
        }
        const string ResourceData::getShortName() const {
            return ResourceData::ShortName();
        }
        const string ResourceData::getLongName() const {
            return ResourceData::LongName();
        }
        
        ostream& ResourceData::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, identifier);
            s->write(2, capacity);
            s->write(3, currentLevel);
            s->write(4, unit);

            return out;
        }
        
        istream& ResourceData::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            d->read(1, identifier);
            d->read(2, capacity);
            d->read(3, currentLevel);
            d->read(4, unit);

            return in;
        }
        
        const string ResourceData::toString() const {
            stringstream sstr;

            sstr << "ResourceID#" << identifier << endl;
            sstr << "Capacity:" << capacity << " CurrentLevel: " << currentLevel << endl;

            return sstr.str();
        }
    }
}