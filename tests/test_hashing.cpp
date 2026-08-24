#include "core/hashing.h"
#include <doctest/doctest.h>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;
using namespace blocksync::core;

TEST_CASE("hash_file_once matches known SHA256 of 'abc'") {
  fs::path tmp = fs::temp_directory_path() / "blocksync_test_abc.bin";
  {
    std::ofstream tmp_file(tmp, std::ios::binary);
    tmp_file << "abc";
  }

  HashedFile hf = hash_file_once(tmp);
  CHECK(hf.hash() ==
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
  CHECK(hf.file_size() == 3);
  fs::remove(tmp);
}

TEST_CASE("hash_file_once can handle files larger than one chunk") {
  fs::path tmp = fs::temp_directory_path() / "blocksync_test_large.bin";
  std::size_t sz{1024 * 1024 * 5};
  {
    std::ofstream tmp_file(tmp, std::ios::binary);
    tmp_file << std::string(sz, 'x');
  }
  HashedFile hf = hash_file_once(tmp);
  CHECK(hf.file_size() == sz);
  CHECK(hf.hash() ==
        "dba67a476fa78973aabb087f214a1010f3bebca053674e0af50dfe5a582112be");
  // generated with: python3 -c
  // "open('/tmp/b','wb').write(b'x'*(1024*1024*5))" && sha256sum /tmp/b
  fs::remove(tmp);
}
