file(GLOB TEST_MEMORY_POOL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(test_memory_pool ${TEST_MEMORY_POOL_SOURCES})

target_link_libraries(test_memory_pool PRIVATE
    GTest::GTest
    GTest::Main
    memory_pool
)
