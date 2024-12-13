# 设置系统名称和处理器
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64) # 或者根据你的目标架构选择 arm 或其他
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# 设置编译器标志
set(FLAGS "-Wall -Wextra -O2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS} -std=gnu++20")

# 设置可执行文件后缀
set(CMAKE_EXECUTABLE_SUFFIX ".exe")

# 设置链接器（可选，通常 GCC 会自动处理）
set(CMAKE_LINKER "gcc") # GCC 自带链接器，不需要特别指定
