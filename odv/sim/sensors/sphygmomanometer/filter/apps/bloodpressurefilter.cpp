#include "BloodpressureFilterModule.hpp"

int32_t main(int32_t argc, char **argv) {
    BloodpressureFilterModule sensor(argc, argv);
    
    return sensor.runModule();
}