#ifndef BLOCKSYNC_CORE_HASHING_H
#define BLOCKSYNC_CORE_HASHING_H

#include <string>
#include <vector>

std::string hash_block(const std::vector<unsigned char>& block_data);

// it would be good to write a temporary file containing the indexed hashes
std::vector<std::string> hash_file(const std::string& path_to_file);

#endif