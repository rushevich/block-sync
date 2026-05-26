#ifndef BLOCKSYNC_IO_SYNC_H
#define BLOCKSYNC_IO_SYNC_H
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using Hashes = std::vector<std::string>;
using PathMap = std::unordered_map<std::string, Hashes>;

struct Diff {
    std::vector<std::string> to_send;
    std::vector<std::string> to_delete;
};
// the goal of this header file is to provide functionality for syncing between
// manifests

// returns a vector containing relative (to instance root) paths to all
// differing files. assume that both files are json.
auto find_differing_files(std::string current_manifest_name,
                                              std::string new_manifest_name);

// true if blocks differ
bool file_changed(const Hashes& current_hashes,
                  const Hashes& new_hashes);

void flatten_to_map(const json& node, PathMap& out);

Diff compute_diff(const PathMap& source, const PathMap& dest);

#endif
