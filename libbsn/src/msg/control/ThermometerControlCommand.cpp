#include "msg/control/ThermometerControlCommand.hpp"

namespace bsn {
    namespace msg { 
        namespace control {
        
            using namespace std;

            ThermometerControlCommand::ThermometerControlCommand() : active(), params() {}
            
            ThermometerControlCommand::ThermometerControlCommand(const bool &_active, const std::map<std::string,double> &_params) : active (_active), params(_params) {}
            
            ThermometerControlCommand::~ThermometerControlCommand() {}
            
            ThermometerControlCommand::ThermometerControlCommand(const ThermometerControlCommand &obj) :
                SerializableData(),
                active(obj.getActive()),
                params(obj.getParams()){}
            
            ThermometerControlCommand& ThermometerControlCommand::operator=(const ThermometerControlCommand &obj) {
                active = obj.getActive();   
                params = obj.getParams();     
                return (*this);
            }
            
            int32_t ThermometerControlCommand::ID() {
                return 900;
            }
            const string ThermometerControlCommand::ShortName() {
                return "ThermometerControlCommand";
            }
            const string ThermometerControlCommand::LongName() {
                return "control.ThermometerControlCommand";
            }
            
            int32_t ThermometerControlCommand::getID() const {
                return ThermometerControlCommand::ID();
            }
            const string ThermometerControlCommand::getShortName() const {
                return ThermometerControlCommand::ShortName();
            }
            const string ThermometerControlCommand::getLongName() const {
                return ThermometerControlCommand::LongName();
            }
            
            void ThermometerControlCommand::setActive(const bool &_active) {
                active = _active;
            }

            bool ThermometerControlCommand::getActive() const {
                return active;
            }

            void ThermometerControlCommand::setParams(const std::map<std::string,double> &_params) {
                params = _params;
            }
            
            std::map<std::string,double> ThermometerControlCommand::getParams() const {
                return params;
            }

            ostream& ThermometerControlCommand::operator<<(ostream &out) const {
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
            
            istream& ThermometerControlCommand::operator>>(istream &in) {
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
            
            const string ThermometerControlCommand::toString() const {
                stringstream sstr;

                sstr << "ThermometerControlCommand#" << endl;
                sstr << "Activate module: " << active << endl;
                for (auto& x: params){
                    sstr << x.first << ": " << x.second << endl;
                }

                return sstr.str();
            }
        }
    }
}