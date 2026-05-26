#include "prism.h"

#include <fstream>
#include <string>

namespace fs = std::filesystem;

namespace blocksync::prism {

fs::path resolve_instance_path(const fs::path& prism_root) {
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

}  // namespace blocksync::prism
