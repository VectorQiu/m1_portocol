# M1 Protocol

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE) [![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://example.com/build-status)

这是一个使用 CMake 构建的 C 项目模板，包含源代码和单元测试结构，支持 Clang 编译器，并配置了代码格式管理。

## 目录

- [M1 Protocol](#m1-protocol)
  - [目录](#目录)
  - [背景](#背景)
  - [功能](#功能)
  - [环境](#环境)
    - [**Linux**](#linux)
    - [**macOS**](#macos)
    - [**Windows**](#windows)
  - [使用方法](#使用方法)
    - [运行项目](#运行项目)
  - [代码格式管理](#代码格式管理)
    - [代码格式化](#代码格式化)
  - [项目结构](#项目结构)
  - [贡献](#贡献)
    - [提交指南](#提交指南)
      - [示例](#示例)
    - [提交步骤](#提交步骤)
  - [许可证](#许可证)

## 背景

该项目旨在提供一个 C 项目的基本模板，适合中小型项目的快速初始化。它包括代码模块、单元测试框架、代码格式化支持等，帮助开发者建立一致的开发环境和项目结构。

## 功能

- 使用 `CMake` 配置和构建
- 使用 `GoogleTest` 框架的单元测试
- 使用 `VScode Debugging`
- 使用 `clang-format` 代码格式管理
- `crc` 和 `m1_protocol` 功能模块

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

### 运行项目

以下代码展示如何编写并运行一个简单的测试示例：

```bash

```

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
│   └── clang.cmake          # Clang 编译器的 CMake 配置
│
├── scripts                  # 脚本目录
│   └── run-clang-format-from-config.py # 格式化脚本
│
├── src                      # 源代码目录
│   ├── crc                  # crc模块
│   └── m1_protocol          # m1_protocol模块
│
├── test                     # 单元测试目录
│   ├── crc                  # crc模块测试
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
