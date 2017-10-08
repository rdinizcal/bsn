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
            m_type(0),
            m_source_id(0),
            m_destination_id(0),
            m_content() {}

        Request::Request(const int32_t &request_type, const uint32_t &source_id) : 
            m_type(request_type),
            m_source_id(source_id), 
            m_destination_id(0),
            m_content() {}
        
        Request::Request(const int32_t &request_type, const uint32_t &source_id, const string &content) : 
            m_type(request_type),
            m_source_id(source_id), 
            m_destination_id(0),
            m_content(content) {}

        Request::Request(const int32_t &request_type, const uint32_t &source_id, const uint32_t &destination_id) : 
            m_type(request_type),
            m_source_id(source_id), 
            m_destination_id(destination_id),
            m_content() {}

        Request::Request(const int32_t &request_type, const uint32_t &source_id, const uint32_t &destination_id, const string &content) : 
            m_type(request_type),
            m_source_id(source_id), 
            m_destination_id(destination_id),
            m_content(content) {}

        Request::~Request() {}
        
        Request::Request(const Request &obj) :
            SerializableData(),
            m_type(obj.getType()),
            m_source_id(obj.getSourceID()),
            m_destination_id(obj.getDestinationID()),
            m_content(obj.getContent()) {}
        
        Request& Request::operator=(const Request &obj) {
            m_type = obj.getType();
            m_source_id = obj.getSourceID();
            m_destination_id = obj.getDestinationID();
            m_content = obj.getContent();
            return (*this);
        }

        int32_t Request::ID() {
            return 872;
        }
        int32_t Request::getID() const {
            return 872;
        }
        const string Request::getShortName() const {
            return "Request";
        }
        const string Request::getLongName() const {
            return "message.Request";
        }
        
        int32_t Request::getType() const {
            return m_type;
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

        void Request::setContent(const uint32_t &content) {
            m_content = content;
        }

        string Request::getContent() const {
            return m_content;
        }
            
        ostream& Request::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
        
            s->write(1, m_type);
            s->write(2, m_source_id);
            s->write(3, m_destination_id);
            s->write(4, m_content);
        
            return out;
        }
        
        istream& Request::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
        
            d->read(1, m_type);
            d->read(2, m_source_id);
            d->read(3, m_destination_id);
            d->read(4, m_content);
        
            return in;
        }
        
        const string Request::toString() const {
            stringstream sstr;
        
            sstr << Request::getShortName() << "#" << Request::getID() << endl;
            sstr << "Type: " << m_type;
            sstr << "| Source: " << m_source_id;
            sstr << "| Destination: " << m_destination_id; 
            sstr << "| Content: " << m_content << endl; 
            
            return sstr.str();
        }
    }
}