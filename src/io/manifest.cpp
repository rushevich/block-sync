#include "manifest.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;
void write_manifest(const fs::path& destination_path,
                    const std::string& instance_name) {
  json manifest;
  manifest["instance_name"] = instance_name;
  manifest["last_updated"] = "";
  manifest["tree"] = json::array();
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
};