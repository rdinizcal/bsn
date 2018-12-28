#include "ActuatorModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ActuatorModule actuator(argc, argv);
    
    return actuator.runModule();
}