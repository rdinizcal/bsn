#include "msg/control/ControlSignal.hpp"

namespace bsn {
    namespace msg { 
        namespace control {
        
            using namespace std;

            ControlSignal::ControlSignal() : recipient(), active(), params() {}
            
            ControlSignal::ControlSignal(const int32_t &_recipient, const bool &_active, const std::map<std::string,double> &_params) : recipient(_recipient), active (_active), params(_params) {}
            
            ControlSignal::~ControlSignal() {}
            
            ControlSignal::ControlSignal(const ControlSignal &obj) :
                SerializableData(),
                recipient(obj.getRecipientID()),
                active(obj.getActive()),
                params(obj.getParams()){}
            
            ControlSignal& ControlSignal::operator=(const ControlSignal &obj) {
                recipient = obj.getRecipientID();
                active = obj.getActive();   
                params = obj.getParams();     
                return (*this);
            }
            
            int32_t ControlSignal::ID() {
                return 915;
            }
            const string ControlSignal::ShortName() {
                return "ControlSignal";
            }
            const string ControlSignal::LongName() {
                return "control.ControlSignal";
            }
            
            int32_t ControlSignal::getID() const {
                return ControlSignal::ID();
            }
            const string ControlSignal::getShortName() const {
                return ControlSignal::ShortName();
            }
            const string ControlSignal::getLongName() const {
                return ControlSignal::LongName();
            }
            
            void ControlSignal::setRecipientID(const int32_t &_recipient) {
                recipient = _recipient;
            }

            int32_t ControlSignal::getRecipientID() const {
                return recipient;
            }

            void ControlSignal::setActive(const bool &_active) {
                active = _active;
            }

            bool ControlSignal::getActive() const {
                return active;
            }

            void ControlSignal::setParams(const std::map<std::string,double> &_params) {
                params = _params;
            }
            
            std::map<std::string,double> ControlSignal::getParams() const {
                return params;
            }

            ostream& ControlSignal::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1,recipient);
                s->write(2, active);
                s->write(3, params.size());
                int i=3;
                for (auto& x: params){
                    s->write(++i,x.first);
                    s->write(++i,x.second);
                }

                return out;
            }
            
            istream& ControlSignal::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                int32_t size; 

                d->read(1,recipient);
                d->read(2, active);
                d->read(3, size);

                for (int32_t i = 4; i < size; ++i) {
                    std::string key;
                    double value;

                    d->read(i, key);
                    ++i;
                    d->read(i, value);

                    params.insert ( std::pair<std::string,double>(key,value) );
                }

                return in;
            }
            
            const string ControlSignal::toString() const {
                stringstream sstr;

                sstr << "ControlSignal#" << endl;
                sstr << "Activate module: " << active << endl;
                for (auto& x: params){
                    sstr << x.first << ": " << x.second << endl;
                }

                return sstr.str();
            }
        }
    }
}