#include "SenderModule.hpp"

int32_t main(int32_t argc, char **argv) {
    SenderModule sm(argc, argv);
    
    return sm.runModule();
}