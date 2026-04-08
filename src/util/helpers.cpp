#include "helpers.h"

std::string user_os;

void detect_os() {
    #if defined (_WIN32)
        user_os = "WINDOWS";
    #elif defined (__linux__)
        user_os = "LINUX";
    #elif defined (__APPLE__) 
        user_os = "MACOS";
    #endif
}

bool config_found() {
    return true;
}