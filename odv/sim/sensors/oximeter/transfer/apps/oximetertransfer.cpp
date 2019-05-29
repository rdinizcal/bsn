#include "OximeterTransferModule.hpp"

int32_t main(int32_t argc, char **argv) {
    OximeterTransferModule sensor(argc, argv);
    
    return sensor.runModule();
}