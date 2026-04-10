#include "util/helpers.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

// prevents global visibility
namespace {

// safe helper for getenv
fs::path env_path(const char *name) {
  const char *source{std::getenv(name)};
  if (!source || !*source) {
    return {};
  }
  return fs::path(source);
}

// retrieves path for large user data
fs::path get_user_data_root() {
  if (user_os == "WINDOWS") {
    return fs::path{env_path("APPDATA")};
  }
  if (user_os == "LINUX") {
    fs::path data{env_path("XDG_DATA_HOME")};
    if (!data.empty()) {
      return data;
    }
    fs::path home{env_path("HOME")};
    if (home.empty()) {
      return {};
    }
    return home / ".local" / "share";
  }
  if (user_os == "MACOS") {
    fs::path home{env_path("HOME")};
    if (home.empty()) {
      return {};
    }
    return home / "Library" / "Application Support";
  }
  return {};
}
} // namespace


int main(int, char **) {
  detect_os();
  std::cout << "(DEBUG) User OS: " << user_os << std::endl;

  fs::path user_data_root{get_user_data_root()};
  std::cout << "(DEBUG) User Data Root: " << user_data_root << std::endl;

  fs::path prism_root{user_data_root / "PrismLauncher"};
  if (!fs::exists(prism_root)) {
    std::cout << "(DEBUG) PrismLauncher directory not found.";
    return 0;
  }
  std::cout << "(DEBUG) PrismLauncher: " << prism_root << std::endl;

  std::ifstream prism_config{prism_root / "prismlauncher.cfg"};

  std::string line;
  while (std::getline(prism_config, line)) {
    if (line.find("InstanceDir") != std::string::npos)
      break;
  }

  fs::path instance_dir{prism_root / line.substr(line.find('=') + 1)};

  if (!fs::exists(instance_dir)) {
    std::cout << "(DEBUG) Instances not found.";
    return 0;
  }
  std::cout << "(DEBUG) Instances: " << instance_dir << std::endl;

  std::vector<std::filesystem::directory_entry> instances;

  for (auto const &dir_entry :
       std::filesystem::directory_iterator(instance_dir)) {
    if (std::filesystem::is_directory(dir_entry))
      instances.push_back(dir_entry);
  }

  std::cout << "Select the instance to be synced: \n";

  for (size_t idx = 0; idx < instances.size(); idx++) {
    std::string instance_path{instances[idx].path().string()};
    std::string instance_name{
        instance_path.substr(instance_path.find("/instances/") + 11)};
    std::cout << idx << ". " << instance_name << std::endl;
  }

  std::cout << "\nEnter a number (i.e. 0, 1, ...)\n";

  int selected_instance{};
  std::cin >> selected_instance;
  /*
  Prism paths per OS:
      Windows: %APPDATA%/PrismLauncher
      MacOS: ~/Library/Application Support/PrismLauncher
      Linux: ~/.local/share/PrismLauncher
  */

  return 0;
}
