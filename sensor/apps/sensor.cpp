#include <stdint.h>

#include "SensorModule.h"

int32_t main(int32_t argc, char **argv) {
    SensorModule sensor(argc, argv);
    
    return sensor.runModule();
}