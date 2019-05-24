#include "ECGFilterModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ECGFilterModule sensor(argc, argv);
    
    return sensor.runModule();
}