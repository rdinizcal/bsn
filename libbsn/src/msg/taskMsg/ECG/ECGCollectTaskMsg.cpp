#include "msg/taskMsg/ECG/ECGCollectTaskMsg.hpp"
#include "msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            ECGCollectTaskMessage::ECGCollectTaskMessage() : data() {}

            ECGCollectTaskMessage::ECGCollectTaskMessage(const double &_data) : data(_frequency) {}

            ECGCollectTaskMessage::~ECGCollectTaskMessage() {}
            
            ECGCollectTaskMessage::ECGCollectTaskMessage(const ECGCollectTaskMessage &obj) :
                SerializableData(),
                data(obj.getData())
                {}
                
            
            ECGCollectTaskMessage& ECGCollectTaskMessage::operator=(const ECGCollectTaskMessage &obj) {
                data = obj.getData();
                return (*this);
            }
            
            int32_t ECGCollectTaskMessage::ID() {
                return ECGCOLLECTMODULE_MSG_QUE;
            }
            const string ECGCollectTaskMessage::ShortName() {
                return "ECGCollectTaskMessage";
            }
            const string ECGCollectTaskMessage::LongName() {
                return "taskMsg.ECG.ECGCollectTaskMessage";
            }
            
            int32_t ECGCollectTaskMessage::getID() const {
                return ECGCollectTaskMessage::ID();
            }
            const string ECGCollectTaskMessage::getShortName() const {
                return ECGCollectTaskMessage::ShortName();
            }
            const string ECGCollectTaskMessage::getLongName() const {
                return ECGCollectTaskMessage::LongName();
            }
            
            
            void ECGCollectTaskMessage::setData(const double &_data) {
                data = _data;
            }

            double ECGCollectTaskMessage::getData() const {
                return data;
            }

            ostream& ECGCollectTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                return out;
            }
            
            istream& ECGCollectTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                return in;
            }
            
            const string ECGCollectTaskMessage::toString() const {
                stringstream sstr;

                sstr << "ECGCollectTaskMessage#" << endl;
                sstr << "Data: " << data << endl;

                return sstr.str();
            }
        }
    }
}