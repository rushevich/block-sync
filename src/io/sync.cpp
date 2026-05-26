#include "sync.h"

#include <fstream>
#include <unordered_set>

namespace blocksync::io {

using json = nlohmann::json;

Diff find_differing_files(const std::string& current_manifest_name,
                          const std::string& new_manifest_name) {
  std::ifstream if1{current_manifest_name};
  json cur_man;
  if1 >> cur_man;

  std::ifstream if2{new_manifest_name};
  json new_man;
  if2 >> new_man;

  // strips useless fields to improve processing speed
  clean_manifest(cur_man["tree"]);
  clean_manifest(new_man["tree"]);

  // build path -> hashes maps for each manifest, then diff
  PathMap cur_map;
  PathMap new_map;

  flatten_to_map(cur_man.at("tree"), cur_map);
  flatten_to_map(new_man.at("tree"), new_map);

  return compute_diff(new_map, cur_map);
}

void flatten_to_map(const json& node, PathMap& out) {
  if (node.contains("children")) {
    for (const auto& child : node.at("children")) {
      flatten_to_map(child, out);
    }
  } else if (node.contains("hashes")) {
    out[node.at("path")] = node.at("hashes").get<std::vector<std::string>>();
  }
}

Diff compute_diff(const PathMap& source, const PathMap& dest) {
  Diff d;
  for (const auto& [path, hashes] : source) {
    auto it = dest.find(path);
    if (it == dest.end()) {
      d.to_send.push_back(path);
    } else if (it->second != hashes) {
      d.to_send.push_back(path);
    }
  }

  for (const auto& [path, hashes] : dest) {
    if (!source.contains(path)) {
      d.to_delete.push_back(path);
    }
  }
  return d;
}

// erase-safe (erase returns next iterator); walks both objects and arrays.
void clean_manifest(json& node) {
  static const std::unordered_set<std::string> to_clean{
      "name", "block_size", "file_size", "last_updated", "type"};

  if (node.is_object()) {
    for (auto it = node.begin(); it != node.end();) {
      if (to_clean.contains(it.key())) {
        it = node.erase(it);
      } else {
        clean_manifest(*it);
        ++it;
      }
    }
  } else if (node.is_array()) {
    for (auto& element : node) {
      clean_manifest(element);
    }
  }
}

}  // namespace blocksync::io
