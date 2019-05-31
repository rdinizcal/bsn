#include "msg/taskMsg/ECG/ECGFilterTaskMsg.hpp"
#include "msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            ECGFilterTaskMessage::ECGFilterTaskMessage() : data() {}

            ECGFilterTaskMessage::ECGFilterTaskMessage(const double &_data) : data(_data) {}

            ECGFilterTaskMessage::~ECGFilterTaskMessage() {}
            
            ECGFilterTaskMessage::ECGFilterTaskMessage(const ECGFilterTaskMessage &obj) :
                SerializableData(),
                data(obj.getData())
                {}
                
            
            ECGFilterTaskMessage& ECGFilterTaskMessage::operator=(const ECGFilterTaskMessage &obj) {
                data = obj.getData();
                return (*this);
            }
            
            int32_t ECGFilterTaskMessage::ID() {
                return ECGFILTERMODULE_MSG_QUE;
            }
            const string ECGFilterTaskMessage::ShortName() {
                return "ECGFilterTaskMessage";
            }
            const string ECGFilterTaskMessage::LongName() {
                return "taskMsg.ECG.ECGFilterTaskMessage";
            }
            
            int32_t ECGFilterTaskMessage::getID() const {
                return ECGFilterTaskMessage::ID();
            }
            const string ECGFilterTaskMessage::getShortName() const {
                return ECGFilterTaskMessage::ShortName();
            }
            const string ECGFilterTaskMessage::getLongName() const {
                return ECGFilterTaskMessage::LongName();
            }
            
            
            void ECGFilterTaskMessage::setData(const double &_data) {
                data = _data;
            }

            double ECGFilterTaskMessage::getData() const {
                return data;
            }

            ostream& ECGFilterTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                return out;
            }
            
            istream& ECGFilterTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                return in;
            }
            
            const string ECGFilterTaskMessage::toString() const {
                stringstream sstr;

                sstr << "ECGFilterTaskMessage#" << endl;
                sstr << "Data: " << data << endl;

                return sstr.str();
            }
        }
    }
}
