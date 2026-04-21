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

int main(int, char**) {
  // std::cout << "(DEBUG) User OS: " << detect_os() << std::endl;
  std::ignore = detect_os();

  // std::cout << "(DEBUG) User Data Root: " << get_user_data_root() <<
  // std::endl;

  fs::path instance_dir{};
  if (!config_found())
    instance_dir = initial_setup(get_user_data_root());
  else
    instance_dir = retrieve_instance_dir();

  if (instance_dir.empty()) {
    std::cerr << "Initial setup failed. Check to make sure you haven't moved "
                 "the PrismLauncher folder.";
    return 1;
  }

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
