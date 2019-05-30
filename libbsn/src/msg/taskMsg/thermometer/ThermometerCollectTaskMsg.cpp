#include "msg/taskMsg/thermometer/ThermometerCollectTaskMsg.hpp"
#include "msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            ThermometerCollectTaskMessage::ThermometerCollectTaskMessage() : data() {}

            ThermometerCollectTaskMessage::ThermometerCollectTaskMessage(const double &_data) : data(_frequency) {}

            ThermometerCollectTaskMessage::~ThermometerCollectTaskMessage() {}
            
            ThermometerCollectTaskMessage::ThermometerCollectTaskMessage(const ThermometerCollectTaskMessage &obj) :
                SerializableData(),
                data(obj.getData())
                {}
                
            
            ThermometerCollectTaskMessage& ThermometerCollectTaskMessage::operator=(const ThermometerCollectTaskMessage &obj) {
                data = obj.getData();
                return (*this);
            }
            
            int32_t ThermometerCollectTaskMessage::ID() {
                return THERMOMETERCOLLECTMODULE_MSG_QUE;
            }
            const string ThermometerCollectTaskMessage::ShortName() {
                return "ThermometerCollectTaskMessage";
            }
            const string ThermometerCollectTaskMessage::LongName() {
                return "taskMsg.thermometer.ThermometerCollectTaskMessage";
            }
            
            int32_t ThermometerCollectTaskMessage::getID() const {
                return ThermometerCollectTaskMessage::ID();
            }
            const string ThermometerCollectTaskMessage::getShortName() const {
                return ThermometerCollectTaskMessage::ShortName();
            }
            const string ThermometerCollectTaskMessage::getLongName() const {
                return ThermometerCollectTaskMessage::LongName();
            }
            
            
            void ThermometerCollectTaskMessage::setData(const double &_data) {
                data = _data;
            }

            double ThermometerCollectTaskMessage::getData() const {
                return data;
            }

            ostream& ThermometerCollectTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                return out;
            }
            
            istream& ThermometerCollectTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                return in;
            }
            
            const string ThermometerCollectTaskMessage::toString() const {
                stringstream sstr;

                sstr << "ThermometerCollectTaskMessage#" << endl;
                sstr << "Data: " << data << endl;

                return sstr.str();
            }
        }
    }
}