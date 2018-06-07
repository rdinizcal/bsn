#include <stdint.h>
#include <iostream>
#include <string>
#include <memory>
#include <deque>
#include <vector>
#include <mutex>
#include <atomic>
#include <string.h>
#include <opendavinci/odcore/base/Thread.h>
#include <opendavinci/odcore/io/tcp/TCPAcceptor.h>
#include <opendavinci/odcore/io/tcp/TCPFactory.h>
#include <opendavinci/odcore/io/ConnectionListener.h>
#include <opendavinci/odcore/io/StringListener.h>
#include <opendavinci/odcore/io/tcp/TCPAcceptorListener.h>
#include <opendavinci/odcore/io/tcp/TCPConnection.h>
#include <opendavinci/odcore/base/Lock.h>
#include <opendavinci/odcore/wrapper/SharedMemory.h>
#include <opendavinci/odcore/wrapper/SharedMemoryFactory.h>

using namespace std;

using namespace odcore;
using namespace odcore::wrapper;
using namespace odcore::io;
using namespace odcore::io::tcp;

void produce(){
    shared_ptr<SharedMemory> sharedMemory(SharedMemoryFactory::createSharedMemory("teste",1));
    char *p = static_cast<char*>(sharedMemory->getSharedMemory());    
    // Concatena ao buffer
    string b = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    strcat(p,b.c_str());
    cout << "Shared mem: " << p << ' ' << sharedMemory->getSize() << endl;
    SharedMemoryFactory::createSharedMemory("teste",20);
    p = static_cast<char*>(sharedMemory->getSharedMemory());   
    b = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    strcat(p,b.c_str());
    cout << "Shared mem: " << p << ' ' << sharedMemory->getSize() << endl;
}
void consume(){
    shared_ptr<SharedMemory> sharedMemory(SharedMemoryFactory::attachToSharedMemory("teste"));

    if (sharedMemory->isValid()) {
        char *p = static_cast<char*>(sharedMemory->getSharedMemory());
        string s = string(p);
        // Empty buffer
        cout << "Buff: " << s << endl;
                                    
    }
    else{
        cout << "Not valid!\n";
    }
}

int main(){
    produce();
    //consume();
    
}