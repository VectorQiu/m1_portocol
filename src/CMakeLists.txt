add_subdirectory(crc)
add_subdirectory(m1_protocol)
add_subdirectory(memory_pool)

# 定义可执行文件 main，指定 src/main.c 作为源文件
add_executable(main main.c)

target_link_libraries(main PRIVATE m1_protocol)