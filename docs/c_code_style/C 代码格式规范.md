# C语言风格和编码规则

------

## 版本更新记录

| 版本  | 日期       | 修改说明     |
| :---- | :--------- | :----------- |
| 1.0.0 | 2025-02-19 | 初始版本发布 |

## 目录

- [C语言风格和编码规则](#c语言风格和编码规则)
  - [版本更新记录](#版本更新记录)
  - [目录](#目录)
  - [核心规范](#核心规范)
    - [首要原则](#首要原则)
    - [基础规范](#基础规范)
    - [代码格式化](#代码格式化)
  - [编码细则](#编码细则)
    - [注释规范](#注释规范)
    - [函数设计](#函数设计)
    - [变量管理](#变量管理)
    - [复合类型](#复合类型)
    - [控制结构](#控制结构)
    - [预处理器](#预处理器)
  - [工程实际](#工程实际)
    - [文档标准](#文档标准)
    - [文件组织](#文件组织)
    - [工具集成](#工具集成)

------

## 核心规范

### 首要原则

> **黄金法则**：保持与现有代码风格一致，即使与本文规范冲突。维护代码风格一致性高于个人偏好。

本文档中的关键字 必须 、**MUST NOT** 、**REQUIRED** 、**SHALL** 、**SHALL NOT** 、**SHOULD** 、**SHOULD NOT** 、**RECOMMENDED** 、**NOT RECOMMENDED** 、**MAY** 和 **OPTIONAL**应按照`BCP 14`  `[RFC2119]` `[RFC8174]`中的描述进行解释。

### 基础规范

以下列出了最明显和最重要的一般规则。在继续阅读其他章节之前，请仔细检查这些规则。

- `clang-format` 附加到此存储库的格式化文件一起使用

- 使用`C11`标准

- 不使用 Tab，而是使用空格。

- 每级缩进使用 4 个空格。

- 关键字和开括号之间使用1个空格

  ```c
  /* 正确 */
  if (condition)
  while (condition)
  for (init; condition; step)
  do {} while (condition)

  /* 错误 */
  if(condition)
  while(condition)
  for(init;condition;step)
  do {} while(condition)
  ```

- 函数名和开括号之间不要使用空格

  ```c
  int32_t a = sum(4, 3);              /* OK */
  int32_t a = sum (4, 3);             /* Wrong */
  ```

- 永远不要使用`__`或`_`前缀用于变量/函数/宏/类型。这是C语言本身保留的
  - 对于严格模块私有（静态）函数，首选`prv_`名称前缀
  - 对于库内部函数，应该首选`libname_int_`或`libnamei_`前缀，这些函数不应该被用户应用程序使用，但必须在不同库内部模块间使用

- 变量、函数、类型名称仅使用小写字母，可选下划线 `_` 分隔单词。

- 左大括号始终与关键字在同一行（for、while、do、switch、if等）

  ```c
  size_t i;
  for (i = 0; i < 5; ++i) {           /* OK */
  }
  for (i = 0; i < 5; ++i){            /* Wrong */
  }
  for (i = 0; i < 5; ++i)             /* Wrong */
  {
  }
  ```

- 比较运算符和赋值运算符前后使用单个空格

  ```c
  int32_t a;
  a = 3 + 4;              /* OK */
  for (a = 0; a < 5; ++a) /* OK */
  a=3+4;                  /* Wrong */
  a = 3+4;                /* Wrong */
  for (a=0;a<5;++a)       /* Wrong */
  ```

- 每个逗号后使用一个空格

  ```c
  func_name(5, 4);        /* OK */
  func_name(4,3);         /* Wrong */
  ```

- 不要将`global`变量初始化为任何默认值（或`NULL`），在专用`init`函数中实现它（如果需要）。

  ```c
  static int32_t a;       /* Wrong */
  static int32_t b = 4;   /* Wrong */
  static int32_t a = 0;   /* Wrong */
  ```

  > 在嵌入式系统中，RAM 内存分散在系统的不同内存位置是很常见的。处理所有情况很快就会变得很棘手，尤其是当用户声明自定义 RAM 部分时。启动脚本负责设置默认值（`.data` 和`.bss`），而其他自定义部分可能没有填充默认值，这导致具有`init` 值的变量不会产生任何效果。
  >
  > 为了避免这样的问题，为每个模块创建 `init`函数并使用它来设置所有变量的默认值，如下所示：
  >
  > ```c
  > static int32_t a;       /* OK */
  > static int32_t b = 4;   /* Wrong - this value may not be set at zero if linker script&startup files are not properly handled */
  >
  > void my_module_init(void) {
  >     a = 0;
  >     b = 4;
  > }
  > ```

- 在同一行中声明同一类型的所有局部变量

  ```c
  void my_func(void) {
      /* 1 */
      char a;             /* OK */

      /* 2 */
      char a, b;          /* OK */

      /* 3 */
      char a;
      char b;             /* Wrong, variable with char type already exists */
  }
  ```

- 按顺序声明局部变量

  1. 自定义结构和枚举
  2. 整数类型，首先是更宽的无符号类型
  3. 单/双浮点

  ```c
  int my_func(void) {
      /* 1 */
      my_struct_t my;     /* First custom structures */
      my_struct_ptr_t* p; /* Pointers too */

      /* 2 */
      uint32_t a;
      int32_t b;
      uint16_t c;
      int16_t g;
      char h;
      /* ... */

      /* 3 */
      double d;
      float f;
  }
  ```

- 始终在块开头、第一个可执行语句之前声明局部变量

- 始终在结构（或其子元素）初始化的最后一个元素中添加尾随逗号（这有助于 clang-format 正确格式化结构）。除非结构非常简单且简短

  ```c
  typedef struct {
      int a, b;
  } str_t;

  str_t s = {
      .a = 1,
      .b = 2,   /* Comma here */
  }

  /* Examples of "complex" structure, with or with missing several trailing commas, after clang-format runs the formatting */
  static const my_struct_t my_var_1 = {
      .type = TYPE1,
      .type_data =
          {
              .type1 =
                  {
                      .par1 = 0,
                      .par2 = 1, /* Trailing comma here */
                  }, /* Trailing comma here */
          },  /* Trailing comma here */
  };

  static const my_struct_t my_var_2 = {.type = TYPE2,
                                       .type_data = {
                                           .type2 =
                                               {
                                                   .par1 = 0,
                                                   .par2 = 1,
                                               },
                                       }};    /* Missing comma here */
  static const my_struct_t my_var_3 = {.type = TYPE3,
                                       .type_data = {.type3 = {
                                                         .par1 = 0,
                                                         .par2 = 1,
                                                     }}}; /* Missing 2 commas here */

  /* No trailing commas - good only for small and simple structures */
  static const my_struct_t my_var_4 = {.type = TYPE4, .type_data = {.type4 = {.par1 = 0, .par2 = 1}}};
  ```

- `for`在循环中声明计数器变量

  ```c
  /* OK */
  for (size_t i = 0; i < 10; ++i)

  /* OK, if you need counter variable later */
  size_t i;
  for (i = 0; i < 10; ++i) {
      if (...) {
          break;
      }
  }
  if (i == 10) {

  }

  /* Wrong */
  size_t i;
  for (i = 0; i < 10; ++i) ...
  ```

- 避免在声明中使用函数调用来赋值变量，单个变量除外

  ```c
  void a(void) {
      /* Avoid function calls when declaring variable */
      int32_t a, b = sum(1, 2);

      /* Use this */
      int32_t a, b;
      b = sum(1, 2);

      /* This is ok */
      uint8_t a = 3, b = 4;
  }
  ```

- 除了`char`、`float`或 之外`double`，始终使用`stdint.h`库中声明的类型，例如`uint8_t` for`unsigned 8-bit`等。

- 不要使用库`stdbool.h`。分别使用`1`或`0` 表示 `true`  或  `false`

  - 老旧编译器支持 ：一些非常老旧的C编译器（尤其是嵌入式系统中）可能不支持 `C99` 标准或更高版本，而 `stdbool.h` 是 `C99` 引入的标准库头文件。在这种情况下，直接使用 `1` 和 `0` 可以避免依赖 `stdbool.h`，从而提高代码的兼容性。
  - 跨平台开发 ：在某些嵌入式系统或资源受限的环境中，可能需要完全避免标准库的使用。此时，手动定义布尔值为 `1` 和 `0` 可以减少对外部库的依赖。
  - 内存占用更少 ：虽然 `stdbool.h` 中的 `bool` 类型通常被实现为 `_Bool`（通常是 1 字节），但某些嵌入式编译器可能会将其扩展为更大的类型（如 4 字节）。如果直接使用 `1` 和 `0`，可以确保布尔值只占用所需的最小空间（例如通过 `uint8_t` 或其他自定义类型）。
  - 减少指令开销 ：在某些架构上，使用 `1` 和 `0` 直接操作整数可能比操作 `bool` 类型更快，因为整数运算通常是硬件原生支持的。
  - 无需额外头文件 ：通过避免引入 `stdbool.h`，可以减少头文件的数量，简化项目结构，尤其是在小型项目或嵌入式开发中。
  - 显式语义 ：对于熟悉C语言的开发者来说，`1` 和 `0` 的含义非常直观，不需要额外学习 `stdbool.h` 的用法。

  ```c
  /* OK */
  uint8_t status;
  status = 0;

  /* Wrong */
  #include <stdbool.h>
  bool status = true;
  ```

- 永远不要将变量与`true`进行比较，例如

  ```c
  /* Wrong */
  if (check_func() == 1) {
      // ...
  }

  /* OK */
  if (check_func()) {
      // ...
  }
  ```

- 始终将指针与NULL值进行比较

  ```c
  void* ptr;

  /* ... */

  /* OK, compare against NULL */
  if (ptr == NULL || ptr != NULL) {

  }

  /* Wrong */
  if (ptr || !ptr) {

  }
  ```

- 使用前置递增（和递减），而不是后置递增（和递减）

  ```c
  int32_t a = 0;
  ...

  a++;            /* Wrong */
  ++a;            /* OK */

  for (size_t j = 0; j < 10; ++j) {}  /* OK */
  ```

- 始终对长度或大小变量使用`size_t`

- 如果函数不应修改指针指向的内存，始终对指针使用`const`

- 如果函数参数或变量不应被修改，始终使用`const`

  ```c

  /* When d could be modified, data pointed to by d could not be modified */
  void my_func(const void* d) {

  }

  /* When d and data pointed to by d both could not be modified */
  void my_func(const void* const d) {

  }

  /* Not REQUIRED, it is advised */
  void my_func(const size_t len) {

  }

  /* When d should not be modified inside function, only data pointed to by d could be modified */
  void my_func(void* const d) {

  }
  ```

- 当函数可能接受任何类型的指针时，始终使用`void *`，不要使用`uint8_t *`

  - 函数必须在实现中处理适当的类型转换

  ```c
  /*
   * To send data, function should not modify memory pointed to by `data` variable
   * thus `const` keyword is important
   *
   * To send generic data (or to write them to file)
   * any type may be passed for data,
   * thus use `void *`
   */
  /* OK example */
  void send_data(const void* data, size_t len) { /* OK */
      /* Do not cast `void *` or `const void *` */
      const uint8_t* d = data;/* Function handles proper type for internal usage */
  }

  void send_data(const void* data, int len) {    /* Wrong, not not use int */
  }
  ```

- 始终使用带`sizeof`运算符的括号

- 永远不要使用变长数组（VLA）。改用标准C `malloc`和`free`函数进行动态内存分配，或者如果库/项目提供了自定义内存分配，则使用其实现

  - 看看[LwMEM](https://github.com/MaJerle/lwmem)，自定义内存管理库

  ```c
  /* OK */
  #include <stdlib.h>

  void my_func(size_t size) {
      int32_t* arr;
      arr = malloc(sizeof(*arr) * n); /* OK, Allocate memory */
      arr = malloc(sizeof *arr * n);  /* Wrong, brackets for sizeof operator are missing */
      if (arr == NULL) {
          /* FAIL, no memory */
      }

      free(arr);  /* Free memory after usage */
  }

  /* Wrong */
  void my_func(size_t size) {
      int32_t arr[size];  /* Wrong, do not use VLA */
  }
  ```

- 始终将变量与零进行比较，除非将其视为`boolean`类型

- 永远不要将被视为`boolean`类型的变量与零或一进行比较。改用NOT (!)

  ```c
  size_t length = 5;  /* Counter variable */
  uint8_t is_ok = 0;  /* Boolean-treated variable */
  if (length)         /* Wrong, length is not treated as boolean */
  if (length > 0)     /* OK, length is treated as counter variable containing multi values, not only 0 or 1 */
  if (length == 0)    /* OK, length is treated as counter variable containing multi values, not only 0 or 1 */

  if (is_ok)          /* OK, variable is treated as boolean */
  if (!is_ok)         /* OK, -||- */
  if (is_ok == 1)     /* Wrong, never compare boolean variable against 1! */
  if (is_ok == 0)     /* Wrong, use ! for negative check */
  ```

- 始终使用`/* comment */`进行注释，即使是单行注释也一样

- 始终在头文件中包含extern关键字的C++检查

- 每个函数都必须包含`doxygen`启用的注释，即使是静态函数也一样

- 使用英文名称/文本命名函数、变量、注释

- 使用小写字母表示变量

- 如果变量包含多个名称，请使用下划线_字符，例如`force_redraw`。不要使用`forceRedraw`

- 永远不要转换返回`void *`的函数, 因为`void *`可以安全地提升到任何其他指针类型

  ```c
  uint8_t* ptr = (uint8_t *)func_returning_void_ptr();  /* Wrong */

  uint8_t* ptr = func_returning_void_ptr();             /* OK */
  ```

- 对于C标准库包含文件，始终使用<和>，例如`#include <stdlib.h>`

- 对于自定义库，始终使用`""`用于自定义库，例如。`#include "my_library.h"`

- 当转换为指针类型时，总是将星号与类型对齐

  ```c
  uint8_t* t = (uint8_t*)var_width_diff_type;
  ```

- 始终尊重项目或库中已有的代码风格

### 代码格式化

> 将源文件编码统一为 `UTF-8`
>
> 将 TAB 键替换为 4 空格
>
> 将每行末尾多余的空格删除，并统一换行符为 '\n'

该项目包含 `.clang-format` 和 `clang-format-config.json` 文件，可以使用 `clang-format` 工具统一代码风格。在 `.vscode` 目录中还设置了代码格式的自动化管理。

格式化命令：

```bash
python3 .\scripts\run-clang-format-from-config.py
```

## 编码细则

### 注释规范

- 不允许使用`//`开头的注释。始终使用`/* comment */`，即使是单行注释也一样

  ```c
  //This is comment (wrong)
  /* This is comment (ok) */
  ```

- 对于多行注释，每行使用空格加星号

  ```c
  /*
   * This is multi-line comments,
   * written in 2 lines (ok)
   */

  /**
   * Wrong, use double-asterisk only for doxygen documentation
   */

  /*
   * Single line comment in multi-line configuration (wrong)
   */

  /* Single line comment (ok) */
  ```
  
- 每个文件必须包括`doxygen`注解以及简要描述，后跟空行（使用`doxygen`时）

  ```c
  /**
   * \file            template.h
   * \brief           Template include file
   */
                      /* Here is empty line */
  ```

- 每个文件（头文件或源文件）必须包括许可（开头注释包含单个星号，因为这必须被doxygen忽略）

- 使用项目/库已使用的相同许可

  ```c
  /**
   * \file            template.h
   * \brief           Template include file
   */
  
  /*
   * Copyright (c) year FirstName LASTNAME
   *
   * Permission is hereby granted, free of charge, to any person
   * obtaining a copy of this software and associated documentation
   * files (the "Software"), to deal in the Software without restriction,
   * including without limitation the rights to use, copy, modify, merge,
   * publish, distribute, sublicense, and/or sell copies of the Software,
   * and to permit persons to whom the Software is furnished to do so,
   * subject to the following conditions:
   *
   * The above copyright notice and this permission notice shall be
   * included in all copies or substantial portions of the Software.
   *
   * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
   * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
   * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   * OTHER DEALINGS IN THE SOFTWARE.
   *
   * This file is part of library_name.
   *
   * Author:          xxx <xxx@xxx.com>
   * Version:         _version_
   */
  ```


### 函数设计

> 函数应该尽量精简，仅完成相对独立的简单功能。函数的实现不应该太长，函数实现太长，应该反思能够如何修改(或拆分)使得函数更为精简、易懂。

- 每个可能从其模块外部访问的函数，必须包括函数原型（或声明）

- 函数名必须为小写，可选地用下划线`_`字符分隔

  ```c
  /* OK */
  void my_func(void);
  void myfunc(void);

  /* Wrong */
  void MYFunc(void);
  void myFunc();
  ```

- 当函数返回指针时，将星号对齐到返回类型

  ```c
  /* OK */
  const char* my_func(void);
  my_struct_t* my_func(int32_t a, int32_t b);

  /* Wrong */
  const char *my_func(void);
  my_struct_t * my_func(void);
  ```

- 推荐不需要对齐所有功能类似的函数原型

  ```c
  /*  OK, function names not aligned */
  void set(int32_t a);
  const char * get(void);

  /* Not recommended */
  void        set(int32_t a);
  my_type_t   get(void);
  my_ptr_t*   get_ptr(void);
  ```

- 函数实现推荐在同一行中包含返回类型和其他可选关键字

  ```c
  /* OK */
  int32_t foo(void) {
      return 0;
  }

  /* OK */
  static const char* get_string(void) {
      return "Hello world!\r\n";
  }

  /* Not recommended */
  int32_t
  foo(void) {
      return 0;
  }
  ```

### 变量管理

- 使变量名全小写，并可选地使用下划线`_`字符

  ```c
  /* OK */
  int32_t a;
  int32_t my_var;
  int32_t myvar;

  /* Wrong */
  int32_t A;
  int32_t myVar;
  int32_t MYVar;
  ```

- 按类型将局部变量分组在一起

  ```c
  void
  foo(void) {
      int32_t a, b;   /* OK */
      char a;
      char b;         /* Wrong, char type already exists */
  }
  ```

- 不要在第一个可执行语句之后声明变量

  ```c
  void
  foo(void) {
      int32_t a;
      a = bar();
      int32_t b;      /* Wrong, there is already executable statement */
  }
  ```

- 你可以在下一个缩进级别内声明新变量

  ```c
  int32_t a, b;
  a = foo();
  if (a) {
      int32_t c, d;   /* OK, c and d are in if-statement scope */
      c = foo();
      int32_t e;      /* Wrong, there was already executable statement inside block */
  }
  ```

- 声明指针变量时，将星号对齐到类型

- 当声明多个指针变量时，你可以将星号对齐到变量名

  ```c
  /* OK */
  char *p, *n;
  ```

### 复合类型

- 结构体或枚举名称必须为小写，并可选地在单词之间使用下划线`_`字符
- 结构体或枚举可以包含`typedef`关键字
- 所有结构体成员必须为小写
- 所有枚举成员必须为大写

当声明结构体时，可以使用以下三种选项之一：

- 当结构体仅用名称声明时，其名称后面不需包含`_t`后缀。

  ```c
  struct struct_name {
      char* a;
      char b;
  };
  ```

- 当结构体仅用`typedef`声明时，其名称后面必须包含`_t`后缀。

  ```c
  typedef struct {
      char* a;
      char b;
  } struct_name_t;
  ```

- 当结构体同时用名称和`typedef`声明时，基本名称不得包含`_t`，而`typedef`部分的名称后面必须包含`_t`后缀。

  ```c
  typedef struct struct_name {    /* No _t */
      char* a;
      char b;
      char c;
  } struct_name_t;    /* _t */
  ```

不良声明示例及其建议更正

```c
/* a and b MUST be separated to 2 lines */
/* Name of structure with typedef MUST include _t suffix */
typedef struct {
    int32_t a, b;
} a;

/* Corrected version */
typedef struct {
    int32_t a;
    int32_t b;
} a_t;

/* Wrong name, it MUST not include _t suffix */
struct name_t {
    int32_t a;
    int32_t b;
};

/* Wrong parameters, MUST be all uppercase */
typedef enum {
    MY_ENUM_TESTA,
    my_enum_testb,
} my_enum_t;
```

- 在声明时初始化结构体时，使用`C99`初始化风格

  ```c
  /* OK */
  a_t a = {
      .a = 4,
      .b = 5,
  };

  /* Wrong */
  a_t a = {1, 2};
  ```

- 当引入新的`typedef`用于函数句柄时，使用`_fn`后缀

  ```c
  /* Function accepts 2 parameters and returns uint8_t */
  /* Name of typedef has `_fn` suffix */
  typedef uint8_t (*my_func_typedef_fn)(uint8_t p1, const char* p2);
  ```

### 控制结构

- 每个复合语句必须包含开括号和闭括号，即使它只包含一个嵌套语句

- 每个复合语句必须包含单个缩进；当嵌套语句时，每次嵌套增加一个缩进大小

  ```c
  /* OK */
  if (c) {
      do_a();
  } else {
      do_b();
  }

  /* Wrong */
  if (c)
      do_a();
  else
      do_b();

  /* Wrong */
  if (c) do_a();
  else do_b();
  ```

- 在`if`或`if-else-if`语句的情况下，`else`必须与第一个语句的闭括号在同一行

  ```c
  /* OK */
  if (a) {

  } else if (b) {

  } else {

  }

  /* Wrong */
  if (a) {

  }
  else {

  }

  /* Wrong */
  if (a) {

  }
  else
  {

  }
  ```

- 在`do-while`语句的情况下，`while`部分必须与`do`部分的闭括号在同一行

  ```c
  /* OK */
  do {
      int32_t a;
      a = do_a();
      do_b(a);
  } while (check());

  /* Wrong */
  do
  {
  /* ... */
  } while (check());

  /* Wrong */
  do {
  /* ... */
  }
  while (check());
  ```

- 复合语句必须包含大括号，即使在单个语句的情况下。下面的例子显示了不良做法

  ```c
  if (a) do_b();
  else do_c();

  if (a) do_a(); else do_b();
  ```

- 空的`while`、`do-while`或`for`循环必须包含大括号

  ```c
  if (a) do_b();
  else do_c();

  if (a) do_a(); else do_b();
  ```

- 如果`while`（或`for`、`do-while`等）为空（这在嵌入式编程中可能是这种情况），则使用空的单行大括号

  ```c
  /* Wait for bit to be set in embedded hardware unit */
  volatile uint32_t* addr = HW_PERIPH_REGISTER_ADDR;

  /* Wait bit 13 to be ready */
  while (*addr & (1 << 13)) {}        /* OK, empty loop contains no spaces inside curly brackets */
  while (*addr & (1 << 13)) { }       /* Wrong */
  while (*addr & (1 << 13)) {         /* Wrong */

  }
  while (*addr & (1 << 13));          /* Wrong, curly brackets are missing. Can lead to compiler warnings or unintentional bugs */
  ```

- 始终优先使用这种顺序的循环：`for`、`do-while`、`while`

- 尽可能避免在循环块内增加变量，请参见示例

  ```c
  /* Not recommended */
  int32_t a = 0;
  while (a < 10) {
      .
      ..
      ...
      ++a;
  }

  /* Better */
  for (size_t a = 0; a < 10; ++a) {

  }

  /* Better, if inc may not happen in every cycle */
  for (size_t a = 0; a < 10; ) {
      if (...) {
          ++a;
      }
  }
  ```

- 内联`if`语句只能用于赋值或函数调用操作

  ```c
  /* OK */
  int a = condition ? if_yes : if_no; /* Assignment */
  func_call(condition ? if_yes : if_no); /* Function call */
  switch (condition ? if_yes : if_no) {...}   /* OK */

  /* Wrong, this code is not well maintenable */
  condition ? call_to_function_a() : call_to_function_b();

  /* Rework to have better program flow */
  if (condition) {
      call_to_function_a();
  } else {
      call_to_function_b();
  }
  ```

- 每个case语句添加单个缩进

- 在每个case或default语句中，break语句使用额外的单个缩进

  ```c
  /* OK, every case has single indent */
  /* OK, every break has additional indent */
  switch (check()) {
      case 0:
          do_a();
          break;
      case 1:
          do_b();
          break;
      default:
          break;
  }

  /* Wrong, case indent missing */
  switch (check()) {
  case 0:
      do_a();
      break;
  case 1:
      do_b();
      break;
  default:
      break;
  }

  /* Wrong */
  switch (check()) {
      case 0:
          do_a();
      break;      /* Wrong, break MUST have indent as it is under case */
      case 1:
      do_b();     /* Wrong, indent under case is missing */
      break;
      default:
          break;
  }
  ```

- 始终包含default语句

- 如果需要局部变量，使用大括号并将break语句放在内部。

  - 将开括号放在与case语句同一行

  ```c
  switch (a) {
      /* OK */
      case 0: {
          int32_t a, b;
          char c;
          a = 5;
          /* ... */
          break;
      }

      /* Wrong */
      case 1:
      {
          int32_t a;
          break;
      }

      /* Wrong, break shall be inside */
      case 2: {
          int32_t a;
      }
      break;
  }
  ```

------

### 预处理器

- 始终使用宏代替文字常量，尤其是数字

- 所有宏必须全大写，可选地使用下划线`_`字符，除非它们明确标记为将来可能用常规函数语法替换的函数

  ```c
  /* OK */
  #define SQUARE(x)         ((x) * (x))

  /* Wrong */
  #define square(x)           ((x) * (x))
  ```

- 始终用括号保护输入参数

  ```c
  /* OK */
  #define MIN(x, y)           ((x) < (y) ? (x) : (y))

  /* Wrong */
  #define MIN(x, y)           x < y ? x : y
  ```

- 始终用括号保护最终的宏评估

  ```c
  /* Wrong */
  #define MIN(x, y)           (x) < (y) ? (x) : (y)
  #define SUM(x, y)           (x) + (y)

  /* Imagine result of this equation using wrong SUM implementation */
  int32_t x = 5 * SUM(3, 4);  /* Expected result is 5 * 7 = 35 */
  int32_t x = 5 * (3) + (4);  /* It is evaluated to this, final result = 19 which is not what we expect */

  /* Correct implementation */
  #define MIN(x, y)           ((x) < (y) ? (x) : (y))
  #define SUM(x, y)           ((x) + (y))
  ```

- 当宏使用多个语句时，使用`do {} while (0)`语句保护它们

  ```c
  typedef struct {
      int32_t px, py;
  } point_t;
  point_t p;                  /* Define new point */

  /* Wrong implementation */

  /* Define macro to set point */
  #define SET_POINT(p, x, y)  (p)->px = (x); (p)->py = (y)    /* 2 statements. Last one should not implement semicolon */

  SET_POINT(&p, 3, 4);        /* Set point to position 3, 4. This evaluates to... */
  (&p)->px = (3); (&p)->py = (4); /* ... to this. In this example this is not a problem. */

  /* Consider this ugly code, however it is valid by C standard (not recommended) */
  if (a)                      /* If a is true */
      if (b)                  /* If b is true */
          SET_POINT(&p, 3, 4);/* Set point to x = 3, y = 4 */
      else
          SET_POINT(&p, 5, 6);/* Set point to x = 5, y = 6 */

  /* Evaluates to code below. Do you see the problem? */
  if (a)
      if (b)
          (&p)->px = (3); (&p)->py = (4);
      else
          (&p)->px = (5); (&p)->py = (6);

  /* Or if we rewrite it a little */
  if (a)
      if (b)
          (&p)->px = (3);
          (&p)->py = (4);
      else
          (&p)->px = (5);
          (&p)->py = (6);

  /*
   * Ask yourself a question: To which `if` statement does the `else` keyword belong?
   *
   * Based on first part of code, answer is straight-forward. To inner `if` statement when we check `b` condition
   * Actual answer: Compilation error as `else` belongs nowhere
   */

  /* Better and correct implementation of macro */
  #define SET_POINT(p, x, y)  do { (p)->px = (x); (p)->py = (y); } while (0)    /* 2 statements. No semicolon after while loop */
  /* Or even better */
  #define SET_POINT(p, x, y)  do {    \   /* Backslash indicates statement continues in new line */
      (p)->px = (x);                  \
      (p)->py = (y);                  \
  } while (0)                             /* 2 statements. No semicolon after while loop */

  /* Now original code evaluates to */
  if (a)
      if (b)
          do { (&p)->px = (3); (&p)->py = (4); } while (0);
      else
          do { (&p)->px = (5); (&p)->py = (6); } while (0);

  /* Every part of `if` or `else` contains only `1` inner statement (do-while), hence this is valid evaluation */

  /* To make code perfect, use brackets for every if-ifelse-else statements */
  if (a) {                    /* If a is true */
      if (b) {                /* If b is true */
          SET_POINT(&p, 3, 4);/* Set point to x = 3, y = 4 */
      } else {
          SET_POINT(&p, 5, 6);/* Set point to x = 5, y = 6 */
      }
  }
  ```

- 避免使用`#ifdef`或`#ifndef`。改用`defined()`或`!defined()`

  ```c
  #ifdef XYZ
  /* do something */
  #endif /* XYZ */
  ```

- 始终记录`if/elif/else/endif`语句

  ```c
  /* OK */
  #if defined(XYZ)
  /* Do if XYZ defined */
  #else /* defined(XYZ) */
  /* Do if XYZ not defined */
  #endif /* !defined(XYZ) */

  /* Wrong */
  #if defined(XYZ)
  /* Do if XYZ defined */
  #else
  /* Do if XYZ not defined */
  #endif
  ```

- *不要缩进#if语句内的子语句*

  ```c
  /* OK */
  #if defined(XYZ)
  #if defined(ABC)
  /* do when ABC defined */
  #endif /* defined(ABC) */
  #else /* defined(XYZ) */
  /* Do when XYZ not defined */
  #endif /* !defined(XYZ) */

  /* Wrong */
  #if defined(XYZ)
      #if defined(ABC)
          /* do when ABC defined */
      #endif /* defined(ABC) */
  #else /* defined(XYZ) */
      /* Do when XYZ not defined */
  #endif /* !defined(XYZ) */
  ```

## 工程实际

### 文档标准

#### 版本控制

- 使用 `Git` 进行版本管理。

#### 工作流

- **主分支 (`main` 或 `master`)** : 用于存放稳定的、可发布的代码。
- **开发分支 (`develop`)** : 用于日常开发，所有新功能的集成都在此分支上完成。
- **功能分支 (`feature/*`)** : 每个新功能从 `develop` 分支创建，完成后合并回 `develop`。
- **发布分支 (`release/*`)** : 当 `develop` 分支准备发布时，创建一个 `release` 分支，用于修复小问题并准备发布。
- **热修复分支 (`hotfix/*`)** : 用于紧急修复生产环境中的问题，直接从 `main` 分支创建，完成后合并到 `main` 和 `develop`。

#### 语义化版本

> https://semver.org/lang/zh-CN/

- 版本格式：主版本号.次版本号.修订号，版本号递增规则如下：
  - 主版本号：当你做了不兼容的 API 修改，
  - 次版本号：当你做了向下兼容的功能性新增，
  - 修订号：当你做了向下兼容的问题修正。

先行版本号及版本编译信息可以加到“主版本号.次版本号.修订号”的后面，作为延伸。

#### 约定式提交

> [Conventional Commits](https://www.conventionalcommits.org/) 
>
> [VSCode Conventional Commits](https://marketplace.visualstudio.com/items?itemName=vivaxy.vscode-conventional-commits) 

约定式提交规范是一种基于提交信息的轻量级约定。 它提供了一组简单规则来创建清晰的提交历史； 这更有利于编写自动化工具。 通过在提交信息中描述功能、修复和破坏性变更， 使这种惯例与 [SemVer](http://semver.org/lang/zh-CN) 相互对应。

- 遵循 [约定式提交](https://www.conventionalcommits.org/zh-hans/) 规范。请确保你的提交信息符合以下格式：

  ```c
  <type>[optional scope]: <description>
  
  [optional body]
  
  [optional footer(s)]
  ```

- 提交说明包含了下面的结构化元素，以向类库使用者表明其意图：

  1. **fix:** *类型* 为 `fix` 的提交表示在代码库中修复了一个 bug（这和语义化版本中的 [`PATCH`](https://semver.org/lang/zh-CN/#摘要) 相对应）。
  2. **feat:** *类型* 为 `feat` 的提交表示在代码库中新增了一个功能（这和语义化版本中的 [`MINOR`](https://semver.org/lang/zh-CN/#摘要) 相对应）。
  3. **BREAKING CHANGE:** 在脚注中包含 `BREAKING CHANGE:` 或 <类型>(范围) 后面有一个 `!` 的提交，表示引入了破坏性 API 变更（这和语义化版本中的 [`MAJOR`](https://semver.org/lang/zh-CN/#摘要) 相对应）。 破坏性变更可以是任意 *类型* 提交的一部分。
  4. 除`fix:`和`feat:`之外，也可以使用其它提交类型，例如@commitlint/config-conventional（基于Angular 约定）中推荐的`build:`、`chore:`、`ci:`、`docs:`、`style:`、`refactor:`、`perf:`、`test:`，等等。
     - build: 用于修改项目构建系统，例如修改依赖库、外部接口或者升级 Node 版本等；
     - chore: 用于对非业务性代码进行修改，例如修改构建流程或者工具配置等；
     - ci: 用于修改持续集成流程，例如修改 Travis、Jenkins 等工作流配置；
     - docs: 用于修改文档，例如修改 README 文件、API 文档等；
     - style: 用于修改代码的样式，例如调整缩进、空格、空行等；
     - refactor: 用于重构代码，例如修改代码结构、变量名、函数名等但不修改功能逻辑；
     - perf: 用于优化性能，例如提升代码的性能、减少内存占用等；
     - test: 用于修改测试用例，例如添加、删除、修改代码的测试用例等。
  5. 脚注中除了 `BREAKING CHANGE: <description>` ，其它条目应该采用类似 [git trailer format](https://git-scm.com/docs/git-interpret-trailers) 这样的惯例。

  其它提交类型在约定式提交规范中并没有强制限制，并且在语义化版本中没有隐式影响（除非它们包含 BREAKING CHANGE）。 可以为提交类型添加一个围在圆括号内的范围，以为其提供额外的上下文信息。例如 `feat(parser): adds ability to parse arrays.`。

#### clang-format

> https://clang.llvm.org/docs/ClangFormat.html
>
> https://clang.llvm.org/docs/ClangFormatStyleOptions.html
>
> [vscode code-formatting](https://code.visualstudio.com/docs/cpp/cpp-ide#_code-formatting)

`clang-format` 是一个由 LLVM 项目开发的代码格式化工具，支持多种编程语言，包括 C、C++、Java、JavaScript、Objective-C、Protobuf 和 C#。它可以根据预定义的或自定义的代码风格规则自动格式化代码，从而保持代码的一致性和可读性。

#### Doxygen

> [doxygen](https://www.doxygen.nl/index.html)
>
> [Do one thing and do it well](http://cedar-renjun.github.io/2014/03/21/learn-doxygen-in-10-minutes/index.html)
>
> [Microsoft HTML Help Downloads](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/htmlhelp/microsoft-html-help-downloads)
>
> [Microsoft HTML Help Workshop](https://www.helpandmanual.com/downloads_mscomp.html)
>
> [Graphviz](https://graphviz.org)

配置好`Doxyfile`后，运行以下命令生成HTML文档：

```cpp
doxygen Doxyfile
```

生成的HTML文档会保存在`docs/html`目录中，你可以通过浏览器打开`index.html`文件来查看文档。

- 记录代码允许`doxygen`解析并生成`html/pdf/latex`输出，因此在项目早期阶段正确完成这一点非常重要。

  - 使用`doxygen`启用的文档风格记录变量、函数和结构体/枚举
  - 始终使用`\`表示`doxygen`，不要使用`@`
  - 始终使用从行首偏移`5x4`空格（5个制表符）的文本

  ```c
  /**
   * \brief           Holds pointer to first entry in linked list
   *                  Beginning of this text is 5 tabs (20 spaces) from beginning of line
   */
  static
  type_t* list;
  ```

- 每个结构体/枚举成员必须包括文档

- 将不同结构体成员的注释起始部分对齐到同一列

  ```c
  /**
   * \brief           This is point struct
   * \note            This structure is used to calculate all point
   *                      related stuff
   */
  typedef struct {
      int32_t x;                                  /*!< Point X coordinate */
      int32_t y;                                  /*!< Point Y coordinate */
      int32_t size;                               /*!< Point size.
                                                      Since comment is very big,
                                                      you may go to next line */
  } point_t;

  /**
   * \brief           Point color enumeration
   */
  typedef enum {
      COLOR_RED,                                  /*!< Red color */
      COLOR_GREEN,                                /*!< Green color */
      COLOR_BLUE,                                 /*!< Blue color */
  } point_color_t;
  ```

- 函数的文档必须写在函数实现中（通常是源文件）

- 函数必须包括简要说明和所有参数的文档

- 每个参数必须注明是in还是out，分别表示输入和输出

- 如果函数返回内容，函数必须包括返回参数。这不适用于void函数

- 函数可以包括其他`doxygen`关键字，如`note`或`warning`

- 在参数名称和描述之间使用冒号`:`

  ```c
  /**
   * \brief           Sum `2` numbers
   * \param[in]       a: First number
   * \param[in]       b: Second number
   * \return          Sum of input values
   */
  int32_t
  sum(int32_t a, int32_t b) {
      return a + b;
  }

  /**
   * \brief           Sum `2` numbers and write it to pointer
   * \note            This function does not return value, it stores it to pointer instead
   * \param[in]       a: First number
   * \param[in]       b: Second number
   * \param[out]      result: Output variable used to save result
   */
  void
  void_sum(int32_t a, int32_t b, int32_t* result) {
      *result = a + b;
  }
  ```

- 如果函数返回枚举成员，使用`ref`关键字指定哪一个

  ```c
  /**
   * \brief           My enumeration
   */
  typedef enum {
      MY_ERR,                                     /*!< Error value */
      MY_OK                                       /*!< OK value */
  } my_enum_t;

  /**
   * \brief           Check some value
   * \return          \ref MY_OK on success, member of \ref my_enum_t otherwise
   */
  my_enum_t
  check_value(void) {
      return MY_OK;
  }
  ```

- 使用符号（`NULL` => NULL）表示常量或数字

  ```c
  /**
   * \brief           Get data from input array
   * \param[in]       in: Input data
   * \return          Pointer to output data on success, `NULL` otherwise
   */
  const void *
  get_data(const void* in) {
      return in;
  }
  ```

- 宏的文档必须包括`hideinitializer`  `doxygen`命令

  ```c
  /**
   * \brief           Get minimal value between `x` and `y`
   * \param[in]       x: First value
   * \param[in]       y: Second value
   * \return          Minimal value between `x` and `y`
   * \hideinitializer
   */
  #define MIN(x, y)       ((x) < (y) ? (x) : (y))
  ```

### 文件组织

#### **目录结构**

> 一个清晰的目录结构能够帮助开发者快速定位文件并理解项目的整体架构。以下是一个推荐的目录结构：

```bash
project/
├── .vscode/            				# vscode配置
│   ├── c_cpp_properties.json 			# 配置 C/C++ 项目
│   ├── extensions.json       			# 推荐VS Code 扩展列表  
│   ├── file_comment.code-snippets      # 自定义代码片段文件     
│   ├── launch.json          			# 配置调试环境
│   ├── settings.json          			# 配置项目的本地设置
│   └── tasks.json          			# 定义构建任务或其他自动化任务
├── src/                				# 源代码文件
│   ├── main.c          				# 主程序入口
│   ├── module1/        				# 模块1相关代码
│   │   ├── module1.c
│   │   └── include
│   │   	└── module1
│   │   		└── module1.h
│   ├── module2/        				# 模块2相关代码
│   │   ├── module2.c
│   │   └── include/
│   │   	└── module2/
│   │   		└── module2.h
│   └── utils/          				# 工具函数或通用工具模块
│       ├── utils.c
│       └── utils.h
├── lib/                				# 第三方库或静态库文件
│   ├── libfoo.a
│   └── libbar.so
├── docs/               				# 文档目录
│   ├── design.md       			# 设计文档
│   └── api.md          			# API文档
├── test/               			# 测试代码
│   ├── module1
│   │	└── test_module1.c
│   └── module2
│    	└── test_module2.c
├── build/              			# 编译输出目录
│   ├── obj/            			# 中间目标文件
│   └── bin/            			# 可执行文件
├── scripts/            			# 脚本文件（如构建脚本、自动化脚本）
│   └── build.sh
├── Makefile            			# 构建工具配置文件
└── README.md           			# 项目说明文件
```

#### 文件命名

> 目录名称如果无特殊的需求，请使用全小写的形式；
>
> 目录名称应能够反映部分的意思，例如各芯片移植由其芯片名称构成或芯片类别构成；
>
> components 目录下能够反映组件的意义。

#### 文件名称

- **源文件** ：使用小写字母和下划线命名，例如 `module1.c`。
- **头文件** ：与对应的源文件同名，例如 `module1.h`。
- **测试文件** ：以 `test_` 开头，例如 `test_module1.c`。
- **第三方库文件** ：保持原命名，但可以放在 `lib/` 目录下。

#### 头文件/源文件管理

> 头文件：
>
> - 声明函数原型。
> - 定义结构体、枚举和宏。
> - 包含必要的依赖头文件。
> - 避免在头文件中包含实现细节。
>
> 源文件：
>
> - **单一职责原则** ：每个源文件应专注于实现一个模块的功能。
> - **函数声明与实现分离** ：函数声明放在头文件中，实现放在源文件中。
> - **避免全局变量** ：尽量减少全局变量的使用，优先使用局部变量或通过参数传递。

- 文件末尾留一个空行

- 每个文件必须包括`doxygen`注解以及简要描述，后跟空行（使用`doxygen`时）

  ```c
  /**
   * \file            template.h
   * \brief           Template include file
   */
                      /* Here is empty line */
  ```

- 每个文件（头文件或源文件）必须包括许可（开头注释包含单个星号，因为这必须被doxygen忽略）

- 使用项目/库已使用的相同许可

  ```c
  /**
   * \file            template.h
   * \brief           Template include file
   */

  /*
   * Copyright (c) year FirstName LASTNAME
   *
   * Permission is hereby granted, free of charge, to any person
   * obtaining a copy of this software and associated documentation
   * files (the "Software"), to deal in the Software without restriction,
   * including without limitation the rights to use, copy, modify, merge,
   * publish, distribute, sublicense, and/or sell copies of the Software,
   * and to permit persons to whom the Software is furnished to do so,
   * subject to the following conditions:
   *
   * The above copyright notice and this permission notice shall be
   * included in all copies or substantial portions of the Software.
   *
   * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
   * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
   * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   * OTHER DEALINGS IN THE SOFTWARE.
   *
   * This file is part of library_name.
   *
   * Author:          FirstName LASTNAME <optional_email@example.com>
   */
  ```

- 头文件必须包含`#ifndef`防护

- 头文件必须包含C++检查

- 在C++检查之外包含外部头文件

- 先包含带有`STL C`文件的外部头文件，然后是应用自定义文件

- 头文件必须仅包含每个其他头文件以正确编译，但不能更多（.c应包含其余必要的部分）

- 头文件必须仅暴露模块公共变量/类型/函数

- 在头文件中使用extern声明全局模块变量，在源文件中稍后定义

  ```c
  /* file.h ... */
  #ifndef ...

  extern int32_t my_variable; /* This is global variable declaration in header */

  #endif

  /* file.c ... */
  int32_t my_variable;        /* Actually defined in source */
  ```

- 永远不要在另一个`.c`文件中包含`.c`文件

- `.c`文件应首先包含相应的`.h`文件，然后是其他的，除非另有特别必要

- 不要在头文件中包含模块私有声明

- 头文件示例（为了示例不包含许可）

  ```c
  /* License comes here */
  #ifndef TEMPLATE_HDR_H
  #define TEMPLATE_HDR_H
  
  /* Include headers */
  
  #ifdef __cplusplus
  extern "C" {
  #endif /* __cplusplus */
  
  /* File content here */
  
  #ifdef __cplusplus
  }
  #endif /* __cplusplus */
  
  #endif /* TEMPLATE_HDR_H */
  ```

#### **文档**

- **README.md** ：项目概述、编译方法、运行方式等。
- **设计文档** ：记录系统架构、模块划分和关键设计决策。
- **API文档** ：说明公共接口的使用方法。

### 工具集成

#### 构建系统

- **Makefile**:
- **cmake**:
- **bazel**:

#### 测试与调试

#### 代码审查

> 定期进行代码审查，确保代码质量。

#### 持续集成

> 使用 CI 工具（如 Jenkins、GitHub Actions）自动化构建和测试流程。

## 贡献

欢迎任何形式的贡献！如果你发现任何问题或希望增加新功能，请提交 Issue 或 Pull Request。

### 提交步骤

1. Fork 此仓库
2. 创建分支 (`git checkout -b feat/your-feature`)
3. 按照约定式提交规范提交更改 (`git commit -m 'feat(scope): 添加新功能'`)
4. 推送到分支 (`git push origin feat/your-feature`)
5. 创建一个 Pull Request