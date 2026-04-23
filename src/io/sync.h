#ifndef BLOCKSYNC_IO_SYNC_H
#define BLOCKSYNC_IO_SYNC_H
#include <string>
#include <vector>
// the goal of this header file is to provide functionality for syncing between
// manifests

// returns a vector of the differing hash indices
std::vector<int> find_hash_diffs(std::vector<std::string> current_hashes,
                                 std::vector<std::string> new_hashes);

// returns true if the hashes differ and false otherwise.
bool hash_diff(std::string hash1, std::string hash2);
#endif
