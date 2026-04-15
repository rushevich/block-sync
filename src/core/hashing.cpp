#include "hashing.h"

#include <openssl/evp.h>

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <memory>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

#define MEGABYTE 1024 * 1024

HashedFile::HashedFile(const fs::path& p, const uintmax_t& sz)
    : hashed_blocks_{},
      file_path_{p},
      hash_time_{},
      block_size_{sz},
      file_size_{fs::file_size(p)} {};

HashedFile::HashedFile(HashedFile&& other) noexcept
    : hashed_blocks_{std::move(other.hashed_blocks_)},
      file_path_{std::move(other.file_path_)},
      hash_time_{std::move(other.hash_time_)},
      block_size_(std::move(other.block_size_)),
      file_size_(std::move(other.file_size_)) {};

std::string hash_block(const std::span<const unsigned char>& block_data) {
  auto ctx = std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)>(
      EVP_MD_CTX_new(), EVP_MD_CTX_free);
  unsigned char hash[EVP_MAX_MD_SIZE /* =64 */];
  unsigned int lengthOfHash{};

  EVP_DigestInit_ex(ctx.get(), EVP_sha256(), NULL);
  EVP_DigestUpdate(ctx.get(), block_data.data(), block_data.size());
  EVP_DigestFinal_ex(ctx.get(), hash, &lengthOfHash);

  EVP_MD_CTX_free(ctx.get());

  std::stringstream ss;
  for (size_t idx{}; idx < lengthOfHash; ++idx) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[idx];
  }

  return ss.str();
};

// reads the file in chunks of 1MB and hashes each chunk
HashedFile hash_file(const fs::path& input_file_path) {
  HashedFile hf{input_file_path, MEGABYTE};

  uintmax_t block_size{MEGABYTE};
  std::vector<unsigned char> buf(block_size);
  std::ifstream file(input_file_path, std::ios::binary);

  while (file.read(reinterpret_cast<char*>(buf.data()),
                   static_cast<std::streamsize>(block_size)) ||
         file.gcount() > 0) {
    std::streamsize actual_bytes{file.gcount()};
    hf.add_hash(
        hash_block(std::span(buf.data(), static_cast<size_t>(actual_bytes))));
  }

  hf.update_write_time();
  return hf;
};
