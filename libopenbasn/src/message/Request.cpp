#include "openbasn/message/Request.h"

#include <ostream>
#include <string>

#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

namespace openbasn {
    namespace message {
        
        using namespace std;
        
        Request::Request() : 
            m_request_type(0),
            m_source_id(0),
            m_destination_id(0) {}

        Request::Request(const int32_t &request_type, const uint32_t &source_id) : 
            m_request_type(request_type),
            m_source_id(source_id), 
            m_destination_id(0) {}

        Request::Request(const int32_t &request_type, const uint32_t &source_id, const uint32_t &destination_id) : 
            m_request_type(request_type),
            m_source_id(source_id), 
            m_destination_id(destination_id) {}

        Request::~Request() {}
        
        Request::Request(const Request &obj) :
            SerializableData(),
            m_request_type(obj.getRequestType()),
            m_source_id(obj.getSourceID()),
            m_destination_id(obj.getDestinationID()) {}
        
        Request& Request::operator=(const Request &obj) {
            m_request_type = obj.getRequestType();
            m_source_id = obj.getSourceID();
            m_destination_id = obj.getDestinationID();
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

        void Request::setSourceID(const uint32_t &source_id) {
            m_source_id = source_id;
        }

        uint32_t Request::getSourceID() const {
            return m_source_id;
        }

        void Request::setDestinationID(const uint32_t &destination_id) {
            m_destination_id = destination_id;
        }

        uint32_t Request::getDestinationID() const {
            return m_destination_id;
        }
            
        ostream& Request::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
        
            s->write(1, m_request_type);
            s->write(2, m_source_id);
            s->write(3, m_destination_id);
        
            return out;
        }
        
        istream& Request::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
        
            d->read(1, m_request_type);
            d->read(2, m_source_id);
            d->read(3, m_destination_id);
        
            return in;
        }
        
        const string Request::toString() const {
            stringstream sstr;
        
            sstr << Request::ShortName() << "#" << Request::ID() << endl;
            sstr << "Type: " << m_request_type << endl;
            sstr << "Source ID: " << m_destination_id << endl;
            sstr << "Dest. ID: " << m_destination_id << endl; 
            
            return sstr.str();
        }
    }
}