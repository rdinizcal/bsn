#include "msg/control/BloodpressureControlCommand.hpp"

namespace bsn {
    namespace msg { 
        namespace control {
        
            using namespace std;

            BloodpressureControlCommand::BloodpressureControlCommand() : active(), params() {}
            
            BloodpressureControlCommand::BloodpressureControlCommand(const bool &_active, const std::map<std::string,double> &_params) : active (_active), params(_params) {}
            
            BloodpressureControlCommand::~BloodpressureControlCommand() {}
            
            BloodpressureControlCommand::BloodpressureControlCommand(const BloodpressureControlCommand &obj) :
                SerializableData(),
                active(obj.getActive()),
                params(obj.getParams()){}
            
            BloodpressureControlCommand& BloodpressureControlCommand::operator=(const BloodpressureControlCommand &obj) {
                active = obj.getActive();   
                params = obj.getParams();     
                return (*this);
            }
            
            int32_t BloodpressureControlCommand::ID() {
                return 903;
            }
            const string BloodpressureControlCommand::ShortName() {
                return "BloodpressureControlCommand";
            }
            const string BloodpressureControlCommand::LongName() {
                return "control.BloodpressureControlCommand";
            }
            
            int32_t BloodpressureControlCommand::getID() const {
                return BloodpressureControlCommand::ID();
            }
            const string BloodpressureControlCommand::getShortName() const {
                return BloodpressureControlCommand::ShortName();
            }
            const string BloodpressureControlCommand::getLongName() const {
                return BloodpressureControlCommand::LongName();
            }
            
            void BloodpressureControlCommand::setActive(const bool &_active) {
                active = _active;
            }

            bool BloodpressureControlCommand::getActive() const {
                return active;
            }

            void BloodpressureControlCommand::setParams(const std::map<std::string,double> &_params) {
                params = _params;
            }
            
            std::map<std::string,double> BloodpressureControlCommand::getParams() const {
                return params;
            }

            ostream& BloodpressureControlCommand::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, active);
                s->write(2, params.size());

                int32_t i = 2;
                for (auto& x: params){
                    s->write(++i,x.first);
                    s->write(++i,x.second);
                }

                return out;
            }
            
            istream& BloodpressureControlCommand::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                int32_t size; 

                d->read(1, active);
                d->read(2, size);

                int32_t i = 2;
                while(i<size) {
                    std::string key;
                    double value;

                    d->read(++i, key);
                    d->read(++i, value);

                    params.insert ( std::pair<std::string,double>(key,value) );
                }

                return in;
            }
            
            const string BloodpressureControlCommand::toString() const {
                stringstream sstr;

                sstr << "BloodpressureControlCommand#" << endl;
                sstr << "Activate module: " << active << endl;
                for (auto& x: params){
                    sstr << x.first << ": " << x.second << endl;
                }

                return sstr.str();
            }
        }
    }
}