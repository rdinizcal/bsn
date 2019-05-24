#include "ECGCollectModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ECGCollectModule sensor(argc, argv);
    
    return sensor.runModule();
}