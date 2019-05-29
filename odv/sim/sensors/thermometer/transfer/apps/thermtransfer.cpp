#include "ThermTransferModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ThermTransferModule sensor(argc, argv);
    
    return sensor.runModule();
}