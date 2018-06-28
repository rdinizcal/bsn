// TODO:
// Retirar prints

#include <cxxtest/TestSuite.h>
#include "../include/TCPReceiveBytes.hpp"
#include "../include/TCPSendBytes.hpp"

using namespace std;

class test_comunications : public CxxTest::TestSuite
{
public:    
    void test_initialization_receiver(void) {
        cout << "\nTesting receiver constructor...\n";
        TCPReceiveBytes server(8080);
        TS_ASSERT_EQUALS(8080,server.get_port());        
        server.stop_connection();
    }

    void test_initialization_sender(void) {
        cout << "\nTesting sender constructor...\n";
        TCPSendBytes sender("localhost",8080);
        TS_ASSERT_EQUALS(8080,sender.get_port());        
        sender.disconnect();
    }

    void test_false_connection(void) {
        cout << "\nTesting sender connection false condition...\n";
        TCPSendBytes sender("localhost",8080);
        // Como ninguem está escutando deve retornar false
        TS_ASSERT_EQUALS(false,sender.connect());
        sender.disconnect();
    }

    void test_simple_send(void) {        
        cout << "\nTesting Simple message sending...\n";                        
        TCPSendBytes sender2("localhost",1234);      
        TCPReceiveBytes server(1234);  
        
        server.start_connection();    
        
        sender2.connect();
        
        sender2.send("Test message");
        //Wait for package
        sleep(1);
        string pack = server.get_package();
        
        sender2.disconnect();
        server.stop_connection();
        
        TS_ASSERT_EQUALS(pack,"Test message");
        
    }    
};
