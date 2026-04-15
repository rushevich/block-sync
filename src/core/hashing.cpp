#include "hashing.h"

#include <openssl/evp.h>

#include <chrono>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <memory>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

#define MEGABYTE 1024 * 1024

HashedFile::HashedFile(fs::path p)
    : hashed_blocks_{}, file_path_{p}, write_time_{} {};

HashedFile::HashedFile(HashedFile&& other) noexcept
    : hashed_blocks_(std::move(other.hashed_blocks_)),
      file_path_(std::move(other.file_path_)),
      write_time_(other.write_time_) {};

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
  for (size_t idx = 0; idx < lengthOfHash; ++idx) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[idx];
  }

  return ss.str();
};

// reads the file in chunks of 1MB and hashes each chunk
HashedFile hash_file(const fs::path& input_file_path) {
  HashedFile hf{input_file_path};

  const size_t block_size = MEGABYTE;
  std::vector<unsigned char> buf(block_size);
  std::ifstream file(input_file_path, std::ios::binary);

  while (file.read(reinterpret_cast<char*>(buf.data()),
                   static_cast<std::streamsize>(block_size)) ||
         file.gcount() > 0) {
    std::streamsize actual_bytes = file.gcount();
    hf.add_hash(
        hash_block(std::span(buf.data(), static_cast<size_t>(actual_bytes))));
  }
  return hf;
};
