#ifndef BLOCKSYNC_CORE_HASHING_H
#define BLOCKSYNC_CORE_HASHING_H

#include <chrono>
#include <filesystem>
#include <span>
#include <string>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

struct HashedFile {
  explicit HashedFile(fs::path p);

  HashedFile() = delete;  // disallow inexplicit construction

  HashedFile(const HashedFile&) = delete;             // copy constructor
  HashedFile& operator=(const HashedFile&) = delete;  // copy assignment

  HashedFile(HashedFile&& other) noexcept;

  HashedFile& operator=(HashedFile&& other) noexcept {
    if (this != &other) {
      hashed_blocks_ = std::move(other.hashed_blocks_);
      file_path_ = std::move(other.file_path_);
      write_time_ = other.write_time_;
    }
    return *this;
  };  // move assignment

  const std::string get_file_path() { return file_path_; }

  const auto get_write_time() { return write_time_; }

  void update_write_time() { write_time_ = std::chrono::system_clock::now(); }

  const auto get_hashed_blocks() { return hashed_blocks_; }

  void add_hash(std::string hash) { hashed_blocks_.emplace_back(hash); };

 private:
  // below are effectively immutable
  fs::path file_path_;
  std::chrono::system_clock::time_point write_time_;
  std::vector<std::string> hashed_blocks_;
};

std::string hash_block(const std::span<const unsigned char>& block_data);

HashedFile hash_file(const fs::path& input_file_path);

#endif