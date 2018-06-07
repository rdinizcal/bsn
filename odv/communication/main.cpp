#include <thread>
#include "TCPReceiveBytes.hpp"
#include "consumer.hpp"

using namespace std;

using namespace odcore;
using namespace odcore::wrapper;

void create_thread(TCPReceiveBytes * s) {
    s->initialize();
}

int main() {
    TCPReceiveBytes server;
    
    std::thread listener(create_thread,&server);
    sleep(5);
    string pack = get_package();    
    cout << "GOT PACKAGE: " <<  pack << endl;
    
    server.stop_connection();
    
    listener.join();
    
    return 0;
}
