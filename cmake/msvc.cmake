# 设置系统名称和处理器
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)  # 或者根据你的目标架构选择 arm 或其他

# 设置编译器（默认情况下 CMake 会自动检测 MSVC 编译器）
# 不需要手动设置 CMAKE_C_COMPILER 和 CMAKE_CXX_COMPILER

# 设置编译器标志
set(FLAGS "/W4 /O2 /utf-8")  # 使用 MSVC 的编译器标志
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")

# 设置可执行文件后缀
set(CMAKE_EXECUTABLE_SUFFIX ".exe")

# 链接器（MSVC 会自动处理，无需手动设置）
