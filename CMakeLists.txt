cmake_minimum_required(VERSION 3.15)
project(block-sync VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

find_package(OpenSSL REQUIRED)

add_executable(block-sync
    src/main.cpp
    src/cli/console.cpp
    src/config/config.cpp
    src/platform/platform.cpp
    src/prism/prism.cpp
    src/core/hashing.cpp
    src/io/manifest.cpp
    src/io/sync.cpp
    src/util/time.cpp
)

target_include_directories(block-sync PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(block-sync PRIVATE OpenSSL::Crypto nlohmann_json::nlohmann_json tomlplusplus::tomlplusplus)
