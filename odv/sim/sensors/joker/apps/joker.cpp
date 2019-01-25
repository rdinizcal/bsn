#include "JokerModule.hpp"

int32_t main(int32_t argc, char **argv) {
    JokerModule sensor(argc, argv);
    
    return sensor.runModule();
}