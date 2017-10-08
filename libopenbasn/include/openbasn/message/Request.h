#ifndef MESSAGE_REQUEST_H_
#define MESSAGE_REQUEST_H_

#include "opendavinci/odcore/data/SerializableData.h"

namespace openbasn {
    namespace message {

        using namespace std;
        using namespace odcore::data;
        
        class Request : public SerializableData {

            public:
                Request(const int32_t &/*type*/, const uint32_t &/*source_id*/);
                Request(const int32_t &/*type*/, const uint32_t &/*source_id*/, const uint32_t &/*destination_id*/);
                Request(const int32_t &/*type*/, const uint32_t &/*source_id*/, const string &/*content*/);
                Request(const int32_t &/*type*/, const uint32_t &/*source_id*/, const uint32_t &/*destination_id*/, const string &/*content*/);

                Request();
                virtual ~Request();
        
                Request(const Request &/*obj*/);
                Request& operator=(const Request &/*obj*/);

            public:
                enum REQUESTTYPE {
                    UNDEFINED           = 0,
                    SENSOR_DATA         = 1,
                    REGISTER            = 2,
                    UNREGISTER          = 3
                };

            public:
                static int32_t ID();
                virtual int32_t getID() const;
                virtual const string getShortName() const;
                virtual const string getLongName() const;
                virtual const string toString() const;
                        
            public:
                virtual ostream& operator<<(ostream &out) const;
                virtual istream& operator>>(istream &in);
            
            public:
                int32_t getType() const;

                void setSourceID(const uint32_t &/*m_source_id*/);
                uint32_t getSourceID() const;

                void setDestinationID(const uint32_t &/*m_destination_id*/);
                uint32_t getDestinationID() const;

                void setContent(const uint32_t &/*m_content*/);
                string getContent() const;

            private:
                int32_t m_type;
                uint32_t m_source_id;
                uint32_t m_destination_id;

                string m_content;
        };
    }
}

#endif