#ifndef ECG_TRANSFER_TASK_MSG
#define ECG_TRANSFER_TASK_MSG

#include "opendavinci/odcore/data/SerializableData.h"
#include "opendavinci/odcore/serialization/Deserializer.h"
#include "opendavinci/odcore/serialization/SerializationFactory.h"
#include "opendavinci/odcore/serialization/Serializer.h"

#include <stdint.h>

namespace bsn {
    namespace msg { 
        namespace taskMsg {
        
            class  ECGTransferTaskMessage : public odcore::data::SerializableData {
            
                public:
                    ECGTransferTaskMessage(const double &/*data*/);
    
                    ECGTransferTaskMessage();
                    virtual ~  ECGTransferTaskMessage();
                    
                    ECGTransferTaskMessage(const ECGTransferTaskMessage &/*obj*/);
                    ECGTransferTaskMessage& operator=(const  ECGTransferTaskMessage &/*obj*/);
                
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
                    void setData(const double &/*data*/);
                    double getData() const;

                private:
                    double data;
            };
        }
    }
}

#endif