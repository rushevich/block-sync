add_executable(blocksync_tests
    test_main.cpp
    test_hashing.cpp
    test_sync.cpp
)
target_include_directories(blocksync_tests PRIVATE ${DOCTEST_INCLUDE_DIR})
target_link_libraries(blocksync_tests PRIVATE blocksync_lib)

add_test(NAME blocksync_tests COMMAND blocksync_tests)
