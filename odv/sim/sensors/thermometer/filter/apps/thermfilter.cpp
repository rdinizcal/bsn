#include "ThermFilterModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ThermFilterModule sensor(argc, argv);
    
    return sensor.runModule();
}