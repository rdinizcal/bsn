#include "bsn/data/ResourceInfo.h"

namespace bsn {
    namespace data {
        
        using namespace std;
        using namespace bsn::resource;

        ResourceInfo::ResourceInfo() : mResource() {}
        
        ResourceInfo::ResourceInfo(const Resource &rsrc) : 
            mResource(rsrc) {}
        
        ResourceInfo::~ResourceInfo() {}
        
        ResourceInfo::ResourceInfo(const ResourceInfo &obj) :
            SerializableData(),
            mResource(obj.getResource()){}
        
        ResourceInfo& ResourceInfo::operator=(const ResourceInfo &obj) {
            mResource = obj.getResource();        
            return (*this);
        }
        
        int32_t ResourceInfo::ID() {
            return 875;
        }
        const string ResourceInfo::ShortName() {
            return "ResourceInfo";
        }
        const string ResourceInfo::LongName() {
            return "data.ResourceInfo";
        }
        
        int32_t ResourceInfo::getID() const {
            return ResourceInfo::ID();
        }
        const string ResourceInfo::getShortName() const {
            return ResourceInfo::ShortName();
        }
        const string ResourceInfo::getLongName() const {
            return ResourceInfo::LongName();
        }

        void ResourceInfo::setResource(const Resource &_resource) {
            mResource = _resource;
        }
    
        Resource ResourceInfo::getResource() const {
            return mResource;
        }
        
        ostream& ResourceInfo::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
            
            s->write(1, mResource.getName());
            s->write(2, mResource.getCapacity());
            s->write(3, mResource.getCurrentLevel());
            s->write(4, mResource.getUnit());

            return out;
        }
        
        istream& ResourceInfo::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
            
            string id = mResource.getName();
            double cap = mResource.getCapacity();
            double cLv = mResource.getCurrentLevel();
            double unit = mResource.getUnit();

            d->read(1, id);
            d->read(2, cap);
            d->read(3, cLv);
            d->read(4, unit);

            return in;
        }
        
        const string ResourceInfo::toString() const {
            stringstream sstr;

            sstr << "ResourceName#" << mResource.getName() << endl;
            sstr << "Capacity:" << mResource.getCapacity() << " CurrentLevel: " << mResource.getCurrentLevel() << endl;

            return sstr.str();
        }
    }
}