#include <iostream>
#include <fstream>
#include <filesystem>

int main(int, char**){
    std::cout << "Hello, from block-sync!\nPlease enter the path to your /PrismLauncher directory: ";
    
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

    return 0;
}
