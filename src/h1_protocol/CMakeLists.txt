file(GLOB H1_PROTOCOL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(h1_protocol STATIC ${H1_PROTOCOL_SOURCES})

# 头文件中include其他模块头文件需要在这添加
target_include_directories(h1_protocol PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(h1_protocol PRIVATE
    m1_protocol
)
