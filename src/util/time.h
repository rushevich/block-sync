#ifndef BLOCKSYNC_UTIL_TIME_H
#define BLOCKSYNC_UTIL_TIME_H
#include <string>

namespace blocksync::util {

// current local time, ISO 8601 format
std::string get_iso8601_time();

}  // namespace blocksync::util
#endif
