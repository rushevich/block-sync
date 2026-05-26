#ifndef BLOCKSYNC_PRISM_PRISM_H
#define BLOCKSYNC_PRISM_PRISM_H
#include <filesystem>

namespace blocksync::prism {

// reads prismlauncher.cfg under prism_root and resolves the configured
// InstanceDir; empty path if not found
std::filesystem::path resolve_instance_path(const std::filesystem::path& prism_root);

}  // namespace blocksync::prism
#endif
