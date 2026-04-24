#ifndef BLOCKSYNC_UTIL_HELPERS_H
#define BLOCKSYNC_UTIL_HELPERS_H
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

namespace fs = std::filesystem;
using json = nlohmann::json;

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

// note that dereferencing the iterator yields the value at the current key
template <typename UnaryFunction>
auto recursive_json_iterate_if(json& j, UnaryFunction f)
    -> void {  // an example of using trailing return type. but i think void
               // prefix
  for (auto it = j.begin(); it != j.end(); ++it) {
    if (it.key() == "children")
      recursive_json_iterate_if(
          *it,
          f);  // this needs to be changed since children
               // is an array. not sure what happens when the iterator is
               // dereferenced when pointing to the key coresponding to json
               // array
    else
      f(it);
  }
};

// cleans manifest
auto clean_manifest(json& manifest) -> void;
#endif