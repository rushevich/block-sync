#ifndef BLOCKSYNC_IO_SYNC_H
#define BLOCKSYNC_IO_SYNC_H
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace blocksync::io {

using json = nlohmann::json;
// using Hashes = std::vector<std::string>;
// using PathMap = std::unordered_map<std::string, Hashes>;
using Hash = std::string;
using PathMap = std::unordered_map<std::string, Hash>;

struct Diff {
  std::vector<std::string> to_send;
  std::vector<std::string> to_delete;
};

Diff diff_manifests(json const &current_manifest, json const &new_manifest);
// diffs two manifest files (by path). returns paths to send / delete.
// NOTE: not yet wired into main.
Diff find_differing_files(std::string const &current_manifest_name,
                          std::string const &new_manifest_name);

// flattens a manifest tree node into path -> hashes
void flatten_to_map(nlohmann::json const &node, PathMap &out);

// path present+changed or new -> to_send; path absent in source -> to_delete
Diff compute_diff(PathMap const &source, PathMap const &dest);

// strips bookkeeping fields (name/type/sizes/last_updated) from a manifest tree
// in place, leaving only path + hashes + children. recurses objects and arrays.
void clean_manifest(nlohmann::json &node);

} // namespace blocksync::io
#endif
