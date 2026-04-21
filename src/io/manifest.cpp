#include "manifest.h"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "../core/hashing.h"

using json = nlohmann::json;
void write_manifest(const fs::path& destination_path,
                    const fs::path& instance_path) {
  json manifest;
  manifest["instance_name"] = instance_path.filename().string();
  manifest["last_updated"] = get_iso8601_time();
  manifest["tree"].push_back(build_tree(instance_path, instance_path));
  std::ofstream output_manifest(destination_path / "manifest.json");
  if (!output_manifest) {
    throw std::runtime_error("Unable to write manifest in " +
                             destination_path.string());
  }

  output_manifest << manifest.dump();

  output_manifest.close();
};

json build_tree(
    const fs::path& cur_path,
    const fs::path& instance_root) {  // recursively construct the tree
  json node;

  node["name"] = cur_path.filename().string();
  node["type"] = "directory";
  node["path"] = fs::relative(cur_path, instance_root).string();
  node["children"] = json::array();

  for (auto const& entry : fs::directory_iterator(cur_path)) {
    if (entry.is_directory()) {
      node["children"].push_back(build_tree(entry.path(), instance_root));
    } else {
      // call hash file. but i will add more members to HashedFile so that this
      // code can be simpler.
      json file_node;
      file_node["name"] = entry.path().filename().string();
      file_node["type"] = "file";
      file_node["path"] = fs::relative(entry.path(), instance_root).string();
      HashedFile hf(hash_file(entry));
      file_node["last_updated"] = hf.hash_time();
      file_node["block_size"] = hf.block_size();
      file_node["file_size"] = hf.file_size();
      file_node["hashes"] = hf.hashed_blocks();
      node["children"].push_back(file_node);
    }
  }
  return node;
};