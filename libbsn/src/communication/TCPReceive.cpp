#include "bsn/communication/TCPReceive.hpp"

using namespace std;

using namespace odcore;
using namespace odcore::wrapper;
using namespace odcore::io;
using namespace odcore::io::tcp;

deque<string> buffer;

const vector<string> split(const string& s, const char& c) {    
	string buff{""};
	vector<string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

TCPReceive::TCPReceive(int p){    
    port = p;
}

void TCPReceive::set_port(int p){
    port = p;
}

int TCPReceive::get_port(){
    return port;
}

void TCPReceive::start_connection(){
    thread listener(&TCPReceive::initialize,this);
    listener.detach();
}

void TCPReceive::handleConnectionError() {

    this_connection->stop();
    cout << "Connection terminated\n";
}

string TCPReceive::get_package() {
    string p;
    buffer_lock.lock();
        if(buffer.size() > 0) {
            p = buffer.front();
            buffer.pop_front();
        }
        else {
            p = "";
        }
        
    buffer_lock.unlock();
    return p;
}

void TCPReceive::print_buffer() {
    buffer_lock.lock();
    cout << "\nBuffer " << buffer.size() << ':';
    for(auto s : buffer)
        cout << s << ',';
    buffer_lock.unlock();
}

void TCPReceive::nextString(const std::string &received_string) {
    // Realiza split pelo caracter separador escolhido. No caso '*'
    vector<string> vet = split(received_string,'*');
    buffer_lock.lock();
        for (auto str : vet) {
            buffer.push_back(str);
        }        
    buffer_lock.unlock();

}

void TCPReceive::onNewConnection(std::shared_ptr<odcore::io::tcp::TCPConnection> connection) {
    if (connection.get()) {
        cout << "Handle a new connection." << endl;
	
        connection->setRaw(true);	
        connection->setStringListener(this);
        connection->setConnectionListener(this);
        connection->start();        
        
        this_connection = connection;        
    }
}

void TCPReceive::stop_connection(){
    should_run = false;
}

void TCPReceive::initialize(){
    cout << "Server listening on port " << get_port() << endl;    
    should_run = true;
    try {
        std::shared_ptr<TCPAcceptor>
            tcpacceptor(TCPFactory::createTCPAcceptor(port));
        TCPReceive handler(-1);
        tcpacceptor->setAcceptorListener(&handler);
        tcpacceptor->start();
                
        while(should_run);        
        tcpacceptor.reset();
        cout << "Stopped the connection\n";
    }
    catch(string &exception) {
        cerr << "Error while creating TCP receiver: " << exception << endl;
    }
}