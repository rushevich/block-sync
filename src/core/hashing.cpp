#include "hashing.h"

#include <format>
#include <ios>
#include <openssl/evp.h>

#include <cstddef>
#include <fstream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

namespace blocksync::core {

constexpr std::size_t kBlockSize = 1024 * 1024;

HashedFile::HashedFile(fs::path const& p /*const std::uintmax_t& sz*/)
    : m_file_path {p}, m_hash_time {}, m_hash {},
      // m_hashed_blocks{},
      // block_size_{sz},
      m_file_size {fs::file_size(p)} {};

// std::string hash_block(const std::span<const unsigned char>& block_data) {
//   auto ctx = std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)>(
//       EVP_MD_CTX_new(), EVP_MD_CTX_free);
//   unsigned char hash[EVP_MAX_MD_SIZE /* =64 */];
//   unsigned int lengthOfHash{};
//
//   EVP_DigestInit_ex(ctx.get(), EVP_sha256(), NULL);
//   EVP_DigestUpdate(ctx.get(), block_data.data(), block_data.size());
//   EVP_DigestFinal_ex(ctx.get(), hash, &lengthOfHash);
//
//   std::stringstream ss;
//   for (size_t idx{}; idx < lengthOfHash; ++idx) {
//     ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[idx];
//   }
//
//   return ss.str();
// };
//
// // reads the file in chunks of 1MB and hashes each chunk
// HashedFile hash_file(const fs::path& input_file_path) {
//   HashedFile hf{input_file_path/*, kBlockSize*/};
//
//   // std::uintmax_t block_size{kBlockSize};
//   // std::vector<unsigned char> buf(block_size);
//   std::ifstream file(input_file_path, std::ios::binary);
//
//   while (file.read(reinterpret_cast<char*>(buf.data()),
//                    static_cast<std::streamsize>(block_size)) ||
//          file.gcount() > 0) {
//     std::streamsize actual_bytes{file.gcount()};
//     hf.add_hash(
//         hash_block(std::span(buf.data(),
//         static_cast<size_t>(actual_bytes))));
//   }
//
//   hf.update_write_time();
//   return hf;
// };

HashedFile hash_file_once(fs::path const& input_file_path) {
    std::ifstream file(input_file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error(
            std::format("Could not open file {})", input_file_path.string()));
    }
    HashedFile hf {input_file_path};
    auto       ctx = std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)>(
        EVP_MD_CTX_new(), EVP_MD_CTX_free);
    EVP_DigestInit_ex(ctx.get(), EVP_sha256(), NULL);
    std::vector<unsigned char> buf(kBlockSize);
    size_t                     total_bytes {};
    while (file.read(reinterpret_cast<char*>(buf.data()),
                     static_cast<std::streamsize>(kBlockSize)) ||
           file.gcount() > 0) {
        auto n = file.gcount();
        EVP_DigestUpdate(ctx.get(), buf.data(), n);
        total_bytes += n;
    }
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int  lengthOfHash {};
    EVP_DigestFinal_ex(ctx.get(), hash, &lengthOfHash);
    std::stringstream ss;
    for (size_t idx {}; idx < lengthOfHash; ++idx) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[idx];
    }
    hf.set_hash(ss.str());
    hf.update_file_size(total_bytes);
    hf.update_write_time();
    return hf;
}

} // namespace blocksync::core
