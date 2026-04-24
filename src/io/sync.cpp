#include "sync.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>

#include "../util/helpers.h"
#include "manifest.h"

// incomplete implementation
std::vector<std::string> find_differing_files(std::string current_manifest,
                                              std::string new_manifest) {
  std::ifstream if1{current_manifest};
  json cur_man;
  if1 >> cur_man;

  std::ifstream if2{new_manifest};
  json new_man;
  if2 >> new_man;

  std::vector<std::string> to_fetch;

  // this seems like a pretty difficult problem:
  /*
  - we need to iterate through both json trees and find both of:
    - if there are new files in the new manifest
    - if files that are shared between current and new differ
  - this can be done with two pointers. but this can be costly for performance:
  come across file A in new_manifest -> check if current_manifest has file A
  -> search  O(log n) -> if it has this, check if block hashes differ -> 1 MB
  hashes means time complexity not bad


  */

  // will make a function to clean manifests to simplify processing
  // we can also decide to automatically mark a file for syncing if the update
  // times dont match. but this requires further infrastructure

  // strips useless fields to improve processing speed
  clean_manifest(cur_man["tree"]);
  clean_manifest(new_man["tree"]);

  // we can do this by constructing a filepath : hashes  map data structure for
  // each manifest, then for each pair in the new_man map, check if the cur_man
  // map contains an identical key and then compare the hashes if so. if we
  // encounter any files that dont have a matching key, they must be added to
  // the return vector along with conflicting hash paths

  std::unordered_map<std::string, std::vector<std::string>> cur_map;
  std::unordered_map<std::string, std::vector<std::string>> new_map;

  // since clean manifest deletes fields type, name, updated, sizes, we can
  // safely assume that if the lambda is called, it only has hashes and path
  // available
  auto add_map_fields = [&](json::iterator& it) {
    std::vector<std::string> hashes;
    if (it.key() == "hashes")
      hashes = it.value();
    else {
    }
  };

  return {};
};

// returns true if the file has changed
bool file_changed(const std::vector<std::string>& current_hashes,
                  const std::vector<std::string>& new_hashes) {
  if (current_hashes[0] != new_hashes[0])
    return true;
  else if (current_hashes.size() != new_hashes.size())
    return true;
  for (size_t i{}; i < current_hashes.size(); ++i) {
    if (current_hashes[i] != new_hashes[i]) return true;
  }
  return false;
};