#include "msg/taskMsg/sphygmomanometer/BloodpressureFilterTaskMsg.hpp"
#include "msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            BloodpressureFilterTaskMessage::BloodpressureFilterTaskMessage() : data() {}

            BloodpressureFilterTaskMessage::BloodpressureFilterTaskMessage(const double &_data) : data(_frequency) {}

            BloodpressureFilterTaskMessage::~BloodpressureFilterTaskMessage() {}
            
            BloodpressureFilterTaskMessage::BloodpressureFilterTaskMessage(const BloodpressureFilterTaskMessage &obj) :
                SerializableData(),
                data(obj.getData())
                {}
                
            
            BloodpressureFilterTaskMessage& BloodpressureFilterTaskMessage::operator=(const BloodpressureFilterTaskMessage &obj) {
                data = obj.getData();
                return (*this);
            }
            
            int32_t BloodpressureFilterTaskMessage::ID() {
                return BLOODPRESSUREFILTERMODULE_MSG_QUE;
            }
            const string BloodpressureFilterTaskMessage::ShortName() {
                return "BloodpressureFilterTaskMessage";
            }
            const string BloodpressureFilterTaskMessage::LongName() {
                return "taskMsg.sphygmomanometer.BloodpressureFilterTaskMessage";
            }
            
            int32_t BloodpressureFilterTaskMessage::getID() const {
                return BloodpressureFilterTaskMessage::ID();
            }
            const string BloodpressureFilterTaskMessage::getShortName() const {
                return BloodpressureFilterTaskMessage::ShortName();
            }
            const string BloodpressureFilterTaskMessage::getLongName() const {
                return BloodpressureFilterTaskMessage::LongName();
            }
            
            
            void BloodpressureFilterTaskMessage::setData(const double &_data) {
                data = _data;
            }

            double BloodpressureFilterTaskMessage::getData() const {
                return data;
            }

            ostream& BloodpressureFilterTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                return out;
            }
            
            istream& BloodpressureFilterTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                return in;
            }
            
            const string BloodpressureFilterTaskMessage::toString() const {
                stringstream sstr;

                sstr << "BloodpressureFilterTaskMessage#" << endl;
                sstr << "Data: " << data << endl;

                return sstr.str();
            }
        }
    }
}