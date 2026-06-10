#include "doctest.h"
#include "io/sync.h"
#include <string>
#include <vector>

using namespace blocksync::io;

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
