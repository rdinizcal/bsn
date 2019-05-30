#include "msg/taskMsg/oximeter/OximeterCollectTaskMsg.hpp"
#include "msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            OximeterCollectTaskMessage::OximeterCollectTaskMessage() : data() {}

            OximeterCollectTaskMessage::OximeterCollectTaskMessage(const double &_data) : data(_frequency) {}

            OximeterCollectTaskMessage::~OximeterCollectTaskMessage() {}
            
            OximeterCollectTaskMessage::OximeterCollectTaskMessage(const OximeterCollectTaskMessage &obj) :
                SerializableData(),
                data(obj.getData())
                {}
                
            
            OximeterCollectTaskMessage& OximeterCollectTaskMessage::operator=(const OximeterCollectTaskMessage &obj) {
                data = obj.getData();
                return (*this);
            }
            
            int32_t OximeterCollectTaskMessage::ID() {
                return OXIMETERCOLLECTMODULE_MSG_QUE;
            }
            const string OximeterCollectTaskMessage::ShortName() {
                return "OximeterCollectTaskMessage";
            }
            const string OximeterCollectTaskMessage::LongName() {
                return "taskMsg.oximeter.OximeterCollectTaskMessage";
            }
            
            int32_t OximeterCollectTaskMessage::getID() const {
                return OximeterCollectTaskMessage::ID();
            }
            const string OximeterCollectTaskMessage::getShortName() const {
                return OximeterCollectTaskMessage::ShortName();
            }
            const string OximeterCollectTaskMessage::getLongName() const {
                return OximeterCollectTaskMessage::LongName();
            }
            
            
            void OximeterCollectTaskMessage::setData(const double &_data) {
                data = _data;
            }

            double OximeterCollectTaskMessage::getData() const {
                return data;
            }

            ostream& OximeterCollectTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                return out;
            }
            
            istream& OximeterCollectTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                return in;
            }
            
            const string OximeterCollectTaskMessage::toString() const {
                stringstream sstr;

                sstr << "OximeterCollectTaskMessage#" << endl;
                sstr << "Data: " << data << endl;

                return sstr.str();
            }
        }
    }
}