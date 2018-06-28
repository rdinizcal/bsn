#include "../include/consumer.hpp"

using namespace std;
using namespace odcore;
using namespace odcore::wrapper;

string get_package() {
    const string NAME = "Area";
    try {
        std::shared_ptr<SharedMemory> sharedMemory(SharedMemoryFactory::attachToSharedMemory(NAME));

        if (sharedMemory->isValid()) {
            string s;

            sharedMemory->lock();
            char *p = static_cast<char*>(sharedMemory->getSharedMemory());
            s = string(p);
            // Empty buffer
            strcpy(p,"");
            sharedMemory->unlock();                                    
        
            return s;            
        }
    }
    catch(string &exception) {
        cerr << "Shared memory could not be created: " << exception << endl;
        return "error";
    }    
}
