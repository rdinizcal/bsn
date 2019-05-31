#include "msg/taskMsg/sphygmomanometer/BloodpressureFilterTaskMsg.hpp"
#include "msg/MessageQueueCodes.hpp"

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            using namespace std;

            BloodpressureFilterTaskMessage::BloodpressureFilterTaskMessage() : dataS(), dataD() {}

            BloodpressureFilterTaskMessage::BloodpressureFilterTaskMessage(const double &_dataS, const double &_dataD) : dataS(_dataS), dataD(_dataD) {}

            BloodpressureFilterTaskMessage::~BloodpressureFilterTaskMessage() {}
            
            BloodpressureFilterTaskMessage::BloodpressureFilterTaskMessage(const BloodpressureFilterTaskMessage &obj) :
                SerializableData(),
                dataS(obj.getDataS()),
                dataD(obj.getDataD())
                {}
                
            
            BloodpressureFilterTaskMessage& BloodpressureFilterTaskMessage::operator=(const BloodpressureFilterTaskMessage &obj) {
                dataS = obj.getDataS();
                dataD = obj.getDataD();
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
            
            
            void BloodpressureFilterTaskMessage::setData(const double &_filterS, const double &_filterD) {
                dataS = _filterS;
                dataD = _filterD;
            }

            double BloodpressureFilterTaskMessage::getDataS() const {
                return dataS;
            }

            double BloodpressureFilterTaskMessage::getDataD() const {
                return dataD;
            }

            ostream& BloodpressureFilterTaskMessage::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, dataS);
                s->write(1, dataD);
                return out;
            }
            
            istream& BloodpressureFilterTaskMessage::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, dataS);
                d->read(2, dataD);
                return in;
            }
            
            const string BloodpressureFilterTaskMessage::toString() const {
                stringstream sstr;

                sstr << "BloodpressureCollectTaskMessage#" << endl;
                sstr << "Data Systolic: " << dataS << endl;
                sstr << "Data Diastolic: " << dataD << endl;

                return sstr.str();
            }
        }
    }
}