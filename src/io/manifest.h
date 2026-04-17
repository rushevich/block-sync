#ifndef BLOCKSYNC_IO_MANIFEST_H
#define BLOCKSYNC_IO_MANIFEST_H
#include <filesystem>
#include <nlohmann/json.hpp>

#include "nlohmann/json_fwd.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;
void write_manifest(const fs::path& destination_path,
                    const fs::path& instance_path);

json build_tree(const fs::path&, const fs::path&);

#endif