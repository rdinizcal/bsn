#include "msg/taskMsg/oximeter/OximeterFilterTaskMsg.hpp"
#include "msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            OximeterFilterTaskMessage::OximeterFilterTaskMessage() : data() {}

            OximeterFilterTaskMessage::OximeterFilterTaskMessage(const double &_data) : data(_frequency) {}

            OximeterFilterTaskMessage::~OximeterFilterTaskMessage() {}
            
            OximeterFilterTaskMessage::OximeterFilterTaskMessage(const OximeterFilterTaskMessage &obj) :
                SerializableData(),
                data(obj.getData())
                {}
                
            
            OximeterFilterTaskMessage& OximeterFilterTaskMessage::operator=(const OximeterFilterTaskMessage &obj) {
                data = obj.getData();
                return (*this);
            }
            
            int32_t OximeterFilterTaskMessage::ID() {
                return OXIMETERFILTERMODULE_MSG_QUE;
            }
            const string OximeterFilterTaskMessage::ShortName() {
                return "OximeterFilterTaskMessage";
            }
            const string OximeterFilterTaskMessage::LongName() {
                return "taskMsg.oximeter.OximeterFilterTaskMessage";
            }
            
            int32_t OximeterFilterTaskMessage::getID() const {
                return OximeterFilterTaskMessage::ID();
            }
            const string OximeterFilterTaskMessage::getShortName() const {
                return OximeterFilterTaskMessage::ShortName();
            }
            const string OximeterFilterTaskMessage::getLongName() const {
                return OximeterFilterTaskMessage::LongName();
            }
            
            
            void OximeterFilterTaskMessage::setData(const double &_data) {
                data = _data;
            }

            double OximeterFilterTaskMessage::getData() const {
                return data;
            }

            ostream& OximeterFilterTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                return out;
            }
            
            istream& OximeterFilterTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                return in;
            }
            
            const string OximeterFilterTaskMessage::toString() const {
                stringstream sstr;

                sstr << "OximeterFilterTaskMessage#" << endl;
                sstr << "Data: " << data << endl;

                return sstr.str();
            }
        }
    }
}