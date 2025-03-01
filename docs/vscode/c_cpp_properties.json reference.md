# c_cpp_properties.json reference



本文介绍了 `c_cpp_properties.json` 设置文件的方案。

想要开始配置您的项目？请参阅 [Configure Intellisense](https://code.visualstudio.com/docs/cpp/configure-intellisense)。有关更改这些设置的更多信息，请参阅 [自定义默认设置](https://code.visualstudio.com/docs/cpp/customize-default-settings-cpp)。

## [变量示例](https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference#_example-of-variables)

注意，这是所有字段的示例。您不必在 `c_cpp_properties.json` 文件中指定所有字段。扩展会自动使用默认值填写任何缺失的字段。

```json
{
    "env": {
        "myIncludePath": [
            "${workspaceFolder}/src/calculator/include",
            "${workspaceFolder}/src/stack/include"
        ],
        "myDefines": [
            "_DEBUG",
            "UNICODE",
            "_UNICODE",
            "_WINDOWS"
        ],
        "mySourcePath": [
            "${workspaceFolder}/src/calculator",
            "${workspaceFolder}/src/stack"
        ]
    },
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${env:myIncludePath}"
            ],
            "defines": [
                "${env:myDefines}"
            ],
            "configurationProvider": "ms-vscode.cmake-tools",
            "compilerPath": "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.41.34120/bin/Hostx64/x64/cl.exe",
            "compilerArgs": [],
            "cStandard": "c11",
            "cppStandard": "c++17",
            // "windowsSdkVersion": "",
            "intelliSenseMode": "windows-msvc-x64",
            "mergeConfigurations": false,
            "browse": {
                "path": [
                        "${env:myIncludePath}"
                ],
                "limitSymbolsToIncludedHeaders": true
            }
        },
        {
            "name": "Linux",
            "includePath": [
                "${env:myIncludePath}"
                // "/usr/include"
            ],
            "defines": [
                "${env:myDefines}"
            ],
            "configurationProvider": "ms-vscode.cmake-tools",
            "compilerPath": "",
            "compilerArgs": ["-m32"],
            "intelliSenseMode": "linux-gcc-x86",
            "cStandard": "gnu11",
            "cppStandard": "gnu++14",
            "forcedInclude": [
            ],
            "compileCommands": "",
            "dotConfig": "",
            "mergeConfigurations": true,
            "customConfigurationVariables": {
            },
            "browse": {
                "path": [
                    "${env:myIncludePath}"
                ],
                "limitSymbolsToIncludedHeaders": true,
                "databaseFilename": "${workspaceFolder}/.vscode/browse.vc.db"
            }
        },
        {
            "name": "Mac",
            "includePath": [
                "${env:myIncludePath}"
            ],
            "defines": [
                "${env:myDefines}"
            ],
            "configurationProvider": "ms-vscode.cmake-tools",
            "compilerPath": "",
            "intelliSenseMode": "macos-clang-x64",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "macFrameworkPath": [
            ],
            "browse": {
                "path": [
                    "${env:myIncludePath}"
                ]
            }
        }
    ],
    "version": 4,
    "enableConfigurationSquiggles": true
}
```

## [顶级属性](https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference#_toplevel-properties)

- `env` 用户定义的变量数组，可通过标准环境变量语法在配置中使用：${<var>}` 或 `${env:<var>}`。接受字符串和字符串数组。
- `configurations` 一个配置对象数组，为 IntelliSense 引擎提供有关项目和偏好的信息。默认情况下，扩展会根据操作系统为您创建一个配置。您也可以添加其他配置。
- `version` 我们建议您不要编辑此字段。它跟踪当前版本的`c_cpp_properties.json` 文件，这样扩展程序就知道应该有哪些属性和设置，以及如何将该文件升级到最新版本。
- `enableConfigurationSquiggles` 设为 `true`，则向 C/C++ Extension 报告在 `c_cpp_properties.json` 文件中检测到的错误。

## [配置属性](https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference#_configuration-properties)

- `name` 标识配置的友好名称。

  - Linux、Mac 和 Win32 是配置的特殊标识符，将在这些平台上自动选择。
  - VS 代码中的状态栏会显示哪个配置处于活动状态。
  - 您还可以选择状态栏中的标签来更改活动配置。

- `compilerPath` (可选） 用于构建项目的编译器的完整路径，例如 `/usr/bin/gcc`, 以启用更准确的智能提示。

  - 该扩展将查询编译器，以确定 IntelliSense 使用的系统包含路径和默认定义。
  - 放 置 `"compilerPath": ""` (空字符串）将跳过查询编译器。
  - 如果指定的编译器不支持用于查询的参数，这将非常有用，因为扩展程序会默认返回到它能找到的任何编译器（如 Visual C）。
  - 省略 compilerPath 属性不会跳过查询。

- `compilerArgs`(可选）用于修改包含或定义的编译器参数，例如 -nostdinc++、-m32 等。

  - 使用额外空格分隔的参数应在数组中作为单独参数输入，例如，对于 --sysroot <arg> 使用 （"--sysroot/"）、 （"<arg>/"）。

- `intelliSenseMode` T映射到 MSVC、gcc 或 Clang 的特定架构变体的 IntelliSense 模式。

  如果未设置或设置为 `${default}`，扩展将选择该平台的默认模式。

  平台默认值：

  - Windows: `windows-msvc-x64`
  - Linux: `linux-gcc-x64`

  - macOS: `macos-clang-x64`

  仅指定 <compiler>-<architecture> 变体（例如 gcc-x64）的 IntelliSense 模式是传统模式，会根据主机平台自动转换为 <platform>-<compiler>-<architecture> 变体。

- `includePath` 包含路径是指包含在源文件中的头文件（如 #include "myHeaderFile.h"）的文件夹。

  - 指定一个路径列表，以便 IntelliSense 引擎在搜索包含的头文件时使用。
  - 在这些路径上的搜索不是递归的。指定 ** 表示递归搜索。
  - 例如，${workspaceFolder}/** 将搜索所有子目录，而 ${workspaceFolder} 则不会。
  - 如果在安装了 Visual Studio 的 Windows 系统中，或者在编译器路径设置中指定了编译器，则无需在此列表中列出系统包含路径。

- `defines` 供 IntelliSense 引擎在解析文件时使用的预处理器定义列表。

  - 可选择使用 = 来设置值，例如 VERSION=1。

- `cStandard` 用于 IntelliSense 的 C 语言标准版本。

  - 例如，c17、gnu23 或 ${default}。
  - 请注意，GNU 标准仅用于查询集编译器以获取 GNU 定义，IntelliSense 将模拟等效的 C 标准版本。

- `cppStandard` 用于 IntelliSense 的 C++ 语言标准版本。

  - 例如，"c++20"、"gnu++23 "或"${default}"。
  - 注意：GNU 标准仅用于查询集编译器以获取 GNU 定义，IntelliSense 将模拟等效的 C++ 标准版本。

- `configurationProvider` 可为源文件提供 IntelliSense 配置信息的 VS Code 扩展 ID。

  - 例如，使用 VS 代码扩展 ID `ms-vscode.cmake-tools` 来提供 CMake 工具扩展的配置信息。
  - 如果您指定了一个 configurationProvider，那么它提供的配置将优先于您在`c_cpp_properties.json`.
  -  `configurationProvider`候选扩展必须实施 vscode-cpptools-api。

- `windowsSdkVersion` Windows SDK 的版本包括在 Windows 上使用的路径，例如`10.0.17134.0`.

- `macFrameworkPath` 供 IntelliSense 引擎搜索 Mac 框架包含的头文件时使用的路径列表。仅支持 macOS 配置。

- `forcedInclude` (可选）在处理源文件中的任何其他字符前应包含的文件列表。文件按列出的顺序包含。

- `compileCommands` (可选）工作区的 compile_commands.json 文件的完整路径。

  - 如果在编辑器中打开的文件在 compile_commands.json 文件中有匹配的条目，则将使用该命令行来配置该文件的 IntelliSense，而不是 c_cpp_properties.json 文件中的其他字段。
  - 有关文件格式的更多信息，请参阅 Clang 文档。某些编译系统（如 CMake）会简化此文件的生成。

- `dotConfig` Kconfig 系统生成的 .config 文件的路径。

  - Kconfig 系统生成的文件包含构建项目所需的所有定义。
  - 使用 Kconfig 系统的项目有 Linux 内核和 NuttX 实时操作系统。

- `mergeConfigurations` 设置为 "true "可将包含路径、定义和强制包含与来自配置提供程序的路径、定义和强制包含合并。

- `customConfigurationVariables` 可通过 ${cpptools:activeConfigCustomVariable} 命令查询的自定义变量，用于 launch.json 或 tasks.json 中的输入变量。

- `browse` 当 "C_Cpp.intelliSenseEngine "设置为 "Tag Parser"（也称为 "模糊 "IntelliSense 或 "浏览 "引擎）时使用的属性集。

- 当 "默认 "IntelliSense 引擎无法解析源文件中的 #includes 时，转到定义/声明功能也会使用这些属性。

### [Browse properties](https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference#_browse-properties)

- `path` 标签解析器搜索源文件中包含的头文件的路径列表。
  - 如果省略，"includePath "将被用作 "path"。默认情况下，在这些路径上的搜索是递归的。
  - 指定 `*` 表示非递归搜索。例如：`${workspaceFolder}` 将搜索所有子目录，而 `${workspaceFolder}/*`则不会。
- `limitSymbolsToIncludedHeaders` 
  - 为 "true "时，标签解析器将只解析直接或间接包含在"${workspaceFolder}`"源文件中的代码文件。`
  -  为"false"时，标签解析器将解析在 `browse.path` 列表中指定的路径下找到的所有代码文件。
- `databaseFilename` 生成的符号数据库的路径。
  - 该属性指示扩展程序将标签解析器的符号数据库保存到工作区默认存储位置以外的其他位置。
  - 如果指定了相对路径，则该路径将相对于工作区的默认存储位置，而不是工作区文件夹本身。
  - 可使用 `${workspaceFolder}` 变量指定相对于工作区文件夹的路径（例如 `${workspaceFolder}/.vscode/browse.vc.db`)

## [Supported variables](https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference#_supported-variables)

您可以让 `tasks.json` 或 `launch.json` 从 `c_cpp_properties.json` 中查询当前活动配置。

为此，请在 `tasks.json` 或 `launch.json` 脚本中使用变量 `${command:cpptools.activeConfigName}` 作为参数。