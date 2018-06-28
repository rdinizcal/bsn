#include "../include/TCPSendBytes.hpp"

using namespace std;
using namespace odcore;
using namespace odcore::io::tcp;

TCPSendBytes::TCPSendBytes(std::string this_ip, int this_port) {
    port = this_port;
    ip = this_ip;
    is_connected = false;
}

bool TCPSendBytes::connect(){
    try{                
        std::shared_ptr<TCPConnection> connection(TCPFactory::createTCPConnectionTo(ip, port));                
        connection->setRaw(true);        
        this_connection = connection;                                
        cout << "Sender connected on " << get_port() << endl;
        is_connected = true;
        return true;
    }
    catch(string &exception) {
        cout << "Sender couldn't connect on " << get_port() << '(' << exception << ')' << endl;
        is_connected = false;
        return false;
    }   
}
int TCPSendBytes::get_port() {
    return port;
}

void TCPSendBytes::disconnect() {
    // Desaloca o ponteiro pra conexão
    this_connection.reset();
}

void TCPSendBytes::send(string package){    
    if(is_connected){
        try {
            this_connection->send(package);        
        }
        catch(string &exception) {
            cerr << "Data could not be sent: " << exception << endl;
        }
    }
}

