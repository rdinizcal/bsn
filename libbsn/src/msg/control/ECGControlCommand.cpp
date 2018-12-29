#include "msg/control/ECGControlCommand.hpp"

namespace bsn {
    namespace msg { 
        namespace control {
        
            using namespace std;

            ECGControlCommand::ECGControlCommand() : active(), params() {}
            
            ECGControlCommand::ECGControlCommand(const bool &_active, const std::map<std::string,double> &_params) : active (_active), params(_params) {}
            
            ECGControlCommand::~ECGControlCommand() {}
            
            ECGControlCommand::ECGControlCommand(const ECGControlCommand &obj) :
                SerializableData(),
                active(obj.getActive()),
                params(obj.getParams()){}
            
            ECGControlCommand& ECGControlCommand::operator=(const ECGControlCommand &obj) {
                active = obj.getActive();   
                params = obj.getParams();     
                return (*this);
            }
            
            int32_t ECGControlCommand::ID() {
                return 902;
            }
            const string ECGControlCommand::ShortName() {
                return "ECGControlCommand";
            }
            const string ECGControlCommand::LongName() {
                return "control.ECGControlCommand";
            }
            
            int32_t ECGControlCommand::getID() const {
                return ECGControlCommand::ID();
            }
            const string ECGControlCommand::getShortName() const {
                return ECGControlCommand::ShortName();
            }
            const string ECGControlCommand::getLongName() const {
                return ECGControlCommand::LongName();
            }
            
            void ECGControlCommand::setActive(const bool &_active) {
                active = _active;
            }

            bool ECGControlCommand::getActive() const {
                return active;
            }

            void ECGControlCommand::setParams(const std::map<std::string,double> &_params) {
                params = _params;
            }
            
            std::map<std::string,double> ECGControlCommand::getParams() const {
                return params;
            }

            ostream& ECGControlCommand::operator<<(ostream &out) const {
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
            
            istream& ECGControlCommand::operator>>(istream &in) {
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
            
            const string ECGControlCommand::toString() const {
                stringstream sstr;

                sstr << "ECGControlCommand#" << endl;
                sstr << "Activate module: " << active << endl;
                for (auto& x: params){
                    sstr << x.first << ": " << x.second << endl;
                }

                return sstr.str();
            }
        }
    }
}