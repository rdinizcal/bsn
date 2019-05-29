#include "BloodpressureCollectModule.hpp"

int32_t main(int32_t argc, char **argv) {
    BloodpressureCollectModule sensor(argc, argv);
    
    return sensor.runModule();
}