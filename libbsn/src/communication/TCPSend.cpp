#include "bsn/communication/TCPSend.hpp"

using namespace std;
using namespace odcore;
using namespace odcore::io::tcp;

TCPSend::TCPSend(int32_t this_port) {
    port = this_port;
    is_connected = false;
}

TCPSend::TCPSend(std::string this_ip, int32_t this_port) {
    port = this_port;
    ip = this_ip;
    is_connected = false;
}

bool TCPSend::connect(){
    try{                
        std::shared_ptr<TCPConnection> connection(TCPFactory::createTCPConnectionTo(ip, port));                
        connection->setRaw(true);        
        thisConnection = connection;                                
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

void TCPSend::setIP(string vIP){
    this->ip = vIP;
}

void TCPSend::set_port(int32_t p){
    port = p;
}

int32_t TCPSend::get_port() {
    return port;
}

void TCPSend::disconnect() {
    // Desaloca o ponteiro pra conexão
    thisConnection.reset();
}

void TCPSend::send(string package){    
    if(is_connected){
        try {
            // Adiciona caracter separador
            package += '*';
            thisConnection->send(package);        
        }
        catch(string &exception) {
            cerr << "Data could not be sent: " << exception << endl;
        }
    }
}

