#ifndef BLOCKSYNC_CORE_HASHING_H
#define BLOCKSYNC_CORE_HASHING_H

#include <chrono>
#include <string>
#include <utility>
#include <vector>
#include <span>

struct HashedFile {
  HashedFile(std::vector<std::string> v, std::string p,
             std::chrono::system_clock::time_point t)
      : hashed_blocks_(std::move(v)),
        file_path_(std::move(p)),
        write_time_(t) {}

  HashedFile() = delete;  // disallow inexplicit construction

  HashedFile(const HashedFile&) = delete;             // copy constructor
  HashedFile& operator=(const HashedFile&) = delete;  // copy assignment

  HashedFile(HashedFile&& other) noexcept
      : hashed_blocks_(std::move(other.hashed_blocks_)),
        file_path_(std::move(other.file_path_)),
        write_time_(other.write_time_) {}  // move constructor

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

  const auto get_hashed_block() { return hashed_blocks_; }

 private:
  // below are effectively immutable
  std::string file_path_;
  std::chrono::system_clock::time_point write_time_;
  std::vector<std::string> hashed_blocks_;
};

std::string hash_block(const std::span<const unsigned char>& block_data);

HashedFile hash_file(const std::string& path_to_file);

#endif