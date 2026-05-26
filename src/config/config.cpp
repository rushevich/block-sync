#include "config.h"

#include <fstream>
#include <iostream>
#include <optional>
#include <toml++/toml.h>

#include "prism/prism.h"

namespace fs = std::filesystem;

namespace blocksync::config {

namespace {

void create_config_directory(const fs::path& bs_config_path) {
  if (fs::create_directories(bs_config_path)) {
    std::cout << "Config dir created \n";
  } else {
    std::cout << "Creation failed or dir already exists\n";
  }
}

}  // namespace

bool config_found(AppConfig& cfg) {
  if (cfg.os == "LINUX")
    cfg.bs_config_path =
        cfg.user_data_root / ".." / ".." / ".config" / "block-sync";
  else
    cfg.bs_config_path = cfg.user_data_root / "block-sync";
  return fs::exists(cfg.bs_config_path);
}

fs::path initial_setup(const AppConfig& cfg) {
  create_config_directory(cfg.bs_config_path);

  fs::path prism_root{cfg.user_data_root / "PrismLauncher"};

  if (!fs::exists(prism_root)) {
    return {};
  }

  auto instance_dir{prism::resolve_instance_path(prism_root)};
  if (instance_dir.empty() || !fs::is_directory(instance_dir)) {
    return {};
  }

  std::ofstream bs_config(cfg.bs_config_path / "config.toml");
  if (!bs_config) {
    return {};
  }

  auto info_tbl = toml::table{{"prism", prism_root.string()},
                              {"instances", instance_dir.string()}};

  bs_config << info_tbl;
  return instance_dir;
}

fs::path retrieve_instance_dir(const AppConfig& cfg) {
  toml::table info_tbl =
      toml::parse_file((cfg.bs_config_path / "config.toml").string());

  std::optional<std::string> instances =
      info_tbl["instances"].value<std::string>();

  return {*instances};
}

}  // namespace blocksync::config
