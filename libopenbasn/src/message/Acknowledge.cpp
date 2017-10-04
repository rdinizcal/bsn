#include "openbasn/message/Acknowledge.h"

#include <ostream>
#include <string>

#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

namespace openbasn {
    namespace message {
        
        using namespace std;
        
        Acknowledge::Acknowledge() : 
            m_type(0),
            m_source_id(0),
            m_destination_id(0) {}

        Acknowledge::Acknowledge(const int32_t &type, const uint32_t &source_id) : 
            m_type(type),
            m_source_id(source_id), 
            m_destination_id(0) {}

        Acknowledge::Acknowledge(const int32_t &type, const uint32_t &source_id, const uint32_t &destination_id) : 
            m_type(type),
            m_source_id(source_id), 
            m_destination_id(destination_id) {}

        Acknowledge::~Acknowledge() {}
        
        Acknowledge::Acknowledge(const Acknowledge &obj) :
            SerializableData(),
            m_type(obj.getType()),
            m_source_id(obj.getSourceID()),
            m_destination_id(obj.getDestinationID()) {}
        
        Acknowledge& Acknowledge::operator=(const Acknowledge &obj) {
            m_type = obj.getType();
            m_source_id = obj.getSourceID();
            m_destination_id = obj.getDestinationID();
            return (*this);
        }

        int32_t Acknowledge::ID() {
            return 4;
        }
        int32_t Acknowledge::getID() const {
            return 4;
        }
        const string Acknowledge::getShortName() const {
            return "Acknowledge";
        }
        const string Acknowledge::getLongName() const {
            return "message.Acknowledge";
        }
        
        int32_t Acknowledge::getType() const {
            return m_type;
        }

        void Acknowledge::setSourceID(const uint32_t &source_id) {
            m_source_id = source_id;
        }

        uint32_t Acknowledge::getSourceID() const {
            return m_source_id;
        }

        void Acknowledge::setDestinationID(const uint32_t &destination_id) {
            m_destination_id = destination_id;
        }

        uint32_t Acknowledge::getDestinationID() const {
            return m_destination_id;
        }
            
        ostream& Acknowledge::operator<<(ostream &out) const {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
        
            s->write(1, m_type);
            s->write(2, m_source_id);
            s->write(3, m_destination_id);
        
            return out;
        }
        
        istream& Acknowledge::operator>>(istream &in) {
            odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
            std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
        
            d->read(1, m_type);
            d->read(2, m_source_id);
            d->read(3, m_destination_id);
        
            return in;
        }
        
        const string Acknowledge::toString() const {
            stringstream sstr;
        
            sstr << Acknowledge::getShortName() << "#" << Acknowledge::getID() << endl;
            sstr << "Type: " << m_type << endl;
            sstr << "Source ID: " << m_destination_id << endl;
            sstr << "Dest. ID: " << m_destination_id << endl; 
            
            return sstr.str();
        }
    }
}