#ifndef BLOCKSYNC_PLATFORM_PLATFORM_H
#define BLOCKSYNC_PLATFORM_PLATFORM_H
#include <filesystem>
#include <string>

namespace blocksync::platform {

// "WINDOWS" | "LINUX" | "MACOS"
std::string detect_os();

// reads an environment variable as a path; empty path if unset/empty
std::filesystem::path env_path(const char* name);

// per-OS root for application data, derived from the given os string
std::filesystem::path user_data_root(const std::string& os);

}  // namespace blocksync::platform
#endif
