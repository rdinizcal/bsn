#include "ECGTransferModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ECGTransferModule sensor(argc, argv);
    
    return sensor.runModule();
}