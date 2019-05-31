#include "msg/taskMsg/thermometer/ThermometerTransferTaskMsg.hpp"
#include "msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            ThermometerTransferTaskMessage::ThermometerTransferTaskMessage() : data() {}

            ThermometerTransferTaskMessage::ThermometerTransferTaskMessage(const double &_data) : data(_data) {}

            ThermometerTransferTaskMessage::~ThermometerTransferTaskMessage() {}
            
            ThermometerTransferTaskMessage::ThermometerTransferTaskMessage(const ThermometerTransferTaskMessage &obj) :
                SerializableData(),
                data(obj.getData())
                {}
                
            
            ThermometerTransferTaskMessage& ThermometerTransferTaskMessage::operator=(const ThermometerTransferTaskMessage &obj) {
                data = obj.getData();
                return (*this);
            }
            
            int32_t ThermometerTransferTaskMessage::ID() {
                return THERMTRANSFERMODULE_MSG_QUE;
            }
            const string ThermometerTransferTaskMessage::ShortName() {
                return "ThermometerTransferTaskMessage";
            }
            const string ThermometerTransferTaskMessage::LongName() {
                return "taskMsg.thermometer.ThermometerTransferTaskMessage";
            }
            
            int32_t ThermometerTransferTaskMessage::getID() const {
                return ThermometerTransferTaskMessage::ID();
            }
            const string ThermometerTransferTaskMessage::getShortName() const {
                return ThermometerTransferTaskMessage::ShortName();
            }
            const string ThermometerTransferTaskMessage::getLongName() const {
                return ThermometerTransferTaskMessage::LongName();
            }
            
            
            void ThermometerTransferTaskMessage::setData(const double &_data) {
                data = _data;
            }

            double ThermometerTransferTaskMessage::getData() const {
                return data;
            }

            ostream& ThermometerTransferTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                return out;
            }
            
            istream& ThermometerTransferTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                return in;
            }
            
            const string ThermometerTransferTaskMessage::toString() const {
                stringstream sstr;

                sstr << "ThermometerTransferTaskMessage#" << endl;
                sstr << "Data: " << data << endl;

                return sstr.str();
            }
        }
    }
}