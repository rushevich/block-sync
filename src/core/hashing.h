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
    explicit HashedFile(
        std::filesystem::path const& p /*, const std::uintmax_t& sz*/);

    HashedFile() = delete; // disallow inexplicit construction

    // implicit, but for clarity, delete
    HashedFile(HashedFile const&)            = delete; // copy constructor
    HashedFile& operator=(HashedFile const&) = delete; // copy assignment

    HashedFile(HashedFile&& other) noexcept = default;

    HashedFile& operator=(HashedFile&& other) noexcept = default;

    std::filesystem::path const& file_path() const { return m_file_path; }

    std::string hash_time() const { return m_hash_time; }

    void update_write_time() { m_hash_time = util::get_iso8601_time(); }

    // std::vector<std::string> const& hashed_blocks() const {
    //     return hashed_blocks_;
    // }
    inline auto hash() const { return m_hash; }

    void set_hash(std::string hash) { m_hash = hash; };

    std::uintmax_t file_size() const { return m_file_size; }

    void update_file_size(size_t upd) { m_file_size = upd; }

    // std::uintmax_t block_size() const { return block_size_; }

  private:
    std::filesystem::path m_file_path;
    std::string           m_hash_time; // iso 8601 fmt
    std::string           m_hash;
    std::uintmax_t        m_file_size; // in bytes
                                // std::vector<std::string> hashed_blocks_;
                                // std::uintmax_t block_size_;  // in bytes
};

// std::string hash_block(const std::span<const unsigned char>& block_data);

// HashedFile hash_file(std::filesystem::path const& input_file_path);

HashedFile hash_file_once(std::filesystem::path const& input_file_path);

} // namespace blocksync::core

#endif
