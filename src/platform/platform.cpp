#include "platform.h"

#include <cstdlib>

namespace fs = std::filesystem;

namespace blocksync::platform {

std::string detect_os() {
#if defined(_WIN32)
  return "WINDOWS";
#elif defined(__linux__)
  return "LINUX";
#elif defined(__APPLE__)
  return "MACOS";
#else
  return {};
#endif
}

fs::path env_path(const char* name) {
  const char* source{std::getenv(name)};
  if (!source || !*source) {
    return {};
  }
  return fs::path(source);
}

fs::path user_data_root(const std::string& os) {
  if (os == "WINDOWS") {
    return env_path("APPDATA");
  }
  if (os == "LINUX") {
    fs::path home{env_path("HOME")};
    if (home.empty()) {
      return {};
    }
    return home / ".local" / "share";
  }
  if (os == "MACOS") {
    fs::path home{env_path("HOME")};
    if (home.empty()) {
      return {};
    }
    return home / "Library" / "Application Support";
  }
  return {};
}

}  // namespace blocksync::platform
