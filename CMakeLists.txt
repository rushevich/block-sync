cmake_minimum_required(VERSION 3.15)
project(block-sync VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(blocksync_warnings INTERFACE)
target_compile_options(blocksync_warnings INTERFACE -Wall -Wextra -Werror)

set(DOCTEST_VERSION v2.4.11)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(doctest REQUIRED)
find_package(tomlplusplus REQUIRED)
add_library(blocksync_lib STATIC
    src/cli/console.cpp
    src/config/config.cpp
    src/platform/platform.cpp
    src/prism/prism.cpp
    src/core/hashing.cpp
    src/io/manifest.cpp
    src/io/sync.cpp
    src/util/time.cpp
)
target_include_directories(blocksync_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(blocksync_lib
    PUBLIC OpenSSL::Crypto nlohmann_json::nlohmann_json tomlplusplus::tomlplusplus
    PRIVATE blocksync_warnings)

add_executable(block-sync src/main.cpp)
target_link_libraries(block-sync PRIVATE blocksync_lib blocksync_warnings)

enable_testing()
add_subdirectory(tests)
