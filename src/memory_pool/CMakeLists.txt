file(GLOB MEMORY_POOL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(memory_pool STATIC ${MEMORY_POOL_SOURCES})

target_include_directories(memory_pool PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
