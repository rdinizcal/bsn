#include "../include/Listener.hpp"

int32_t main(int32_t argc, char **argv) {
    TimeTriggeredSender tts(argc, argv);

    return tts.runModule();
}