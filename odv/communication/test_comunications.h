// TODO:
// Retirar prints
// Receber parâmetros em construtores das classes
// Execução em thread sendo chamada pelo receiver
// Criação da classe Sender 

#include <cxxtest/TestSuite.h>
#include <thread>
#include "TCPReceiveBytes.hpp"
#include "consumer.hpp"
#include "TCPSendBytes.hpp"

using namespace std;

void create_thread(TCPReceiveBytes * s) {
    s->initialize();
}

class test_comunications : public CxxTest::TestSuite
{
public:
    void test_simple_send(void) {        
        TCPReceiveBytes server;
        std::thread listener(create_thread,&server);
        send();
        sleep(1);
        string pack = get_package();
        server.stop_connection();
        listener.join();        
        
        TS_ASSERT_EQUALS(pack,"Hello world 1 Hello world 2 ");
        
    }
};
