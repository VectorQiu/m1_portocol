add_subdirectory(crc)
add_subdirectory(memory_pool)
add_subdirectory(m1_protocol)
add_subdirectory(h1_protocol)

file(GLOB MAIN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

# 定义可执行文件 main，指定 src/main.c 作为源文件
add_executable(main ${MAIN_SOURCES})

target_link_libraries(main PRIVATE
    m1_protocol
    h1_protocol
)
