#include <thread>
#include <deque>
#include "../include/TCPReceiveBytes.hpp"
#include "../include/TCPSendBytes.hpp"

using namespace std;

using namespace odcore;
using namespace odcore::wrapper;

int main() {
    TCPSendBytes sender("localhost",1234);      
    TCPReceiveBytes server(1234);  
    
    server.start_connection();    
    
    sender.connect();
    
    sender.send("Test message1*Test message2");
    //Wait for package        
    cout << "Wait...\n";
    sleep(2);    

    cout << "Pacote recebido: " << server.get_package() <<  ',' << server.get_package() << endl;
    
    sender.disconnect();
    server.stop_connection();

    return 0;
}
