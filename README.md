# M1 Protocol

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE) [![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://example.com/build-status)

M1 Protocol 是一个专为嵌入式设备设计的轻量级通信协议栈，采用 C 语言实现，旨在提供高效、可靠的数据传输能力。该协议栈分为 数据链路层 、网络层 和 传输层 三层架构，每一层都专注于特定的功能模块，确保在资源受限的嵌入式环境中实现灵活且高效的通信。

## 目录

- [M1 Protocol](#m1-protocol)
  - [目录](#目录)
  - [背景](#背景)
  - [功能](#功能)
    - [数据链路层 (Data Link Layer)](#数据链路层-data-link-layer)
    - [网络层 (Network Layer)](#网络层-network-layer)
    - [传输层 (Transport Layer)](#传输层-transport-layer)
  - [特点与优势](#特点与优势)
  - [环境](#环境)
    - [**Linux**](#linux)
    - [**macOS**](#macos)
    - [**Windows**](#windows)
  - [使用方法](#使用方法)
  - [代码格式管理](#代码格式管理)
    - [代码格式化](#代码格式化)
  - [项目结构](#项目结构)
  - [贡献](#贡献)
    - [提交指南](#提交指南)
      - [示例](#示例)
    - [提交步骤](#提交步骤)
  - [许可证](#许可证)

## 背景

M1 Protocol 的目标是为嵌入式开发者提供一个简单易用、高效可靠的通信解决方案，帮助用户快速构建稳定的数据通信系统，同时降低开发成本和时间投入。

如果你正在寻找一个轻量级、模块化的嵌入式通信协议栈，M1 Protocol 将是一个理想的选择！

## 功能

### 数据链路层 (Data Link Layer)

数据链路层是协议栈的基础，负责与物理层接口交互，并处理帧的传输与解析。主要功能包括：

- 帧封装与解析：将上层数据封装成帧格式，并解析接收到的帧。
- 错误检测 ：通过 CRC（循环冗余校验）等机制检测数据传输中的错误，确保帧的完整性。
- 流量控制 ：根据硬件能力动态调整数据流，避免缓冲区溢出或数据丢失。
- 适配多种物理层接口 ：支持 UART、SPI、I2C 等常见物理层通信方式，便于嵌入式设备的集成。

### 网络层 (Network Layer)

网络层负责数据包的路由和转发，确保数据能够准确地从源设备传递到目标设备。主要功能包括：

- 路由管理 ：支持静态路由和动态路由算法，适用于小型网络拓扑。
- 地址解析 ：实现设备地址的映射与解析，支持多设备间的点对点通信。
- 分片与重组 ：对于超出单帧承载能力的数据包，进行分片传输并在接收端重组。
- 网络状态监控 ：实时监测网络连接状态，快速响应断连或异常情况。

### 传输层 (Transport Layer)

传输层提供端到端的可靠数据传输服务，确保数据在复杂网络环境下的完整性和顺序性。主要功能包括：

- 可靠传输 ：通过确认机制（ACK）和超时重传策略，保证数据包的成功送达。
- 数据确认 ：对接收到的数据包进行确认，防止数据丢失或重复。
- 流量控制与拥塞控制 ：根据网络状况动态调整发送速率，避免网络拥塞。
- 连接管理 ：支持面向连接的通信模式，简化上层应用的开发。

## 特点与优势

- 轻量化设计 ：专为嵌入式设备优化，占用极少的内存和计算资源，适合资源受限的场景。
- 模块化架构 ：各层功能独立实现，便于裁剪和扩展，可根据具体需求定制协议栈。
- 高可靠性 ：通过 CRC 校验、确认重传等机制，确保数据传输的准确性和稳定性。
- 跨平台兼容 ：支持多种物理层接口和硬件平台，易于移植到不同的嵌入式系统中。
- 易于集成 ：提供清晰的 API 接口和详细的文档说明，方便开发者快速上手。

## 环境

> [Visual Studio Code 中使用 C++](https://code.visualstudio.com/docs/cpp/introvideos-cpp)
>
> 对应环境在Visual Studio Code 官网文档中心都有详细说明，点击对应链接查看配置过程

### **Linux**

> [GCC on Linux](https://code.visualstudio.com/docs/cpp/config-linux)

### **macOS**

> [Clang on macOS](https://code.visualstudio.com/docs/cpp/config-clang-mac)

### **Windows**

> [Microsoft C++ on Windows](https://code.visualstudio.com/docs/cpp/config-msvc)
>
> [GCC on Windows](https://code.visualstudio.com/docs/cpp/config-mingw)

请确保已安装以下工具：

> ClangFormat包含在LLVM软件包中
>
> 安装后将对应软件的`bin`目录，添加到系统环境变量 PATH 中，以便在命令行中方便地访问

- [LLVM](https://llvm.org/)
  - [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html): 代码格式化工具
- [CMake](https://cmake.org/) : 跨平台的构建系统生成工具
- [Ninja](https://ninja-build.org/): 小巧且高效的构建系统
- [Python](https://www.python.org): 脚本编写

## 使用方法

## 代码格式管理

项目中配置了 `clang-format`，用于统一代码风格。你可以在 `.vscode` 的 `settings.json` 中配置自动格式化支持，或者手动运行格式化脚本 `scripts/run-clang-format-from-config.py`。

### 代码格式化

该项目包含 `.clang-format` 和 `clang-format-config.json` 文件，可以使用 `clang-format` 工具统一代码风格。在 `.vscode` 目录中还设置了代码格式的自动化管理。

格式化命令：

```bash
python .\scripts\run-clang-format-from-config.py
```

## 项目结构

```plaintext
TEMPLATE
│
├── .vscode                  # VSCode 配置，支持 clang-format
│
├── build                    # 构建生成目录
│
├── cmake                    # CMake 配置
│   ├── gcc.cmake          # GCC 编译器的 CMake 配置
│   └── msvc.cmake           # msvc 编译器的 CMake 配置
│
├── scripts                  # 脚本目录
│   └── run-clang-format-from-config.py # 格式化脚本
│
├── src                      # 源代码目录
│   ├── crc                  # crc模块
│   ├── memory_pool          # 内存池模块
│   └── m1_protocol          # m1_protocol模块
│
├── test                     # 单元测试目录
│   ├── crc                  # crc模块测试
│   ├── memory_pool          # memory_pool模块测试
│   └── m1_protocol          # m1_protocol模块测试
│
├── .clang-format            # Clang 格式化配置文件
├── .gitignore               # Git 忽略文件
├── CMakeLists.txt           # 顶层 CMake 配置文件
├── LICENSE                  # 许可证
└── README.md                # 项目说明文件
```

## 贡献

欢迎任何形式的贡献！如果你发现任何问题或希望增加新功能，请提交 Issue 或 Pull Request。

### 提交指南

遵循 [约定式提交](https://www.conventionalcommits.org/zh-hans/) 规范。请确保你的提交信息符合以下格式：

<类型>(<范围>): <描述>

其中：

- `类型`：用于描述提交的类别，常见的类型包括：
  - `feat`：新功能
  - `fix`：修复问题
  - `docs`：仅文档更新
  - `style`：不影响代码含义的更改（例如格式化、缺少分号等）
  - `refactor`：既不修复错误也不添加功能的代码更改
  - `test`：添加缺失的测试或修改现有测试
  - `chore`：构建过程或辅助工具的变动
- `范围`：提交影响的模块或文件，可以省略。
- `描述`：简要说明修改内容。

#### 示例

- `feat(calculator): 添加乘法功能`
- `fix(stack): 修复栈溢出问题`
- `docs(readme): 更新安装说明`
- `chore(ci): 添加 GitHub Actions 配置`

### 提交步骤

1. Fork 此仓库
2. 创建分支 (`git checkout -b feat/your-feature`)
3. 按照约定式提交规范提交更改 (`git commit -m 'feat(scope): 添加新功能'`)
4. 推送到分支 (`git push origin feat/your-feature`)
5. 创建一个 Pull Request

## 许可证

此项目基于 `MIT`许可证。详细信息请查看 `LICENSE` 文件。
