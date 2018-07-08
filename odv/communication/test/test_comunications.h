// TODO:
// Retirar prints

#include <cxxtest/TestSuite.h>
#include "../include/TCPReceive.hpp"
#include "../include/TCPSend.hpp"

using namespace std;

class test_comunications : public CxxTest::TestSuite
{
public:    
    void test_initialization_receiver(void) {
        cout << "\n\nTesting receiver constructor...\n";
        TCPReceiveBytes server(8080);
        TS_ASSERT_EQUALS(8080,server.get_port());        
        server.stop_connection();
    }

    void test_initialization_sender(void) {
        cout << "\n\nTesting sender constructor...\n";
        TCPSendBytes sender("localhost",8080);
        TS_ASSERT_EQUALS(8080,sender.get_port());        
        sender.disconnect();
    }

    void test_false_connection(void) {
        cout << "\n\nTesting sender connection false condition...\n";
        TCPSendBytes sender("localhost",8080);
        // Como ninguem está escutando deve retornar false
        TS_ASSERT_EQUALS(false,sender.connect());
        sender.disconnect();
    }

    void test_simple_send(void) {        
        cout << "\n\nTesting Simple message sending...\n";                        
        TCPSendBytes sender("localhost",1234);      
        TCPReceiveBytes server(1234);  
        
        server.start_connection();    
        
        sender.connect();
        
        sender.send("Test message");
        //Wait for package
        sleep(1);
        string pack = server.get_package();
        
        sender.disconnect();
        server.stop_connection();
        
        TS_ASSERT_EQUALS(pack,"Test message");
        
    }    
    void test_stress(void) {        
        cout << "\n\nTesting Stress conditions...\n";                        
        TCPSendBytes sender("localhost",8080);      
        TCPReceiveBytes server(8080);  
        
        server.start_connection();    
        
        sender.connect();

        for(int i=0; i < 500; i++)
            sender.send("Test message" + to_string(i+1));
                
        //Wait for package
        sleep(5);

        //server.print_buffer();

        string pack = server.get_package();        
        
        sender.disconnect();
        server.stop_connection();
        
        TS_ASSERT_EQUALS(pack,"Test message1");
        
    }    
};
