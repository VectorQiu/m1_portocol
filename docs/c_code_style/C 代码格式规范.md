# 推荐的C语言风格和编码规则

本文档描述了Tilen MAJERLE在他的项目和库中使用的C代码风格。

------

## 目录

- [推荐的C语言风格和编码规则](#推荐的c语言风格和编码规则)
  - [目录](#目录)
  - [最重要的单一规则](#最重要的单一规则)
  - [集成 `VSCode`](#集成-vscode)
  - [使用的约定](#使用的约定)
  - [通用规则](#通用规则)
  - [注释](#注释)
  - [函数](#函数)
  - [变量](#变量)
  - [结构体、枚举、类型定义](#结构体枚举类型定义)
  - [复合语句](#复合语句)
  - [Switch 语句](#switch-语句)
  - [宏和预处理器指令](#宏和预处理器指令)
  - [文档](#文档)
  - [头文件/源文件](#头文件源文件)
  - [Clang 格式集成](#clang-格式集成)

------

## 最重要的单一规则

引用 GNOME 开发者网站的一句话：

> 编写代码时最重要的规则是：检查周围的代码并尝试模仿它。
>
> 作为维护者，收到明显与现有代码风格不同的补丁是非常令人沮丧的。这就像有人穿着泥鞋闯进了一个一尘不染的房子。
>
> 因此，无论本文档推荐什么，如果已有代码存在并且你正在修补它，请保持其当前风格一致，即使这不是你最喜欢的风格。

------

## 集成 `VSCode`

`VSCode`自带预装的clang-format工具（`LLVM`包的一部分），旨在帮助开发人员在代码开发过程中进行自动格式化。

因此，它允许用户在文件更改（和保存）时格式化代码。当文件保存时，`VSCode`将尝试调用clang-format来格式化代码。使用的规则在.clang-format文件中。如果clang-format在当前文件路径下找不到规则，它会一路向上查找，直到找到为止。如果仍然没有可用的规则，则使用默认规则。

此存储库始终包含最新的.clang-format文件，其中的规则与解释的规则匹配。你可以将其放在项目的根目录下，甚至可以放在软件开发项目的根目录下——为所有项目使用一个文件！

需要启用一些配置：`VSCode`配置

------

## 使用的约定

本文档中的关键字 必须 、**MUST NOT** 、**REQUIRED** 、**SHALL** 、**SHALL NOT** 、**SHOULD** 、**SHOULD NOT** 、**RECOMMENDED** 、**NOT RECOMMENDED** 、**MAY** 和 **OPTIONAL**应按照`BCP 14`  `[RFC2119]` `[RFC8174]`中的描述进行解释。

------

## 通用规则

以下列出了最明显和最重要的一般规则。在继续阅读其他章节之前，请仔细检查这些规则。

- `clang-format` **SHOULD** 与附加到此存储库的格式化文件一起使用

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

## 注释

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
  * Single line comment without space before asterisk (wrong)
  */

  /*
   * Single line comment in multi-line configuration (wrong)
   */

  /* Single line comment (ok) */
  ```

- 使用12个缩进（12 * 4个空格）偏移进行注释。如果语句大于12个缩进，使注释对齐到下一个可用的4空格缩进（如下示例）

  ```c
  void my_func(void) {
      char a, b;

      a = call_func_returning_char_a(a);          /* This is comment with 12*4 spaces indent from beginning of line */
      b = call_func_returning_char_a_but_func_name_is_very_long(a);   /* This is comment, aligned to 4-spaces indent */
  }
  ```

## 函数

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

------

## 变量

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

------

## 结构体、枚举、类型定义

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

------

## 复合语句

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

------

## Switch 语句

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

## 宏和预处理器指令

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

------

## 文档

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

------

## 头文件/源文件

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

------

## Clang 格式集成

存储库附带始终保持最新的.clang-format文件，这是clang-format工具的输入配置。

它可以无缝集成到大多数最新的技术IDE中，包括VSCode。格式化在每次文件保存时即时发生。

> [vscode code-formatting](https://code.visualstudio.com/docs/cpp/cpp-ide#_code-formatting)
