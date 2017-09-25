#include "openbasn/message/Request.h"

#include <ostream>
#include <string>

#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

namespace openbasn {
    namespace message {
        
        using namespace std;
        
        Request::Request() : m_request_type(0) {}

        Request::Request(const int32_t &request_type) : m_request_type(request_type){}

        Request::~Request() {}
        
        Request::Request(const Request &obj) :
            SerializableData(),
            m_request_type(obj.getRequestType()) {}
        
        Request& Request::operator=(const Request &obj) {
            m_request_type = obj.getRequestType();
            return (*this);
        }
        
        int32_t Request::ID() {
            return 3;
        }
        const string Request::ShortName() {
            return "Request";
        }
        const string Request::LongName() {
            return "message.Request";
        }
        
        int32_t Request::getID() const {
            return Request::ID();
        }
        const string Request::getShortName() const {
            return Request::ShortName();
        }
        const string Request::getLongName() const {
            return Request::LongName();
        }
        
        int32_t Request::getRequestType() const {
            return m_request_type;
        }
            
        ostream& Request::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
        
            s->write(1, m_request_type);
        
            return out;
        }
        
        istream& Request::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
        
            d->read(1, m_request_type);
        
            return in;
        }
        
        const string Request::toString() const {
            stringstream sstr;
        
            sstr << Request::ShortName() << "#" << Request::ID() << endl;
            sstr << "Type: " << m_request_type << endl; 
            
            return sstr.str();
        }
    }
}