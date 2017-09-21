#include <stdint.h>

#include "SensorNodeModule.hpp"

int32_t main(int32_t argc, char **argv) {
    SensorNodeModule snmodule(argc, argv);
    
    return snmodule.runModule();
}