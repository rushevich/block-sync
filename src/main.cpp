#include "util/helpers.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
/*
 Prism paths per OS:
     Windows: %APPDATA%/PrismLauncher
     MacOS: ~/Library/Application Support/PrismLauncher
     Linux: ~/.local/share/PrismLauncher
 */
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

// needs revision to trim whitespace
fs::path resolve_prism_instance_path(const fs::path &prism_root) {
  std::ifstream prism_config{prism_root / "prismlauncher.cfg"};
  if (!prism_config.is_open()) {
    return {};
  }

  std::string line;
  while (std::getline(prism_config, line)) {
    if (line.find("InstanceDir") != std::string::npos)
      break;
  }

  return (line.find("InstanceDir") == std::string::npos)
             ? fs::path{}
             : fs::path{prism_root / line.substr(line.find('=') + 1)};
}

void get_instance_selection(int &selected_instance,
                           const size_t &highest_instance_index) {
  while (true) {
    std::cout << "\nEnter a number (i.e. 0, 1, ...), or -1 to quit:\n";
    std::cin >> selected_instance;
    if (selected_instance == -1) return; 
    if (!std::cin ||
        (selected_instance < 0 || selected_instance > highest_instance_index)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid selection. Try again.";
      continue;
    }
    return;
  }
}

} // namespace

int main(int, char **) {
  detect_os();
  std::cout << "(DEBUG) User OS: " << user_os << std::endl;

  fs::path user_data_root{get_user_data_root()};
  std::cout << "(DEBUG) User Data Root: " << user_data_root << std::endl;

  fs::path prism_root{user_data_root / "PrismLauncher"};

  if (!fs::exists(prism_root)) {
    std::cerr << "(DEBUG) PrismLauncher directory not found.";
    return 1;
  }

  std::cout << "(DEBUG) PrismLauncher: " << prism_root << std::endl;

  fs::path instance_dir{resolve_prism_instance_path(prism_root)};
  if (instance_dir.empty() || !fs::is_directory(instance_dir)) {
    std::cerr << "(DEBUG): Instance directory is not valid.";
    return 1;
  }

  std::cout << "(DEBUG) Instances: " << instance_dir << std::endl;

  std::vector<std::filesystem::directory_entry> instances;

  for (auto const &dir_entry :
       std::filesystem::directory_iterator(instance_dir)) {
    if (std::filesystem::is_directory(dir_entry))
      instances.push_back(dir_entry);
  }

  if (instances.empty()) {
    std::cerr << "(DEBUG) No instances found.";
    return 1;
  }

  std::cout << "Select the instance to be synced: \n";

  for (size_t idx = 0; idx < instances.size(); idx++) {
    std::string instance_path{instances[idx].path().string()};
    std::string instance_name{instances[idx].path().filename().string()};
    std::cout << idx << ". " << instance_name << std::endl;
  }

  int selected_instance;
  get_instance_selection(selected_instance, instances.size()-1);
  if (selected_instance == -1) {
    std::cout << "Quitting...\n"; 
    return 0;
  }
  return 0;
}
