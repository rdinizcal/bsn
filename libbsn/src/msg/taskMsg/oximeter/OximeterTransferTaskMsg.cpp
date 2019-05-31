#include "bsn/msg/taskMsg/oximeter/OximeterTransferTaskMsg.hpp"
#include "bsn/msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            OximeterTransferTaskMessage::OximeterTransferTaskMessage() : data() {}

            OximeterTransferTaskMessage::OximeterTransferTaskMessage(const double &_data) : data(_data) {}

            OximeterTransferTaskMessage::~OximeterTransferTaskMessage() {}
            
            OximeterTransferTaskMessage::OximeterTransferTaskMessage(const OximeterTransferTaskMessage &obj) :
                SerializableData(),
                data(obj.getData())
                {}
                
            
            OximeterTransferTaskMessage& OximeterTransferTaskMessage::operator=(const OximeterTransferTaskMessage &obj) {
                data = obj.getData();
                return (*this);
            }
            
            int32_t OximeterTransferTaskMessage::ID() {
                return OXIMETERTRANSFERMODULE_MSG_QUE;
            }
            const string OximeterTransferTaskMessage::ShortName() {
                return "OximeterTransferTaskMessage";
            }
            const string OximeterTransferTaskMessage::LongName() {
                return "taskMsg.oximeter.OximeterTransferTaskMessage";
            }
            
            int32_t OximeterTransferTaskMessage::getID() const {
                return OximeterTransferTaskMessage::ID();
            }
            const string OximeterTransferTaskMessage::getShortName() const {
                return OximeterTransferTaskMessage::ShortName();
            }
            const string OximeterTransferTaskMessage::getLongName() const {
                return OximeterTransferTaskMessage::LongName();
            }
            
            
            void OximeterTransferTaskMessage::setData(const double &_data) {
                data = _data;
            }

            double OximeterTransferTaskMessage::getData() const {
                return data;
            }

            ostream& OximeterTransferTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                return out;
            }
            
            istream& OximeterTransferTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                return in;
            }
            
            const string OximeterTransferTaskMessage::toString() const {
                stringstream sstr;

                sstr << "OximeterTransferTaskMessage#" << endl;
                sstr << "Data: " << data << endl;

                return sstr.str();
            }
        }
    }
}