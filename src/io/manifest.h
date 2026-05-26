#ifndef BLOCKSYNC_IO_MANIFEST_H
#define BLOCKSYNC_IO_MANIFEST_H
#include <filesystem>
#include <nlohmann/json.hpp>

namespace blocksync::io {

void write_manifest(const std::filesystem::path& destination_path,
                    const std::filesystem::path& instance_path);

// recursively builds the manifest tree node for cur_path, paths relative to
// instance_root
nlohmann::json build_tree(const std::filesystem::path& cur_path,
                          const std::filesystem::path& instance_root);

}  // namespace blocksync::io
#endif
