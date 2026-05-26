#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "cli/console.h"
#include "config/config.h"
#include "io/manifest.h"
#include "platform/platform.h"

/*
 Prism paths per OS:
     Windows: %APPDATA%/PrismLauncher
     MacOS: ~/Library/Application Support/PrismLauncher
     Linux: ~/.local/share/PrismLauncher
 */
namespace fs = std::filesystem;

int main(int, char **) {
    using namespace blocksync;

    config::AppConfig cfg{};
    cfg.os = platform::detect_os();
    cfg.user_data_root = platform::user_data_root(cfg.os);

    fs::path instance_dir{};
    if (!config::config_found(cfg))  // resolves cfg.bs_config_path
        instance_dir = config::initial_setup(cfg);
    else
        instance_dir = config::retrieve_instance_dir(cfg);

    if (instance_dir.empty()) {
        std::cerr << "Initial setup failed. Check to make sure you haven't moved "
                "the PrismLauncher folder.";
        return 1;
    }

    auto instances = cli::list_instances(instance_dir);
    if (instances.empty()) {
        std::cerr << "(DEBUG) No instances found.";
        return 1;
    }

    int selected = cli::select_instance(instances);
    if (selected == -1) {
        std::cout << "Quitting...\n";
        return 0;
    }

    std::cout << "(DEBUG) Selected instance: "
            << instances[selected].path().filename() << "\n\n";

    if (!cli::confirm("Confirm instance to sync? [y/n]: ")) return 0;

    std::cout << "Proceeding with sync...\n";
    io::write_manifest(cfg.bs_config_path, instances[selected].path());
    std::cout << "Manifest written.\n";
    return 0;
}
