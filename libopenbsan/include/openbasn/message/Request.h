#ifndef MESSAGE_REQUEST_H_
#define MESSAGE_REQUEST_H_

#include "opendavinci/odcore/data/SerializableData.h"

namespace openbasn {
    namespace message {

        using namespace std;
        using namespace odcore::data;
        
        class Request : public SerializableData {

            public:
                Request(const int32_t &/*request_type*/);

                Request();
                virtual ~Request();
        
                Request(const Request &/*obj*/);
                Request& operator=(const Request &/*obj*/);

            public:
                enum REQUESTTYPE {
                    UNDEFINED           = 0,
                    SENSOR_DATA         = 1
                };

            public:
                virtual int32_t getID() const;
                virtual const string getShortName() const;
                virtual const string getLongName() const;

                static int32_t ID();
                static const string ShortName();
                static const string LongName();
                        
            public:
                virtual ostream& operator<<(ostream &out) const;
                virtual istream& operator>>(istream &in);
        
                virtual const string toString() const;
            
            public:
                int32_t getRequestType() const;

            private:
                int32_t m_request_type;
        };
    }
}

#endif