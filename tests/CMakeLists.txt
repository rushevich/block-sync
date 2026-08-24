add_executable(blocksync_tests
    test_main.cpp
    test_hashing.cpp
    test_sync.cpp
  )

target_link_libraries(blocksync_tests PRIVATE blocksync_lib doctest::doctest)
target_compile_definitions(blocksync_tests PRIVATE
    BLOCKSYNC_TEST_ASSETS="${CMAKE_CURRENT_SOURCE_DIR}/assets")
add_test(NAME blocksync_tests COMMAND blocksync_tests)
