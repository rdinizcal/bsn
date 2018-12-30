#ifndef CONTEXT_INFO_H
#define CONTEXT_INFO_H

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

namespace bsn {
    namespace msg { 
        namespace info {
        
            class ContextInfo : public odcore::data::SerializableData {
            
                public:
                    ContextInfo(const std::string &/*task_id*/, const double &/*cost*/, const double &/*reliability*/);
    
                    ContextInfo();
                    virtual ~ContextInfo();
                    
                    ContextInfo(const ContextInfo &/*obj*/);
                    ContextInfo& operator=(const ContextInfo &/*obj*/);
                
                public:
                    virtual int32_t getID() const;
                    virtual const std::string getShortName() const;
                    virtual const std::string getLongName() const;

                    static int32_t ID();
                    static const std::string ShortName();
                    static const std::string LongName();
            
                public:
                    virtual std::ostream& operator<<(std::ostream &out) const;
                    virtual std::istream& operator>>(std::istream &in);
            
                    virtual const std::string toString() const;
                    
                public:
                    void setTask(const std::string &/*task_id*/);
                    std::string getTask() const;

                    void setCost(const double &/*cost*/);
                    double getCost() const;

                    void setReliability(const double &/*reliability*/);
                    double getReliability() const;
                
                private:
                    std::string task_id;
                    double      cost;
                    double      reliability;
            };
        }
    }
}

#endif