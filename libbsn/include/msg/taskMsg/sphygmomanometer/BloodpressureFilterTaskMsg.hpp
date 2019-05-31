#ifndef BLOODPRESSURE_FILTER_TASK_MSG
#define BLOODPRESSURE_FILTER_TASK_MSG

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

#include <stdint.h>

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            class  BloodpressureFilterTaskMessage : public odcore::data::SerializableData {
            
                public:
                    BloodpressureFilterTaskMessage(const double &/*data*/, const double &/*data*/);
    
                    BloodpressureFilterTaskMessage();
                    virtual ~  BloodpressureFilterTaskMessage();
                    
                    BloodpressureFilterTaskMessage(const BloodpressureFilterTaskMessage &/*obj*/);
                    BloodpressureFilterTaskMessage& operator=(const  BloodpressureFilterTaskMessage &/*obj*/);
                
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
                    void setData(const double &/*data*/, const double &/*data*/);
                    double getDataS() const;
                    double getDataD() const;
                private:
                    double dataS;
                    double dataD;
            };
        }
    }
}

#endif