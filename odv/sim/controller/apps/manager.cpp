#include "ManagerModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ManagerModule cm(argc, argv);
    
    return cm.runModule();
}