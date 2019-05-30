#include "msg/taskMsg/sphygmomanometer/BloodpressureCollectTaskMsg.hpp"
#include "msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            BloodpressureCollectTaskMessage::BloodpressureCollectTaskMessage() : data() {}

            BloodpressureCollectTaskMessage::BloodpressureCollectTaskMessage(const double &_data) : data(_frequency) {}

            BloodpressureCollectTaskMessage::~BloodpressureCollectTaskMessage() {}
            
            BloodpressureCollectTaskMessage::BloodpressureCollectTaskMessage(const BloodpressureCollectTaskMessage &obj) :
                SerializableData(),
                data(obj.getData())
                {}
                
            
            BloodpressureCollectTaskMessage& BloodpressureCollectTaskMessage::operator=(const BloodpressureCollectTaskMessage &obj) {
                data = obj.getData();
                return (*this);
            }
            
            int32_t BloodpressureCollectTaskMessage::ID() {
                return BLOODPRESSURECOLLECTMODULE_MSG_QUE;
            }
            const string BloodpressureCollectTaskMessage::ShortName() {
                return "BloodpressureCollectTaskMessage";
            }
            const string BloodpressureCollectTaskMessage::LongName() {
                return "taskMsg.sphygmomanometer.BloodpressureCollectTaskMessage";
            }
            
            int32_t BloodpressureCollectTaskMessage::getID() const {
                return BloodpressureCollectTaskMessage::ID();
            }
            const string BloodpressureCollectTaskMessage::getShortName() const {
                return BloodpressureCollectTaskMessage::ShortName();
            }
            const string BloodpressureCollectTaskMessage::getLongName() const {
                return BloodpressureCollectTaskMessage::LongName();
            }
            
            
            void BloodpressureCollectTaskMessage::setData(const double &_data) {
                data = _data;
            }

            double BloodpressureCollectTaskMessage::getData() const {
                return data;
            }

            ostream& BloodpressureCollectTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, data);
                return out;
            }
            
            istream& BloodpressureCollectTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, data);
                return in;
            }
            
            const string BloodpressureCollectTaskMessage::toString() const {
                stringstream sstr;

                sstr << "BloodpressureCollectTaskMessage#" << endl;
                sstr << "Data: " << data << endl;

                return sstr.str();
            }
        }
    }
}