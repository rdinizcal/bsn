#include "msg/control/FilterModuleControlCommand.hpp"

namespace bsn {
    namespace msg { 
        namespace control {
        
            using namespace std;

            FilterModuleControlCommand::FilterModuleControlCommand() : active(), params() {}
            
            FilterModuleControlCommand::FilterModuleControlCommand(const bool & _active, const std::map<std::string,double> &_params) : active (_active), params(_params){
                /*for (auto& x: _params){
                    params.at(x.first) = x.second;
                }*/
            }
            
            FilterModuleControlCommand::~FilterModuleControlCommand() {}
            
            FilterModuleControlCommand::FilterModuleControlCommand(const FilterModuleControlCommand &obj) :
                SerializableData(),
                active(obj.getActive()),
                params(obj.getParams()){}
            
            FilterModuleControlCommand& FilterModuleControlCommand::operator=(const FilterModuleControlCommand &obj) {
                active = obj.getActive();   
                params = obj.getParams();     
                return (*this);
            }
            
            int32_t FilterModuleControlCommand::ID() {
                return 901;
            }
            const string FilterModuleControlCommand::ShortName() {
                return "FilterModuleControlCommand";
            }
            const string FilterModuleControlCommand::LongName() {
                return "data.FilterModuleControlCommand";
            }
            
            int32_t FilterModuleControlCommand::getID() const {
                return FilterModuleControlCommand::ID();
            }
            const string FilterModuleControlCommand::getShortName() const {
                return FilterModuleControlCommand::ShortName();
            }
            const string FilterModuleControlCommand::getLongName() const {
                return FilterModuleControlCommand::LongName();
            }
            
            void FilterModuleControlCommand::setActive(const bool &_active) {
                active = _active;
            }

            bool FilterModuleControlCommand::getActive() const {
                return active;
            }

            void FilterModuleControlCommand::setParams(const std::map<std::string,double> &_params) {
                params = _params;
            }
            
            std::map<std::string,double> FilterModuleControlCommand::getParams() const {
                return params;
            }

            ostream& FilterModuleControlCommand::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, active);
                /*for (auto& x: params){
                    s->write << x.first << ": " << x.second << "\n";
                }*/

                return out;
            }
            
            istream& FilterModuleControlCommand::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);

                d->read(1, active);

                return in;
            }
            
            const string FilterModuleControlCommand::toString() const {
                stringstream sstr;

                sstr << "FilterModuleControlCommand#" << endl;
                sstr << "Activate module: " << active << endl;
                for (auto& x: params){
                    sstr << x.first << ": " << x.second << endl;
                }

                return sstr.str();
            }
        }
    }
}