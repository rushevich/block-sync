#ifndef BLOCKSYNC_IO_MANIFEST_H
#define BLOCKSYNC_IO_MANIFEST_H
#include <filesystem>
#include "../core/hashing.h"

namespace fs = std::filesystem;

void write_manifest(const fs::path& destination_path);

#endif