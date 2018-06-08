#include <thread>
#include "TCPReceiveBytes.hpp"
#include "TCPSendBytes.hpp"
#include "consumer.hpp"

using namespace std;

using namespace odcore;
using namespace odcore::wrapper;

int main() {
    TCPSendBytes sender("localhost",8080);
    sender.connect();
    sender.send("Teste1\n");
    sender.disconnect();
    sleep(5);
    TCPSendBytes sender2("localhost",8080);
    sender2.connect();
    sender2.send("Teste2\n");
    sender2.disconnect();
    
    return 0;
}
