#include "TCPSendBytes.hpp"

using namespace std;
using namespace odcore;
using namespace odcore::io::tcp;

void send(){
    const string RECEIVER = "127.0.0.1";
    const uint32_t PORT = 1234;

    try {
        std::shared_ptr<TCPConnection> connection(TCPFactory::createTCPConnectionTo(RECEIVER, PORT));
        connection->setRaw(true);
        for(int i=0;i<2;i++){
            string str = "Hello world " + to_string(i+1) + ' ';
            connection->send(str);
        }        
        
    }
    catch(string &exception) {
        cerr << "Data could not be sent: " << exception << endl;
    }
}

