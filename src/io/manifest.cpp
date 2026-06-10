#include "manifest.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "core/hashing.h"
#include "util/time.h"

namespace fs = std::filesystem;
using json   = nlohmann::json;

namespace blocksync::io {

void write_manifest(fs::path const& destination_path,
                    fs::path const& instance_path) {
    json manifest;
    manifest["instance_name"] = instance_path.filename().string();
    manifest["last_updated"]  = util::get_iso8601_time();
    manifest["tree"].push_back(build_tree(instance_path, instance_path));
    std::ofstream output_manifest(destination_path / "manifest.json");
    if (!output_manifest) {
        throw std::runtime_error("Unable to write manifest in " +
                                 destination_path.string());
    }

    output_manifest << manifest.dump();

    output_manifest.close();
};

json build_tree(fs::path const& cur_path, fs::path const& instance_root) {
    json node;

    node["name"]     = cur_path.filename().string();
    node["type"]     = "directory";
    node["path"]     = fs::relative(cur_path, instance_root).string();
    node["children"] = json::array();

    for (auto const& entry : fs::directory_iterator(cur_path)) {
        if (entry.is_directory()) {
            node["children"].push_back(build_tree(entry.path(), instance_root));
        } else {
            json file_node;
            file_node["name"] = entry.path().filename().string();
            file_node["type"] = "file";
            file_node["path"] =
                fs::relative(entry.path(), instance_root).string();
            core::HashedFile hf(core::hash_file_once(entry));
            file_node["last_updated"] = hf.hash_time();
            // file_node["block_size"]   = hf.block_size();
            file_node["file_size"] = hf.file_size();
            file_node["hashes"]    = hf.hash();
            node["children"].push_back(file_node);
        }
    }
    return node;
};

} // namespace blocksync::io
