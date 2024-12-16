file(GLOB CRC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(crc STATIC ${CRC_SOURCES})

target_include_directories(crc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
