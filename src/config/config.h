#ifndef BLOCKSYNC_CONFIG_CONFIG_H
#define BLOCKSYNC_CONFIG_CONFIG_H
#include <filesystem>
#include <string>

namespace blocksync::config {

// app-wide runtime state, populated at startup. replaces former file-scope
// globals so call order is explicit.
struct AppConfig {
  std::string os;
  std::filesystem::path user_data_root;
  std::filesystem::path bs_config_path;
};

// resolves cfg.bs_config_path from cfg.os + cfg.user_data_root, returns true if
// that path already exists. requires os + user_data_root set first.
bool config_found(AppConfig& cfg);

// creates config dir + config.toml, returns the resolved instances folder.
// called when no config exists yet. empty path on failure.
std::filesystem::path initial_setup(const AppConfig& cfg);

// reads the instances folder from an existing config.toml
std::filesystem::path retrieve_instance_dir(const AppConfig& cfg);

}  // namespace blocksync::config
#endif
