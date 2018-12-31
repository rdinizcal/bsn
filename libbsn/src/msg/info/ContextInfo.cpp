#include "msg/info/ContextInfo.hpp"

namespace bsn {
    namespace msg { 
        namespace info {
        
            using namespace std;

            ContextInfo::ContextInfo() : 
                context_id(),
                value() {}
            
            ContextInfo::ContextInfo(const std::string &_context_id, const bool &_value) : 
                context_id(_context_id),
                value(_value) {}
            
            ContextInfo::~ContextInfo() {}
            
            ContextInfo::ContextInfo(const ContextInfo &obj) :
                SerializableData(),
                context_id(obj.getContext()),
                value(obj.getValue()) {}
            
            ContextInfo& ContextInfo::operator=(const ContextInfo &obj) {
                context_id = obj.getContext();
                value = obj.getValue();
                return (*this);
            }
            
            int32_t ContextInfo::ID() {
                return 701;
            }
            const string ContextInfo::ShortName() {
                return "ContextInfo";
            }
            const string ContextInfo::LongName() {
                return "info.ContextInfo";
            }
            
            int32_t ContextInfo::getID() const {
                return ContextInfo::ID();
            }
            const string ContextInfo::getShortName() const {
                return ContextInfo::ShortName();
            }
            const string ContextInfo::getLongName() const {
                return ContextInfo::LongName();
            }

            void ContextInfo::setContext(const std::string &_context_id) {
                context_id = _context_id;
            }

            std::string ContextInfo::getContext() const{
                return context_id;
            }

            void ContextInfo::setValue(const bool &_value) {
                value = _value;
            }

            bool ContextInfo::getValue() const {
                return value;
            }

            ostream& ContextInfo::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, context_id);
                s->write(2, value);

                return out;
            }
            
            istream& ContextInfo::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, context_id);
                d->read(2, value);

                return in;
            }
            
            const string ContextInfo::toString() const {
                stringstream sstr;

                sstr << "ContextInfo#" << endl;
                sstr << "Task: " << context_id << ": " << value << endl; 

                return sstr.str();
            }
        }
    }
}