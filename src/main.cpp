#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "io/manifest.h"
#include "util/helpers.h"

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

// retrieves path for large user data

// needs revision to trim whitespace

}  // namespace

int main(int, char**) {
  std::cout << "(DEBUG) User OS: " << detect_os() << std::endl;

  std::cout << "(DEBUG) User Data Root: " << get_user_data_root() << std::endl;

  if (!config_found()) {
    std::cout << "(DEBUG) block-sync config folder doesn't exist yet.\n";
    create_config_directory();
    std::cout << "(DEBUG) New config folder created at " << get_bs_config_path()
              << std::endl;
  } else {
    std::cout << "(DEBUG) block-sync config found at " << get_bs_config_path()
              << std::endl;
  }
  auto prism_root{get_user_data_root() / "PrismLauncher"};

  if (!fs::exists(prism_root)) {
    std::cerr << "(DEBUG) PrismLauncher directory not found.";
    return 1;
  }

  std::cout << "(DEBUG) PrismLauncher: " << prism_root << std::endl;

  auto instance_dir{resolve_prism_instance_path(prism_root)};
  if (instance_dir.empty() || !fs::is_directory(instance_dir)) {
    std::cerr << "(DEBUG): Instance directory is not valid.";
    return 1;
  }

  std::cout << "(DEBUG) Instances: " << instance_dir << std::endl;

  std::vector<std::filesystem::directory_entry> instances{};

  for (auto const& dir_entry :
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
    std::string instance_name{instances[idx].path().filename().string()};
    std::cout << idx << ". " << instance_name << std::endl;
  }

  int selected_instance{};
  get_instance_selection(selected_instance, instances.size() - 1);
  if (selected_instance == -1) {
    std::cout << "Quitting...\n";
    return 0;
  }

  /*
  next to do:
  - create function, most likely in core, to recursively hash the instance
  directory and store it to a temporary file in user data.
  - create function to write manifest.yaml file
  - add functionality to create block-sync config file+folder in user data
  directory --> would help with modularizing main and avoiding path deduction
  every time
  */

  std::cout << "(DEBUG) Selected instance: "
            << instances[selected_instance].path().filename() << std::endl;
  std::cout << "\n";
  char choice;
  while (true) {
    std::cout << "Confirm instance to sync? [y/n]: ";
    std::cin >> choice;
    if (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n') {
      std::cout << "Invalid input. Please enter Y or N." << std::endl;
      continue;
    }
    break;
  };
  if (choice == 'N' || choice == 'n') return 0;

  std::cout << "Proceeding with sync...\n";
  write_manifest(get_bs_config_path(),
                 fs::path(instances[selected_instance].path()));
  std::cout << "Manifest written.\n";
  return 0;
}
