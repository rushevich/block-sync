#ifndef BLOCKSYNC_CORE_HASHING_H
#define BLOCKSYNC_CORE_HASHING_H

#include <cstdint>
#include <filesystem>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "util/time.h"

namespace blocksync::core {

struct HashedFile {
  HashedFile(const std::filesystem::path& p, const std::uintmax_t& sz);

  HashedFile() = delete;  // disallow inexplicit construction

  HashedFile(const HashedFile&) = delete;             // copy constructor
  HashedFile& operator=(const HashedFile&) = delete;  // copy assignment

  HashedFile(HashedFile&& other) noexcept;

  HashedFile& operator=(HashedFile&& other) noexcept {
    if (this != &other) {
      file_path_ = std::move(other.file_path_);
      hash_time_ = std::move(other.hash_time_);
      hashed_blocks_ = std::move(other.hashed_blocks_);
      block_size_ = other.block_size_;
      file_size_ = other.file_size_;
    }
    return *this;
  };  // move assignment

  const std::filesystem::path& file_path() const { return file_path_; }

  std::string hash_time() const { return hash_time_; }

  void update_write_time() { hash_time_ = util::get_iso8601_time(); }

  const std::vector<std::string>& hashed_blocks() const {
    return hashed_blocks_;
  }

  void add_hash(std::string hash) {
    hashed_blocks_.emplace_back(std::move(hash));
  };

  std::uintmax_t file_size() const { return file_size_; }

  std::uintmax_t block_size() const { return block_size_; }

 private:
  std::filesystem::path file_path_;
  std::string hash_time_;  // iso 8601 fmt
  std::vector<std::string> hashed_blocks_;
  std::uintmax_t block_size_;  // in bytes
  std::uintmax_t file_size_;   // in bytes
};

std::string hash_block(const std::span<const unsigned char>& block_data);

HashedFile hash_file(const std::filesystem::path& input_file_path);

}  // namespace blocksync::core

#endif
