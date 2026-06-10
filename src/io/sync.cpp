#include "sync.h"

#include <fstream>
#include <unordered_set>

namespace blocksync::io {

using json = nlohmann::json;

Diff diff_manifests(json const &current_manifest, json const &new_manifest) {
  json nw = new_manifest; // copy
  json cur = current_manifest;
  clean_manifest(cur["tree"]);
  clean_manifest(nw["tree"]);
  PathMap cur_map, new_map;
  flatten_to_map(cur.at("tree"), cur_map);
  flatten_to_map(nw.at("tree"), new_map);
  return compute_diff(new_map, cur_map);
}

Diff find_differing_files(std::string const &current_manifest_name,
                          std::string const &new_manifest_name) {
  std::ifstream if1{current_manifest_name};
  std::ifstream if2{new_manifest_name};
  json cur_man;
  json new_man;
  if1 >> cur_man;
  if2 >> new_man;
  return diff_manifests(cur_man, new_man);
}

void flatten_to_map(json const &node, PathMap &out) {
  if (node.contains("children")) {
    for (auto const &child : node.at("children")) {
      flatten_to_map(child, out);
    }
  } else if (node.contains("hashes")) {
    // out[node.at("path")] =
    // node.at("hashes").get<std::vector<std::string>>();
    out[node.at("path")] = node.at("hash").get<std::string>();
  }
}

Diff compute_diff(PathMap const &current, PathMap const &old) {
  Diff d;
  for (auto const &[path, hashes] : current) {
    auto it = old.find(path);
    if (it == old.end()) {
      d.to_send.push_back(path);
    } else if (it->second != hashes) {
      d.to_send.push_back(path);
    }
  }

  for (auto const &[path, hashes] : old) {
    if (!current.contains(path)) {
      d.to_delete.push_back(path);
    }
  }
  return d;
}

void clean_manifest(json &node) {
  static std::unordered_set<std::string> const to_clean{
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
    for (auto &element : node) {
      clean_manifest(element);
    }
  }
}

} // namespace blocksync::io
