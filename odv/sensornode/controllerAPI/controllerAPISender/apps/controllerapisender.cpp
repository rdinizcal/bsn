#include "ControllerAPISenderModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ControllerAPISenderModule capis(argc, argv);
    
    return capis.runModule();
}