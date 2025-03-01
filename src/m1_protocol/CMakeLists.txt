file(GLOB M1_PROTOCOL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(m1_protocol STATIC ${M1_PROTOCOL_SOURCES})

# 头文件中include其他模块头文件需要在这添加
target_include_directories(m1_protocol PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/memory_pool/include
)

target_link_libraries(m1_protocol PRIVATE
    crc
    memory_pool
)
