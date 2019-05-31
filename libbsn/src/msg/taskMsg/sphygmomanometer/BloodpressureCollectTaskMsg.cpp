#include "bsn/msg/taskMsg/sphygmomanometer/BloodpressureCollectTaskMsg.hpp"
#include "bsn/msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            BloodpressureCollectTaskMessage::BloodpressureCollectTaskMessage() : dataS(), dataD() {}

            BloodpressureCollectTaskMessage::BloodpressureCollectTaskMessage(const double &_dataS, const double &_dataD) : dataS(_dataS), dataD(_dataD){}

            BloodpressureCollectTaskMessage::~BloodpressureCollectTaskMessage() {}
            
            BloodpressureCollectTaskMessage::BloodpressureCollectTaskMessage(const BloodpressureCollectTaskMessage &obj) :
                SerializableData(),
                dataS(obj.getDataS()),
                dataD(obj.getDataD())
                {}
                
            
            BloodpressureCollectTaskMessage& BloodpressureCollectTaskMessage::operator=(const BloodpressureCollectTaskMessage &obj) {
                dataS = obj.getDataS();
                dataD = obj.getDataD();
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
            
            
            void BloodpressureCollectTaskMessage::setData(const double &_dataS, const double &_dataD) {
                dataS = _dataS;
                dataD = _dataD;
            }

            double BloodpressureCollectTaskMessage::getDataS() const {
                return dataS;
            }
            
            double BloodpressureCollectTaskMessage::getDataD() const {
                return dataD;
            }

            ostream& BloodpressureCollectTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, dataS);
                s->write(2, dataD);
                return out;
            }
            
            istream& BloodpressureCollectTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, dataS);
                d->read(2, dataD);
                return in;
            }
            
            const string BloodpressureCollectTaskMessage::toString() const {
                stringstream sstr;

                sstr << "BloodpressureCollectTaskMessage#" << endl;
                sstr << "Data Systolic: " << dataS << endl;
                sstr << "Data Diastolic: " << dataD << endl;

                return sstr.str();
            }
        }
    }
}