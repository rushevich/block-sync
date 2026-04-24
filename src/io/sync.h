#ifndef BLOCKSYNC_IO_SYNC_H
#define BLOCKSYNC_IO_SYNC_H
#include <string>
#include <vector>
// the goal of this header file is to provide functionality for syncing between
// manifests

// returns a vector containing relative (to instance root) paths to all
// differing files. assume that both files are json.
std::vector<std::string> find_differing_files(std::string current_manifest,
                                              std::string new_manifest);

// true if blocks differ
bool file_changed(const std::vector<std::string>& current_hashes,
                  const std::vector<std::string>& new_hashes);

#endif
