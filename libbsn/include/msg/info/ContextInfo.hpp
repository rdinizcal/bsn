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
                    ContextInfo(const std::string &/*context_id*/, const bool &/*value*/);
    
                    ContextInfo();
                    virtual ~ContextInfo();
                    
                    ContextInfo(const ContextInfo &);
                    ContextInfo& operator=(const ContextInfo &);
                
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
                    void setContext(const std::string &/*context_id*/);
                    std::string getContext() const;

                    void setValue(const bool &/*value*/);
                    bool getValue() const;

                
                private:
                    std::string context_id;
                    bool value;
            };
        }
    }
}

#endif