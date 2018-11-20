#include "../include/TxtPersister.hpp"

int32_t main(int32_t argc, char **argv) {
    TxtPersister tp(argc, argv);

    return tp.runModule();
}