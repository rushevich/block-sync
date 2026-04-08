#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "util/helpers.h"

int main(int, char**){
    detect_os();
    std::cout << "(DEBUG) User OS: " << user_os << std::endl;

    std::cout << "Hello, from block-sync!\nPlease enter the path to your PrismLauncher directory: ";
    
    std::string inp;
    std::cin >> inp;

    std::filesystem::path prism_path{inp};

    if (!std::filesystem::exists(prism_path)) {
        std::cout << "The path you entered is not valid.\n";
        return 1;
    }

    
    std::ifstream prism_config{prism_path / "prismlauncher.cfg"};

    if (!prism_config) {
        std::cout << "Your prismlauncher.cfg file was not found.\n";
        return 1;
    }

    std::cout << "Found prismlauncher.cfg.\n";

    std::string line;
    while (prism_config >> line) {
        if (line.find("InstanceDir") != std::string::npos) break;
    }

    // assuming that it is a relative path in the .cfg file
    std::string instance_dir{prism_path / line.substr(line.find('=') + 1)};

    if (!std::filesystem::exists(instance_dir)) {
        std::cout << "Unable to find /instances directory.\n";
        return 1;
    }

    std::cout << "Found instances directory: " << instance_dir << std::endl;

    std::vector<std::filesystem::directory_entry> instances;

    for (auto const& dir_entry : std::filesystem::directory_iterator(instance_dir)) {
        if (std::filesystem::is_directory(dir_entry)) instances.push_back(dir_entry);
    }

    std::cout << "Select the instance to be synced: \n";

    for (size_t idx = 0; idx < instances.size(); idx++) {
        std::string instance_path{instances[idx].path().string()};
        std::string instance_name{instance_path.substr(instance_path.find("/instances/")+11)};
        std::cout << idx << ". " << instance_name << std::endl;
    }

    std::cout << "\nEnter a number (i.e. 0, 1, ...)\n";
    
    int selected_instance{};
    std::cin >> selected_instance;
    
    return 0;
}
