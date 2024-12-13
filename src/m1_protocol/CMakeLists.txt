file(GLOB M1_PROTOCOL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(m1_protocol STATIC ${M1_PROTOCOL_SOURCES})

target_include_directories(m1_protocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(m1_protocol PRIVATE crc)
