#ifndef BLOCKSYNC_CLI_CONSOLE_H
#define BLOCKSYNC_CLI_CONSOLE_H
#include <filesystem>
#include <string_view>
#include <vector>

namespace blocksync::cli {

// immediate subdirectories of instance_dir (each a candidate instance)
std::vector<std::filesystem::directory_entry> list_instances(
    const std::filesystem::path& instance_dir);

// prints numbered menu and prompts. returns the chosen index, or -1 to quit
// (also -1 on EOF).
int select_instance(
    const std::vector<std::filesystem::directory_entry>& instances);

// y/n prompt loop. returns false on EOF.
bool confirm(std::string_view prompt);

}  // namespace blocksync::cli
#endif
