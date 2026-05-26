#include "console.h"

#include <iostream>
#include <limits>

namespace fs = std::filesystem;

namespace blocksync::cli {

namespace {

// prompts until a valid index in [0, highest] is entered, or -1 to quit.
// returns -1 on EOF.
int prompt_index(std::size_t highest) {
  int selected{};
  while (true) {
    std::cout << "\nEnter a number (i.e. 0, 1, ...), or -1 to quit:\n";
    if (!(std::cin >> selected)) {
      if (std::cin.eof()) return -1;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid selection. Try again.";
      continue;
    }
    if (selected == -1) return -1;
    if (selected < 0 || static_cast<std::size_t>(selected) > highest) {
      std::cout << "Invalid selection. Try again.";
      continue;
    }
    return selected;
  }
}

}  // namespace

std::vector<fs::directory_entry> list_instances(const fs::path& instance_dir) {
  std::vector<fs::directory_entry> instances;
  for (auto const& entry : fs::directory_iterator(instance_dir)) {
    if (entry.is_directory()) instances.push_back(entry);
  }
  return instances;
}

int select_instance(const std::vector<fs::directory_entry>& instances) {
  std::cout << "Select the instance to be synced: \n";
  for (std::size_t idx = 0; idx < instances.size(); ++idx) {
    std::cout << idx << ". " << instances[idx].path().filename().string()
              << std::endl;
  }
  return prompt_index(instances.size() - 1);
}

bool confirm(std::string_view prompt) {
  char choice;
  while (true) {
    std::cout << prompt;
    if (!(std::cin >> choice)) return false;  // EOF / stream fail -> treat as no
    if (choice == 'Y' || choice == 'y') return true;
    if (choice == 'N' || choice == 'n') return false;
    std::cout << "Invalid input. Please enter Y or N." << std::endl;
  }
}

}  // namespace blocksync::cli
