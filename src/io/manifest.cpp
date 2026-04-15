#include "manifest.h"

#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
void write_manifest(const fs::path& destination_path,
                    const fs::path& instance_path) {
  json manifest;
  manifest["instance_name"] = instance_path.filename().string();
  manifest["last_updated"] = "";
  manifest["tree"].push_back(build_tree(instance_path, instance_path));
  /*flow:
  - use json pointer that is initially set to "/tree"
  - use recursive directory iterator

  * if current entry is a subdirectory:
  - at json pointer, create name, type, path keys, and then children key with
  value json array
  - set json pointer to /tree/subdir/children
  - proceed assuming that the recursive iterator now dives into this directory

  * if current entry is a file:
  - construct a new json object that we will append to the json pointer later
  - add all required fields, etc.
  - call hash file that will return HashedFile object, we can then simply
  construct the hashes key value pair
  - append this json object to the json pointer (assuming that the pointer is
  currently at some .../children)
  - cleanup if needed, recursive_iterator will proceed as needed.

  when done with loop, write manifest to block-sync config directory
  */

  /*
  would probably be easier to implement a recursive search in hindsight
  */
};

json build_tree(
    const fs::path& cur_path,
    const fs::path& instance_root) {  // recursively construct the tree
  json tree;

  for (auto const& entry : fs::directory_iterator(cur_path)) {
    tree["name"] = entry.path().filename().string();
    tree["path"] = (entry.is_directory()) ? "directory" : "file";
    tree["name"] = entry.path().filename().string();

    if (entry.is_directory()) {
      tree["children"].push_back(build_tree(entry.path(), instance_root));
    } else {
      // call hash file. but i will add more members to HashedFile so that this
      // code can be simpler.
    }
  }
  return tree;
};