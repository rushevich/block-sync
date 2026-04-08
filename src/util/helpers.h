#ifndef BLOCKSYNC_HELPERS_H
#define BLOCKSYNC_HELPERS_H
#include <string>

extern std::string user_os;

// function to detect OS and set a global variable 
void detect_os();

// checks for block-sync config file, returns true if found
bool config_found();

#endif