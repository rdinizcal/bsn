#include <stdint.h>

#include "../include/ConverterModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ConverterModule dcm(argc, argv);
    
    return dcm.runModule();
}