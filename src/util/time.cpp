#include "time.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace blocksync::util {

std::string get_iso8601_time() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << (std::put_time(std::localtime(&in_time_t), "%FT%T%Z"));
  return ss.str();
}

}  // namespace blocksync::util
