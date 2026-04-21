#ifndef BLOCKSYNC_UTIL_HELPERS_H
#define BLOCKSYNC_UTIL_HELPERS_H
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// function to detect OS and set a global variable
std::string detect_os();

// checks for block-sync config file, returns true if found
bool config_found();

std::string get_iso8601_time();

void create_config_directory();

fs::path env_path(const char* name);

fs::path get_user_data_root();

fs::path get_bs_config_path();

fs::path resolve_prism_instance_path(const fs::path& prism_root);

void get_instance_selection(int& selected_instance,
                            const size_t& highest_instance_index);

fs::path initial_setup(
    const fs::path&
        user_data_root);  // this gets called if no block-sync config is found
                          // returns the path to instances folder

fs::path retrieve_instance_dir();
#endif