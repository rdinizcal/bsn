#include "OximeterCollectModule.hpp"

int32_t main(int32_t argc, char **argv) {
    OximeterCollectModule sensor(argc, argv);
    
    return sensor.runModule();
}