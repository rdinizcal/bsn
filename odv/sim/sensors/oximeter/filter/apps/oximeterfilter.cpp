#include "OximeterFilterModule.hpp"

int32_t main(int32_t argc, char **argv) {
    OximeterFilterModule sensor(argc, argv);
    
    return sensor.runModule();
}