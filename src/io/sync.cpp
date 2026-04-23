#include "sync.h"

bool hash_diff(std::string hash1, std::string hash2) {
  bool result = (hash1 != hash2);
  return result;
}