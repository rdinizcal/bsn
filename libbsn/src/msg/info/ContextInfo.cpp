#include "msg/info/ContextInfo.hpp"

namespace bsn {
    namespace msg { 
        namespace info {
        
            using namespace std;

            ContextInfo::ContextInfo() : 
                task_id(),
                cost(),
                reliability() {}
            
            ContextInfo::ContextInfo(const std::string &_task_id, const double &_cost, const double &_reliability) : 
                task_id(_task_id),
                cost(_cost),
                reliability(_reliability) {}
            
            ContextInfo::~ContextInfo() {}
            
            ContextInfo::ContextInfo(const ContextInfo &obj) :
                SerializableData(),
                task_id(obj.getTask()),
                cost(obj.getCost()),
                reliability(obj.getReliability()){}
            
            ContextInfo& ContextInfo::operator=(const ContextInfo &obj) {
                task_id = obj.getTask();
                cost = obj.getCost();
                reliability = obj.getReliability();
                return (*this);
            }
            
            int32_t ContextInfo::ID() {
                return 700;
            }
            const string ContextInfo::ShortName() {
                return "ContextInfo";
            }
            const string ContextInfo::LongName() {
                return "info.ContextInfo";
            }
            
            int32_t ContextInfo::getID() const {
                return ContextInfo::ID();
            }
            const string ContextInfo::getShortName() const {
                return ContextInfo::ShortName();
            }
            const string ContextInfo::getLongName() const {
                return ContextInfo::LongName();
            }

            void ContextInfo::setTask(const std::string &_task_id) {
                task_id = _task_id;
            }

            std::string ContextInfo::getTask() const{
                return task_id;
            }

            void ContextInfo::setCost(const double &_cost) {
                cost = _cost;
            }

            double ContextInfo::getCost() const {
                return cost;
            }

            void ContextInfo::setReliability(const double &_reliability){
                reliability = _reliability;
            }

            double ContextInfo::getReliability() const {
                return reliability;
            }

            ostream& ContextInfo::operator<<(ostream &out) const {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Serializer> s = sf.getQueryableNetstringsSerializer(out);
                
                s->write(1, task_id);
                s->write(2, cost);
                s->write(3, reliability);

                return out;
            }
            
            istream& ContextInfo::operator>>(istream &in) {
                odcore::serialization::SerializationFactory& sf=odcore::serialization::SerializationFactory::getInstance();
                std::shared_ptr<odcore::serialization::Deserializer> d = sf.getQueryableNetstringsDeserializer(in);
                
                d->read(1, task_id);
                d->read(2, cost);
                d->read(3, reliability);

                return in;
            }
            
            const string ContextInfo::toString() const {
                stringstream sstr;

                sstr << "ContextInfo#" << endl;
                sstr << "Task: " << task_id << endl; 
                sstr << "Cost: " << cost << endl;
                sstr << "Reliability: " << reliability << "%" << endl;

                return sstr.str();
            }
        }
    }
}