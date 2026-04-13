#include "helpers.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string user_os;

void detect_os() {
#if defined(_WIN32)
  user_os = "WINDOWS";
#elif defined(__linux__)
  user_os = "LINUX";
#elif defined(__APPLE__)
  user_os = "MACOS";
#endif
}

bool config_found() { return true; }

std::string get_iso8601_time() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << (std::put_time(std::localtime(&in_time_t), "%FT%F%Z"));
  return ss.str();
}