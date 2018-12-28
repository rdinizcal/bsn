#include "FilterModule.hpp"

int32_t main(int32_t argc, char **argv) {
    FilterModule fm(argc, argv);
    
    return fm.runModule();
}