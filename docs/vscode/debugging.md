# 在 Visual Studio Code 中调试 C++

> [Debug C++ in Visual Studio Code](https://code.visualstudio.com/docs/cpp/cpp-debug)
>
> [Debugging](https://code.visualstudio.com/docs/editor/debugging)

## 介绍

Visual Studio Code 支持多种 C/C++ 调试器，具体取决于你使用的操作系统：

- **Linux**: GDB
- **macOS**: LLDB 或 GDB
- **Windows**: Visual Studio Windows Debugger 或 GDB（使用 Cygwin 或 MinGW）

## 在 Windows 上使用 GDB 调试

使用 Cygwin 或 MinGW 开发的 Windows 应用程序可以通过 VS Code 进行调试。要使用 Cygwin 或 MinGW 的调试功能，必须在 `launch.json` 配置中手动设置调试器路径。要调试 Cygwin 或 MinGW 应用程序，添加 `miDebuggerPath` 属性，并将其值设置为对应的 `gdb.exe` 的路径。

例如：

```json
"miDebuggerPath": "c:\\mingw\\bin\\gdb.exe"
```

Cygwin/MinGW 调试在 Windows 上支持附加和启动调试场景。

## 条件断点

条件断点使你可以在某一行代码上仅当条件为真时中断执行。要设置条件断点，右键单击现有的断点，选择“编辑断点”。这将打开一个小窗口，你可以在其中输入必须评估为真的条件。

在编辑器中，条件断点用黑色等号的断点符号表示。你可以将光标悬停在条件断点上以查看其条件。

## 函数断点

函数断点使你可以在函数开始时中断执行，而不是在特定的代码行上。要设置函数断点，在“运行”视图中右键单击“断点”部分，然后选择“添加函数断点”，输入你希望中断执行的函数名称。

## 表达式求值

VS Code 支持在多个上下文中评估表达式：

- 你可以在“监视”部分输入表达式，每次触发断点时会进行评估。
- 你可以在调试控制台输入表达式，只有在一次评估时才会计算。
- 当你在断点处停止时，可以评估代码中的任何表达式。

“监视”部分中的表达式会在调试的应用程序中生效；修改变量值的表达式将在程序执行期间有效。

## 多线程调试

VS Code 的 C/C++ 扩展能够调试多线程程序。所有线程及其调用堆栈将显示在调用堆栈部分。

## 内存转储调试

VS Code 的 C/C++ 扩展也支持内存转储调试。要调试内存转储，请打开 `launch.json` 文件，并添加 `coreDumpPath`（适用于 GDB 或 LLDB）或 `dumpPath`（适用于 Visual Studio Windows Debugger）属性，设置其值为内存转储的路径字符串。这甚至适用于在 x64 机器上调试 x86 程序。

## 其他调试功能

- **无条件断点**
- **监视窗口**
- **调用堆栈**
- **单步调试**

## 其他配置选项

你可以通过 `launch.json` 配置文件中的多个选项进一步配置调试设置：

- **额外符号**：如果有额外的目录可以找到符号文件，可以通过 `additionalSOLibSearchPath`（适用于 GDB 或 LLDB）或 `symbolSearchPath`（适用于 Visual Studio Windows Debugger）进行指定。

例如：

```json
"additionalSOLibSearchPath": "/path/to/symbols;/another/path/to/symbols"
```

- **定位源文件**：如果源文件不在编译位置，可以通过在 `sourceFileMap` 部分添加简单替换对来更改源文件位置。

例如：

```json
"sourceFileMap": {
    "/build/gcc-4.8-fNUjSI/gcc-4.8-4.8.4/build/i686-linux-gnu/libstdc++-v3/include/i686-linux-gnu": "/usr/include/i686-linux-gnu/c++/4.8",
    "/build/gcc-4.8-fNUjSI/gcc-4.8-4.8.4/build/i686-linux-gnu/libstdc++-v3/include": "/usr/include/c++/4.8"
}
```

## 已知限制

### 符号和代码导航

- **所有平台**：由于扩展不会解析函数体内的符号，因此“Peek Definition”和“Go to Definition”不适用于在函数体内部定义的符号。

### 调试

- **Windows**：

  - 在 Cygwin 和 MinGW 上，GDB 不能中断正在运行的进程。要在应用程序运行时设置断点，或暂停正在调试的应用程序，请在应用程序的终端中按 `Ctrl-C`。
  - GDB 在 Cygwin 上无法打开核心转储。

- **Linux**：

  - 你可能会看到错误信息：`ptrace: Operation not permitted`。这是因为 GDB 需要提升的权限才能附加到进程。使用以下命令可以暂时解决此错误：

    ```bash
    echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
    ```

- **macOS**：

  - 当使用 LLDB 调试时，如果在断点模式下关闭终端窗口，调试不会停止。调试可以通过按停止按钮来停止。
  - 使用 GDB 调试时，需要手动安装步骤。请查看 README 中的“Manual Installation of GDB for OS X”。

# Visual Studio Code C/C++ 调试配置文档

## 1. 配置 C/C++ 调试

在 Visual Studio Code 中，`launch.json` 文件用于配置调试器。

Visual Studio Code 会在你的项目中的 `.vscode` 文件夹下生成一个 `launch.json` 文件，其中包含几乎所有所需的信息。要开始调试，你需要在 `program` 字段中填写要调试的可执行文件的路径。无论是启动调试还是附加到正在运行的实例，都必须指定这个路径。

生成的文件包含两个部分，一部分配置启动调试，另一部分配置附加调试。

### 2. 配置 VS Code 的调试行为

设置或更改以下选项以控制 VS Code 在调试期间的行为：

- **program**（必填）: 指定调试器启动或附加的可执行文件的完整路径。调试器需要此位置来加载调试符号。
- **symbolSearchPath**: 告诉 Visual Studio Windows 调试器搜索符号（.pdb）文件的路径。多个路径用分号分隔。例如：`"C:\\Symbols;C:\\SymbolDir2"`。
- **requireExactSource**: 可选标志，指示 Visual Studio Windows 调试器是否要求当前源代码与 pdb 文件匹配。
- **additionalSOLibSearchPath**: 告诉 GDB 或 LLDB 搜索 .so 文件的路径。多个路径用分号分隔。例如：`"/Users/user/dir1;/Users/user/dir2"`。
- **externalConsole**: 仅在启动调试时使用。对于附加调试，此参数不会改变调试目标的行为。
  - Windows: 设置为 true 时，将启动外部控制台。设置为 false 时，将使用 VS Code 的 `integratedTerminal`。
  - Linux: 设置为 true 时，将通知 VS Code 启动外部控制台。设置为 false 时，将使用 VS Code 的 `integratedTerminal`。
  - macOS: 设置为 true 时，将通过 lldb-mi 启动外部控制台。设置为 false 时，输出可在 VS Code 的 `debugConsole` 中查看。
- **avoidWindowsConsoleRedirection**: 为了支持 VS Code 的集成终端与 gdb 在 Windows 上，扩展会向调试目标的参数添加控制台重定向命令以使控制台输入和输出显示在集成终端中。设置此选项为 true 将禁用该功能。
- **logging**: 可选标志，确定应该记录到调试控制台的消息类型。
  - `exceptions`: 可选标志，确定是否应该将异常消息记录到调试控制台。默认为 true。
  - `moduleLoad`: 可选标志，确定是否应该将模块加载事件记录到调试控制台。默认为 true。
  - `programOutput`: 可选标志，确定是否应该将程序输出记录到调试控制台。默认为 true。
  - `engineLogging`: 可选标志，确定是否应该将诊断引擎日志记录到调试控制台。默认为 false。
  - `trace`: 可选标志，确定是否应该将诊断适配器命令跟踪记录到调试控制台。默认为 false。
  - `traceResponse`: 可选标志，确定是否应该将诊断适配器命令和响应跟踪记录到调试控制台。默认为 false。
- **visualizerFile**: 用于调试的 .natvis 文件。有关如何创建 Natvis 文件的信息，请参见创建自定义原生对象视图。
- **showDisplayString**: 当指定了 `visualizerFile` 时，`showDisplayString` 将启用显示字符串。开启此选项可能会导致调试期间性能降低。

### 3. 示例配置

```json
{
  "name": "C++ Launch (Windows)",
  "type": "cppvsdbg",
  "request": "launch",
  "program": "C:\\app1\\Debug\\app1.exe",
  "symbolSearchPath": "C:\\Symbols;C:\\SymbolDir2",
  "externalConsole": true,
  "logging": {
    "moduleLoad": false,
    "trace": true
  },
  "visualizerFile": "${workspaceFolder}/my.natvis",
  "showDisplayString": true
}
```

### 4. 配置目标应用程序

以下选项可以在启动时修改目标应用程序的状态：

- **args**: JSON 数组，传递给程序的命令行参数。例如：`["arg1", "arg2"]`。如果转义字符，则需要双重转义。例如，`["{\\\"arg1\\\": true}"]` 将发送 `{"arg1": true}` 到你的应用程序。
- **cwd**: 设置调试器启动的应用程序的工作目录。
- **environment**: 要添加到程序环境中的环境变量。例如：`[ { "name": "config", "value": "Debug" } ]`，而不是 `[ { "config": "Debug" } ]`。

### 5. 自定义 GDB 或 LLDB

可以通过设置以下选项来更改 GDB 或 LLDB 的行为：

- **MIMode**: 指示 VS Code 连接的调试器。必须设置为 gdb 或 lldb。此项在每个操作系统上预配置，可以根据需要更改。
- **miDebuggerPath**: 调试器（如 gdb）的路径。当仅指定可执行文件时，它将搜索操作系统的 PATH 变量中的调试器（Linux 和 Windows 上的 GDB，OS X 上的 LLDB）。
- **miDebuggerArgs**: 传递给调试器的其他参数。
- **stopAtEntry**: 如果设置为 true，调试器应在目标的入口点停止（附加时被忽略）。默认值为 false。
- **stopAtConnect**: 如果设置为 true，调试器在连接到目标后应停止。如果设置为 false，调试器将在连接后继续。默认值为 false。
- **setupCommands**: JSON 数组，在 GDB 或 LLDB 中执行以进行设置的命令。例如：`"setupCommands": [ { "text": "target-run", "description": "run target", "ignoreFailures": false }]`。
- **customLaunchSetupCommands**: 如果提供，则用其他命令替换用于启动目标的默认命令。例如，可以在附加到目标进程时使用 `-target-attach`。一个空的命令列表将用空白替换启动命令，这在调试器通过命令行选项提供启动选项时很有用。例如：`"customLaunchSetupCommands": [ { "text": "target-run", "description": "run target", "ignoreFailures": false }]`。
- **launchCompleteCommand**: 调试器完全设置后执行的命令，以使目标进程运行。允许的值为 `"exec-run"`、`"exec-continue"`、`"None"`。默认值为 `"exec-run"`。

### 6. 示例配置

```json
{
  "name": "C++ Launch",
  "type": "cppdbg",
  "request": "launch",
  "program": "${workspaceFolder}/a.out",
  "stopAtEntry": false,
  "customLaunchSetupCommands": [
    { "text": "target-run", "description": "run target", "ignoreFailures": false }
  ],
  "launchCompleteCommand": "exec-run",
  "linux": {
    "MIMode": "gdb",
    "miDebuggerPath": "/usr/bin/gdb"
  },
  "osx": {
    "MIMode": "lldb"
  },
  "windows": {
    "MIMode": "gdb",
    "miDebuggerPath": "C:\\MinGw\\bin\\gdb.exe"
  }
}
```

### 7. 调试转储文件

C/C++ 扩展支持在 Windows 和 Linux、OS X 中调试转储文件和核心转储文件。

- **dumpPath**: 要调试的 Windows 转储文件的路径。
- **coreDumpPath**: 要调试的核心转储文件的完整路径。注意：MinGW 不支持核心转储调试。

### 8. 远程调试

- **miDebuggerServerAddress**: 连接到远程调试（例如 gdbserver）的调试器服务器的网络地址（例如，`localhost:1234`）。
- **debugServerPath**: 启动的调试服务器的完整路径。
- **debugServerArgs**: 调试服务器的参数。
- **serverStarted**: 在调试服务器输出中查找的服务器启动模式。支持正则表达式。
- **filterStdout**: 如果设置为 true，则在搜索 stdout 流时查找服务器启动模式并将 stdout 记录到调试输出。默认值为 true。
- **filterStderr**: 如果设置为 true，则在搜索 stderr 流时查找服务器启动模式并将 stderr 记录到调试输出。默认值为 false。
- **serverLaunchTimeout**: 调试器等待调试服务器启动的时间（毫秒）。默认值为 10000。

### 9. 附加属性

- **processId**: 默认为 `${command:pickProcess}`，这将显示调试器可以附加到的可用进程列表。建议保留此默认值，但可以显式设置为调试器附加的特定进程 ID。
- **request**: 指示配置部分是用于启动程序还是附加到已经运行的实例。
- **type**: 指示正在使用的底层调试器。使用 Visual Studio Windows 调试器时必须为 `cppvsdbg`，使用 GDB 或 LLDB 时必须为 `cppdbg`。创建 `launch.json` 文件时会自动设置此值。
- **sourceFileMap**: 允许将编译时路径映射到本地源位置。它是一个键值对对象，将解析第一个字符串匹配的路径。例如：`"sourceFileMap": { "/mnt/c": "c:\\" }` 将映射调试器返回的任何以 `/mnt/c` 开头的路径并转换为 `c:\\`。对象中可以有多个映射，但将按提供的顺序处理。

### 10. 环境变量定义文件

环境变量定义文件是一个简单的文本文件，包含以 `环境变量=值` 形式的键值对，并使用 `#` 作为注释。多行值不受支持。

cppvsdbg 调试器配置还包含一个 `envFile` 属性，允许你轻松设置调试目的的变量。

#### 示例：

`project.env` 文件：

```plaintext
# project.env

# Example environment with key as 'MYENVRIONMENTPATH' and value as C:\\Users\\USERNAME\\Project
MYENVRIONMENTPATH=C:\\Users\\USERNAME\\Project

# Variables with spaces
SPACED_OUT_PATH="C:\\This Has Spaces\\Project"
```

### 11. 符号选项

`symbolOptions` 元素允许自定义调试器搜索符号的方式。

#### 示例：

```json
"symbolOptions": {
    "searchPaths": [
        "C:\\src\\MyOtherProject\\bin\\debug",
        "https://my-companies-symbols-server"
    ],
    "searchMicrosoftSymbolServer": true,
    "cachePath": "%TEMP%\\symcache",
    "moduleFilter": {
        "mode": "loadAllButExcluded",
        "excludedModules": [ "DoNotLookForThisOne*.dll" ]
    }
}
```

### 12. 属性说明

- **searchPaths**: 符号服务器 URL（例如：`https://msdl.microsoft.com/download/symbols`）或目录（例如：`/build/symbols`）的数组，搜索 .pdb 文件。这些目录将在默认位置（模块旁边和 pdb 最初放置的路径）之外进行搜索。
- **searchMicrosoftSymbolServer**: 如果为 true，将 Microsoft 符号服务器（`https://msdl.microsoft.com/download/symbols`）添加到符号搜索路径。如果未指定，默认此选项为 false。
- **cachePath**: 下载自符号服务器的符号应缓存的目录。如果未指定，调试器将默认使用 `%TEMP%\\SymbolCache`。
- **moduleFilter.mode**: 此值可以是 `"loadAllButExcluded"` 或 `"loadOnlyIncluded"`。在 `"loadAllButExcluded"` 模式下，调试器加载所有模块的符号，除非该模块在 `excludedModules` 数组中。 在 `"loadOnlyIncluded"` 模式下，调试器不会尝试为任何模块加载符号，除非它在 `includedModules` 数组中，或者通过 `includeSymbolsNextToModules` 设置包含。

### 13. 结束语

通过调整这些配置，您可以有效地使用 Visual Studio Code 进行 C/C++ 开发和调试。根据具体项目的需求，灵活配置 `launch.json` 文件将有助于提高工作效率和调试体验。

## 下一步

继续了解以下内容：

- [Configure VS Code for Windows Subsystem for Linux](https://code.visualstudio.com/docs/cpp/config-wsl)
- [Configure VS Code for Mingw-w64 and GCC](https://code.visualstudio.com/docs/cpp/config-mingw)
- [Configure VS Code for macOS](https://code.visualstudio.com/docs/cpp/config-clang-mac)
- [Configure C/C++ debugging](https://code.visualstudio.com/docs/cpp/launch-json-reference) - Learn about additional debugger configuration options.
- [Basic Editing](https://code.visualstudio.com/docs/editor/codebasics) - Learn about the powerful Visual Studio Code editor.
- [Code Navigation](https://code.visualstudio.com/docs/editor/editingevolved) - Move quickly through your source code.
- [Tasks](https://code.visualstudio.com/docs/editor/tasks) - use tasks to build your project and more.
- [Debugging](https://code.visualstudio.com/docs/editor/debugging) - find out about the Visual Studio Code debugger.

如果你有任何其他问题或遇到任何问题，请在 GitHub 上提交问题。