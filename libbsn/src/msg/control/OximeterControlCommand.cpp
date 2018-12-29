#include "msg/control/OximeterControlCommand.hpp"

namespace bsn {
    namespace msg { 
        namespace control {
        
            using namespace std;

            OximeterControlCommand::OximeterControlCommand() : active(), params() {}
            
            OximeterControlCommand::OximeterControlCommand(const bool &_active, const std::map<std::string,double> &_params) : active (_active), params(_params) {}
            
            OximeterControlCommand::~OximeterControlCommand() {}
            
            OximeterControlCommand::OximeterControlCommand(const OximeterControlCommand &obj) :
                SerializableData(),
                active(obj.getActive()),
                params(obj.getParams()){}
            
            OximeterControlCommand& OximeterControlCommand::operator=(const OximeterControlCommand &obj) {
                active = obj.getActive();   
                params = obj.getParams();     
                return (*this);
            }
            
            int32_t OximeterControlCommand::ID() {
                return 901;
            }
            const string OximeterControlCommand::ShortName() {
                return "OximeterControlCommand";
            }
            const string OximeterControlCommand::LongName() {
                return "control.OximeterControlCommand";
            }
            
            int32_t OximeterControlCommand::getID() const {
                return OximeterControlCommand::ID();
            }
            const string OximeterControlCommand::getShortName() const {
                return OximeterControlCommand::ShortName();
            }
            const string OximeterControlCommand::getLongName() const {
                return OximeterControlCommand::LongName();
            }
            
            void OximeterControlCommand::setActive(const bool &_active) {
                active = _active;
            }

            bool OximeterControlCommand::getActive() const {
                return active;
            }

            void OximeterControlCommand::setParams(const std::map<std::string,double> &_params) {
                params = _params;
            }
            
            std::map<std::string,double> OximeterControlCommand::getParams() const {
                return params;
            }

            ostream& OximeterControlCommand::operator<<(ostream &out) const {
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
            
            istream& OximeterControlCommand::operator>>(istream &in) {
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
            
            const string OximeterControlCommand::toString() const {
                stringstream sstr;

                sstr << "OximeterControlCommand#" << endl;
                sstr << "Activate module: " << active << endl;
                for (auto& x: params){
                    sstr << x.first << ": " << x.second << endl;
                }

                return sstr.str();
            }
        }
    }
}