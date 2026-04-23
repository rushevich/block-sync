include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)
cmake_minimum_required(VERSION 3.15)
project(block-sync VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)  

add_executable(block-sync src/main.cpp)

target_sources(block-sync PRIVATE ${CMAKE_SOURCE_DIR}/src/core/hashing.cpp ${CMAKE_SOURCE_DIR}/src/util/helpers.cpp ${CMAKE_SOURCE_DIR}/src/io/manifest.cpp
    src/io/sync.cpp)

target_link_libraries(block-sync PRIVATE OpenSSL::Crypto nlohmann_json::nlohmann_json tomlplusplus)