#include "msg/control/SenderModuleControlCommand.hpp"

namespace bsn {
    namespace msg { 
        namespace control {
        
            using namespace std;

            SenderModuleControlCommand::SenderModuleControlCommand() : active(), params() {}
            
            SenderModuleControlCommand::SenderModuleControlCommand(const bool & _active, const std::map<std::string,double> &_params) : active (_active), params(_params){
                /*for (auto& x: _params){
                    params.at(x.first) = x.second;
                }*/
            }
            
            SenderModuleControlCommand::~SenderModuleControlCommand() {}
            
            SenderModuleControlCommand::SenderModuleControlCommand(const SenderModuleControlCommand &obj) :
                SerializableData(),
                active(obj.getActive()),
                params(obj.getParams()){}
            
            SenderModuleControlCommand& SenderModuleControlCommand::operator=(const SenderModuleControlCommand &obj) {
                active = obj.getActive();   
                params = obj.getParams();     
                return (*this);
            }
            
            int32_t SenderModuleControlCommand::ID() {
                return 902;
            }
            const string SenderModuleControlCommand::ShortName() {
                return "SenderModuleControlCommand";
            }
            const string SenderModuleControlCommand::LongName() {
                return "control.SenderModuleControlCommand";
            }
            
            int32_t SenderModuleControlCommand::getID() const {
                return SenderModuleControlCommand::ID();
            }
            const string SenderModuleControlCommand::getShortName() const {
                return SenderModuleControlCommand::ShortName();
            }
            const string SenderModuleControlCommand::getLongName() const {
                return SenderModuleControlCommand::LongName();
            }
            
            void SenderModuleControlCommand::setActive(const bool &_active) {
                active = _active;
            }

            bool SenderModuleControlCommand::getActive() const {
                return active;
            }

            void SenderModuleControlCommand::setParams(const std::map<std::string,double> &_params) {
                params = _params;
            }
            
            std::map<std::string,double> SenderModuleControlCommand::getParams() const {
                return params;
            }

            ostream& SenderModuleControlCommand::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, active);
                /*for (auto& x: params){
                    s->write << x.first << ": " << x.second << "\n";
                }*/

                return out;
            }
            
            istream& SenderModuleControlCommand::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);

                d->read(1, active);

                return in;
            }
            
            const string SenderModuleControlCommand::toString() const {
                stringstream sstr;

                sstr << "SenderModuleControlCommand#" << endl;
                sstr << "Activate module: " << active << endl;
                for (auto& x: params){
                    sstr << x.first << ": " << x.second << endl;
                }

                return sstr.str();
            }
        }
    }
}