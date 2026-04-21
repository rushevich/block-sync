#include "helpers.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <toml++/toml.hpp>

namespace fs = std::filesystem;
std::string user_os{};
fs::path user_data_root{};
fs::path bs_config_path{};

std::string detect_os() {
#if defined(_WIN32)
  user_os = "WINDOWS";
#elif defined(__linux__)
  user_os = "LINUX";
#elif defined(__APPLE__)
  user_os = "MACOS";
#endif
  return user_os;
}

bool config_found() {
  if (user_os == "LINUX")
    bs_config_path = user_data_root / ".." / ".." / ".config" / "block-sync";
  else
    bs_config_path = user_data_root / "block-sync";
  return (fs::exists(bs_config_path)) ? true : false;
}

std::string get_iso8601_time() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << (std::put_time(std::localtime(&in_time_t), "%FT%F%Z"));
  return ss.str();
}

void create_config_directory() {
  if (fs::create_directories(bs_config_path)) {
    std::cout << "Config dir created \n";
  } else {
    std::cout << "Creation failed or dir already exists\n";
  }
  return;
}

fs::path env_path(const char* name) {
  const char* source{std::getenv(name)};
  if (!source || !*source) {
    return {};
  }
  return fs::path(source);
}

fs::path get_user_data_root() {
  if (!user_data_root.empty()) {
    return user_data_root;
  }
  if (user_os == "WINDOWS") {
    user_data_root = fs::path{env_path("APPDATA")};
    return user_data_root;
  }
  if (user_os == "LINUX") {
    fs::path home{env_path("HOME")};
    if (home.empty()) {
      return {};
    }
    user_data_root = home / ".local" / "share";
    return user_data_root;
  }
  if (user_os == "MACOS") {
    fs::path home{env_path("HOME")};
    if (home.empty()) {
      return {};
    }
    user_data_root = home / "Library" / "Application Support";
    return user_data_root;
  }
  return {};
}

fs::path get_bs_config_path() { return bs_config_path; }

fs::path resolve_prism_instance_path(const fs::path& prism_root) {
  std::ifstream prism_config{prism_root / "prismlauncher.cfg"};
  if (!prism_config.is_open()) {
    return {};
  }

  std::string line;
  while (std::getline(prism_config, line)) {
    if (line.find("InstanceDir") != std::string::npos) break;
  }

  return (line.find("InstanceDir") == std::string::npos)
             ? fs::path{}
             : fs::path{prism_root / line.substr(line.find('=') + 1)};
}

void get_instance_selection(int& selected_instance,
                            const size_t& highest_instance_index) {
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

fs::path initial_setup(const fs::path& user_data_root) {
  // std::cout << "(DEBUG) block-sync config folder doesn't exist yet.\n";
  create_config_directory();
  // std::cout << "(DEBUG) New config folder created at " <<
  // get_bs_config_path()
  // << std::endl;

  fs::path prism_root{get_user_data_root() / "PrismLauncher"};

  if (!fs::exists(prism_root)) {
    // std::cerr << "(DEBUG) PrismLauncher directory not found.";
    return {};
  }

  // std::cout << "(DEBUG) PrismLauncher: " << prism_root << std::endl;

  auto instance_dir{resolve_prism_instance_path(prism_root)};
  if (instance_dir.empty() || !fs::is_directory(instance_dir)) {
    // std::cerr << "(DEBUG): Instance directory is not valid.";
    return {};
  }

  std::ofstream bs_config(get_bs_config_path() / "config.toml");
  // std::cout << "(DEBUG) Instances: " << instance_dir << std::endl;
  if (!bs_config) {
    return {};
  }

  auto info_tbl = toml::table{{"prism", prism_root.string()},
                              {"instances", instance_dir.string()}};

  bs_config << info_tbl;
  return instance_dir;
}

fs::path retrieve_instance_dir() {
  toml::table info_tbl =
      toml::parse_file((get_bs_config_path() / "config.toml").string());

  std::optional<std::string> instances =
      info_tbl["instances"].value<std::string>();

  return {*instances};
};
