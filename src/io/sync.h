#ifndef BLOCKSYNC_IO_SYNC_H
#define BLOCKSYNC_IO_SYNC_H
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace blocksync::io {

using Hashes = std::vector<std::string>;
using PathMap = std::unordered_map<std::string, Hashes>;

struct Diff {
  std::vector<std::string> to_send;
  std::vector<std::string> to_delete;
};

// diffs two manifest files (by path). returns paths to send / delete.
// NOTE: not yet wired into main.
Diff find_differing_files(const std::string& current_manifest_name,
                          const std::string& new_manifest_name);

// flattens a manifest tree node into path -> hashes
void flatten_to_map(const nlohmann::json& node, PathMap& out);

// path present+changed or new -> to_send; path absent in source -> to_delete
Diff compute_diff(const PathMap& source, const PathMap& dest);

// strips bookkeeping fields (name/type/sizes/last_updated) from a manifest tree
// in place, leaving only path + hashes + children. recurses objects and arrays.
void clean_manifest(nlohmann::json& node);

}  // namespace blocksync::io
#endif
