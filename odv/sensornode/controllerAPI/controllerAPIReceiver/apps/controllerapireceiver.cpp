#include "ControllerAPIReceiverModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ControllerAPIReceiverModule capir(argc, argv);
    
    return capir.runModule();
}