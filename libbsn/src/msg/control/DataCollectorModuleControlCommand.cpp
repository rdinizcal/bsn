#include "msg/control/DataCollectorModuleControlCommand.hpp"

namespace bsn {
    namespace msg { 
        namespace control {
        
            using namespace std;

            DataCollectorModuleControlCommand::DataCollectorModuleControlCommand() : active(), params() {}
            
            DataCollectorModuleControlCommand::DataCollectorModuleControlCommand(const bool & _active, const std::map<std::string,double> &_params) : active (_active), params(_params){
                /*for (auto& x: _params){
                    params.at(x.first) = x.second;
                }*/
            }
            
            DataCollectorModuleControlCommand::~DataCollectorModuleControlCommand() {}
            
            DataCollectorModuleControlCommand::DataCollectorModuleControlCommand(const DataCollectorModuleControlCommand &obj) :
                SerializableData(),
                active(obj.getActive()),
                params(obj.getParams()){}
            
            DataCollectorModuleControlCommand& DataCollectorModuleControlCommand::operator=(const DataCollectorModuleControlCommand &obj) {
                active = obj.getActive();   
                params = obj.getParams();     
                return (*this);
            }
            
            int32_t DataCollectorModuleControlCommand::ID() {
                return 900;
            }
            const string DataCollectorModuleControlCommand::ShortName() {
                return "DataCollectorModuleControlCommand";
            }
            const string DataCollectorModuleControlCommand::LongName() {
                return "data.DataCollectorModuleControlCommand";
            }
            
            int32_t DataCollectorModuleControlCommand::getID() const {
                return DataCollectorModuleControlCommand::ID();
            }
            const string DataCollectorModuleControlCommand::getShortName() const {
                return DataCollectorModuleControlCommand::ShortName();
            }
            const string DataCollectorModuleControlCommand::getLongName() const {
                return DataCollectorModuleControlCommand::LongName();
            }
            
            void DataCollectorModuleControlCommand::setActive(const bool &_active) {
                active = _active;
            }

            bool DataCollectorModuleControlCommand::getActive() const {
                return active;
            }

            void DataCollectorModuleControlCommand::setParams(const std::map<std::string,double> &_params) {
                params = _params;
            }
            
            std::map<std::string,double> DataCollectorModuleControlCommand::getParams() const {
                return params;
            }

            ostream& DataCollectorModuleControlCommand::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, active);
                /*for (auto& x: params){
                    s->write << x.first << ": " << x.second << "\n";
                }*/

                return out;
            }
            
            istream& DataCollectorModuleControlCommand::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);

                d->read(1, active);

                return in;
            }
            
            const string DataCollectorModuleControlCommand::toString() const {
                stringstream sstr;

                sstr << "DataCollectorModuleControlCommand#" << endl;
                sstr << "Activate module: " << active << endl;
                for (auto& x: params){
                    sstr << x.first << ": " << x.second << endl;
                }

                return sstr.str();
            }
        }
    }
}