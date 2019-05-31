#include "bsn/msg/taskMsg/ECG/ECGTransferTaskMsg.hpp"
#include "bsn/msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            ECGTransferTaskMessage::ECGTransferTaskMessage() : data() {}

            ECGTransferTaskMessage::ECGTransferTaskMessage(const double &_data) : data(_data) {}

            ECGTransferTaskMessage::~ECGTransferTaskMessage() {}
            
            ECGTransferTaskMessage::ECGTransferTaskMessage(const ECGTransferTaskMessage &obj) :
                SerializableData(),
                data(obj.getData())
                {}
                
            
            ECGTransferTaskMessage& ECGTransferTaskMessage::operator=(const ECGTransferTaskMessage &obj) {
                data = obj.getData();
                return (*this);
            }
            
            int32_t ECGTransferTaskMessage::ID() {
                return ECGTRANSFERMODULE_MSG_QUE;
            }
            const string ECGTransferTaskMessage::ShortName() {
                return "ECGTransferTaskMessage";
            }
            const string ECGTransferTaskMessage::LongName() {
                return "taskMsg.ECG.ECGTransferTaskMessage";
            }
            
            int32_t ECGTransferTaskMessage::getID() const {
                return ECGTransferTaskMessage::ID();
            }
            const string ECGTransferTaskMessage::getShortName() const {
                return ECGTransferTaskMessage::ShortName();
            }
            const string ECGTransferTaskMessage::getLongName() const {
                return ECGTransferTaskMessage::LongName();
            }
            
            
            void ECGTransferTaskMessage::setData(const double &_data) {
                data = _data;
            }

            double ECGTransferTaskMessage::getData() const {
                return data;
            }

            ostream& ECGTransferTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                return out;
            }
            
            istream& ECGTransferTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                return in;
            }
            
            const string ECGTransferTaskMessage::toString() const {
                stringstream sstr;

                sstr << "ECGTransferTaskMessage#" << endl;
                sstr << "Data: " << data << endl;

                return sstr.str();
            }
        }
    }
}
