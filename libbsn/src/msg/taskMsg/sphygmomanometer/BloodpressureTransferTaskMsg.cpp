#include "msg/taskMsg/sphygmomanometer/BloodpressureTransferTaskMsg.hpp"
#include "msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            BloodpressureTransferTaskMessage::BloodpressureTransferTaskMessage() : data() {}

            BloodpressureTransferTaskMessage::BloodpressureTransferTaskMessage(const double &_data) : data(_frequency) {}

            BloodpressureTransferTaskMessage::~BloodpressureTransferTaskMessage() {}
            
            BloodpressureTransferTaskMessage::BloodpressureTransferTaskMessage(const BloodpressureTransferTaskMessage &obj) :
                SerializableData(),
                data(obj.getData())
                {}
                
            
            BloodpressureTransferTaskMessage& BloodpressureTransferTaskMessage::operator=(const BloodpressureTransferTaskMessage &obj) {
                data = obj.getData();
                return (*this);
            }
            
            int32_t BloodpressureTransferTaskMessage::ID() {
                return BLOODPRESSURETRANSFERMODULE_MSG_QUE;
            }
            const string BloodpressureTransferTaskMessage::ShortName() {
                return "BloodpressureTransferTaskMessage";
            }
            const string BloodpressureTransferTaskMessage::LongName() {
                return "taskMsg.sphygmomanometer.BloodpressureTransferTaskMessage";
            }
            
            int32_t BloodpressureTransferTaskMessage::getID() const {
                return BloodpressureTransferTaskMessage::ID();
            }
            const string BloodpressureTransferTaskMessage::getShortName() const {
                return BloodpressureTransferTaskMessage::ShortName();
            }
            const string BloodpressureTransferTaskMessage::getLongName() const {
                return BloodpressureTransferTaskMessage::LongName();
            }
            
            
            void BloodpressureTransferTaskMessage::setData(const double &_data) {
                data = _data;
            }

            double BloodpressureTransferTaskMessage::getData() const {
                return data;
            }

            ostream& BloodpressureTransferTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                return out;
            }
            
            istream& BloodpressureTransferTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                return in;
            }
            
            const string BloodpressureTransferTaskMessage::toString() const {
                stringstream sstr;

                sstr << "BloodpressureTransferTaskMessage#" << endl;
                sstr << "Data: " << data << endl;

                return sstr.str();
            }
        }
    }
}