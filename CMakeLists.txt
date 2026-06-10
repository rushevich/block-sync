cmake_minimum_required(VERSION 3.15)
project(block-sync VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(blocksync_warnings INTERFACE)
target_compile_options(blocksync_warnings INTERFACE -Wall -Wextra)

include(FetchContent)

FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(tomlplusplus
    URL https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.4.0.tar.gz
)
FetchContent_MakeAvailable(tomlplusplus)

set(DOCTEST_VERSION v2.4.11)
FetchContent_Declare(doctest
    URL https://raw.githubusercontent.com/doctest/doctest/${DOCTEST_VERSION}/doctest/doctest.h
    DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(doctest)
set(DOCTEST_INCLUDE_DIR ${doctest_SOURCE_DIR})

find_package(OpenSSL REQUIRED)

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
