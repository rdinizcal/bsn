#include "ThermCollectModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ThermCollectModule sensor(argc, argv);
    
    return sensor.runModule();
}