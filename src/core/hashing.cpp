#include "hashing.h"
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <openssl/evp.h>
#include <sstream>

#define MEGABYTE 1024 * 1024

std::string hash_block(const std::vector<unsigned char> &block_data) {
  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  unsigned char hash[EVP_MAX_MD_SIZE /* =64 */];
  unsigned int lengthOfHash{};

  EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
  EVP_DigestUpdate(ctx, block_data.data(), block_data.size());
  EVP_DigestFinal_ex(ctx, hash, &lengthOfHash);

  EVP_MD_CTX_free(ctx);

  std::stringstream ss;
  for (size_t idx = 0; idx < lengthOfHash; ++idx) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[idx];
  }

  return ss.str();
};

// reads the file in chunks of 1MB and hashes each chunk
std::vector<std::string> hash_file(const std::string &path_to_file) {
  std::ifstream file(path_to_file, std::ios::binary);
  std::vector<std::string> hashes;
  const size_t block_size = MEGABYTE;
  std::vector<unsigned char> buf(block_size);

  while (file.read(reinterpret_cast<char *>(buf.data()),
                   static_cast<std::streamsize>(block_size)) ||
         file.gcount() > 0) {
    std::streamsize actual_bytes = file.gcount();
    hashes.push_back(hash_block(std::vector<unsigned char>(
        buf.begin(), buf.begin() + static_cast<std::ptrdiff_t>(actual_bytes))));
  }
  return hashes;
};