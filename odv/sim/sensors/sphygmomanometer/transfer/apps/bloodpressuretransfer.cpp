#include "BloodpressureTransferModule.hpp"

int32_t main(int32_t argc, char **argv) {
    BloodpressureTransferModule sensor(argc, argv);
    
    return sensor.runModule();
}