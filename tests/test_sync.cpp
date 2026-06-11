#include "doctest.h"
#include "io/sync.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace blocksync::io;
using json = nlohmann::json;

static inline bool contains(std::vector<std::string> const &v,
                            std::string const &s) {
  return std::find(v.begin(), v.end(), s) != v.end();
}

TEST_CASE("Manifest diffs: new, unchanged, deleted") {
  PathMap m1{
      {"dir1/file1.txt", "original"},
      {"dir2/file1.txt", "original"},
      {"dir3/file1.txt", "original"},
  };

  PathMap m2{
      {"dir1/file1.txt", "changed"},  // changed
      {"dir2/file1.txt", "original"}, // unchanged
      {"dir3/fileD.txt", "new"},      // new
  };

  Diff d = compute_diff(m2, m1);

  CHECK(contains(d.to_send, "dir1/file1.txt"));
  CHECK(contains(d.to_send, "dir3/fileD.txt"));
  CHECK_FALSE(contains(d.to_send, "dir2/file1.txt"));
  CHECK(d.to_send.size() == 2);

  CHECK(d.to_delete.size() == 1);
  CHECK(d.to_delete[0] == "dir3/file1.txt");
}

TEST_CASE(
    "compute_diff is direction-sensitive; swapping manifests swaps diffs") {
  PathMap m1{{"dir1/file1.txt", "hash1"}};
  PathMap m2{{"dir2/file2.txt", "hash2"}};

  Diff d1 = compute_diff(m2, m1);
  CHECK(contains(d1.to_send, "dir2/file2.txt"));
  CHECK(contains(d1.to_delete, "dir1/file1.txt"));

  d1 = compute_diff(m1, m2);
  CHECK(contains(d1.to_send, "dir1/file1.txt"));
  CHECK(contains(d1.to_delete, "dir2/file2.txt"));
}

TEST_CASE(
    "Full pipeline: Differing manifests -> pass to diff_manifests and verify") {
  json current_json;
  json new_json;
  std::ifstream new_man{std::string(BLOCKSYNC_TEST_ASSETS) + "/new.json"};
  std::ifstream current_man{std::string(BLOCKSYNC_TEST_ASSETS) +
                            "/current.json"};
  REQUIRE(new_man.is_open());
  REQUIRE(current_man.is_open());
  current_json = json::parse(current_man);
  new_json = json::parse(new_man);
  new_man.close();
  current_man.close();
  Diff d = diff_manifests(current_json, new_json);
  // Manually modified to have one changed node, one new node, and one
  // unchanged node
  CHECK(d.to_send.size() == 2);
  CHECK(contains(d.to_send,
                 "mini_instance/saves/world/level.dat")); // This is changed
  CHECK(contains(d.to_send, "mini_instance/mods/newmod.jar")); // This is new
  CHECK_FALSE(
      contains(d.to_send,
               "mini_instance/config/options.txt")); // This is unchanged
  CHECK(d.to_delete.size() == 1);
  CHECK(contains(d.to_delete,
                 "mini_instance/mods/mymod.jar")); // Replaced by new one above
}
