#ifndef BLOCKSYNC_CORE_HASHING_H
#define BLOCKSYNC_CORE_HASHING_H

#include <filesystem>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "../util/helpers.h"

namespace fs = std::filesystem;

struct HashedFile {
  HashedFile(const fs::path& p, const uintmax_t& sz);

  HashedFile() = delete;  // disallow inexplicit construction

  HashedFile(const HashedFile&) = delete;             // copy constructor
  HashedFile& operator=(const HashedFile&) = delete;  // copy assignment

  HashedFile(HashedFile&& other) noexcept;

  HashedFile& operator=(HashedFile&& other) noexcept {
    if (this != &other) {
      hashed_blocks_ = std::move(other.hashed_blocks_);
      file_path_ = std::move(other.file_path_);
      hash_time_ = std::move(other.hash_time_);
      block_size_ = std::move(other.block_size_);
      file_size_ = std::move(other.file_size_);
    }
    return *this;
  };  // move assignment

  const fs::path& file_path() const { return file_path_; }

  std::string hash_time() const { return hash_time_; }

  void update_write_time() { hash_time_ = get_iso8601_time(); }

  const std::vector<std::string>& hashed_blocks() const {
    return hashed_blocks_;
  }

  void add_hash(std::string hash) {
    hashed_blocks_.emplace_back(std::move(hash));
  };

  const uintmax_t file_size() const { return file_size_; }

  const uintmax_t block_size() const { return block_size_; }

 private:
  fs::path file_path_;
  std::string hash_time_;  // iso 8601 fmt
  std::vector<std::string> hashed_blocks_;
  uintmax_t block_size_;  // in bytes
  uintmax_t file_size_;   // in bytes
};

std::string hash_block(const std::span<const unsigned char>& block_data);

HashedFile hash_file(const fs::path& input_file_path);

#endif