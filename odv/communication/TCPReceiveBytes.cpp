#include "TCPReceiveBytes.hpp"

using namespace std;

using namespace odcore;
using namespace odcore::wrapper;
using namespace odcore::io;
using namespace odcore::io::tcp;

const string NAME = "Area";
const uint32_t SIZE = 1000;
shared_ptr<SharedMemory> sharedMemory(SharedMemoryFactory::createSharedMemory(NAME, SIZE));

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

void TCPReceiveBytes::handleConnectionError() {

    this_connection->stop();
    cout << "Connection terminated\n";
}

void TCPReceiveBytes::nextString(const std::string &received_string) {
    //cout << "Received " << received_string.length() 
    //     << " bytes containing '" << received_string << "'" << endl;    

    sharedMemory->lock();
    char *p = static_cast<char*>(sharedMemory->getSharedMemory());
    // Concatena ao buffer
    strcat(p,received_string.c_str());
    sharedMemory->unlock();
}

void TCPReceiveBytes::onNewConnection(std::shared_ptr<odcore::io::tcp::TCPConnection> connection) {
    if (connection.get()) {
        cout << "Handle a new connection." << endl;
	
        connection->setRaw(true);	
        connection->setStringListener(this);
        connection->setConnectionListener(this);
        connection->start();        
        
        this_connection = connection;
        
    }
}

void TCPReceiveBytes::stop_connection(){
    should_run = false;
}

void TCPReceiveBytes::initialize(){
    cout << "initializing\n";
    const uint32_t PORT = 1234;    
    should_run = true;
    try {
        std::shared_ptr<TCPAcceptor>
            tcpacceptor(TCPFactory::createTCPAcceptor(PORT));
        TCPReceiveBytes handler;
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