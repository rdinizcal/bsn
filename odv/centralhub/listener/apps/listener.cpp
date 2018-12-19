#include "ListenerModule.hpp"

int32_t main(int32_t argc, char **argv) {
    ListenerModule lm(argc, argv);

    return lm.runModule();
}