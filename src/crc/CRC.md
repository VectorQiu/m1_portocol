# CRC(循环冗余校验)

## 参考资料

- [Wikipedia - Cyclic Redundancy Check](https://en.wikipedia.org/wiki/Cyclic_redundancy_check)
- [中文维基百科 - 循环冗余校验](https://zh.wikipedia.org/wiki/循環冗餘校驗)
- [SEGGER Wiki - CRC](https://wiki.segger.com/CRC)
- [华为信息百科 - CRC](https://info.support.huawei.com/info-finder/encyclopedia/zh/CRC.html)
- [SegmentFault - CRC详解](https://segmentfault.com/a/1190000018094567)

# 1. 字节序

**字节序** （Endianness）是指在计算机内存中或数字通信链路中，多字节数据的排列顺序。它影响数据在不同系统之间的传输和存储方式。

## 1.1 大端序与小端序

- **大端序（Big Endian）** ：高位字节存储在低地址处，低位字节存储在高地址处。
- **小端序（Little Endian）** ：低位字节存储在低地址处，高位字节存储在高地址处。

### 示例

假设变量 `x` 类型为 `int`，值为 `0x01234567`：

| 地址  | 小端序 | 大端序 |
| ----- | ------ | ------ |
| 0x100 | 0x67   | 0x01   |
| 0x101 | 0x45   | 0x23   |
| 0x102 | 0x23   | 0x45   |
| 0x103 | 0x01   | 0x67   |

对于 `0x0A0B0C0D`：

- **大端序** ：`0x0A 0x0B 0x0C 0x0D`
- **小端序** ：`0x0D 0x0C 0x0B 0x0A`

混合模式示例（高16位和低16位以大端序存储，但16位内部以小端存储）：

```txt
....|0x0B|0x0A|0x0D|0x0C|....
```



## 1.2 处理器体系

不同处理器架构对字节序的支持如下：

- **小端序** ：x86、MOS Technology 6502、Z80、VAX、PDP-11 等。
- **大端序** ：Motorola 6800、Motorola 68000、PowerPC 970、System/370、SPARC（除 V9 外）等。
- **可配置字节序** ：ARM、PowerPC（除 PowerPC 970 外）、DEC Alpha、SPARC V9、MIPS、PA-RISC 及 IA64。

## 1.3 网络序

网络传输通常采用**大端序** ，也称为**网络字节序** 。IP 协议定义大端序为标准字节序。

### 转换函数

Berkeley 套接字定义了以下函数用于字节序转换：

- `htonl`、`htons`：主机字节序转网络字节序。
- `ntohl`、`ntohs`：网络字节序转主机字节序。

## 1.4 位序

位序决定了数据在串行通信中的传输顺序：

- 小端序（先传低位） ：
  - RS-232、RS-422、RS-485、USB、以太网（每字节内低位先传）。
- 大端序（先传高位） ：
  - I2C 协议、SPI 协议、摩尔斯电码。

------

# 2. 循环冗余校验（CRC）

**循环冗余校验（CRC）** 是一种错误检测代码，广泛应用于数字网络和存储设备中，用于检测数据的意外更改。进入这些系统的数据块会根据其内容的[多项式除法的余数附加一个短](https://en.wikipedia.org/wiki/Polynomial_long_division)*校验值。*在检索时，重复计算，如果检查值不匹配，则可以采取纠正措施来防止数据损坏。CRC 可用于[纠错](https://en.wikipedia.org/wiki/Error_detection_and_correction)（请参阅[位过滤器](https://en.wikipedia.org/wiki/Mathematics_of_cyclic_redundancy_checks#Bitfilters)）。[[1\]](https://en.wikipedia.org/wiki/Cyclic_redundancy_check#cite_note-1)

之所以称为 CRC，是因为*校验*（数据验证）值是一种*冗余（它在不添加*[信息的情况](https://en.wikipedia.org/wiki/Entropy_(information_theory))下扩展消息）并且该[算法](https://en.wikipedia.org/wiki/Algorithm)基于[*循环*码](https://en.wikipedia.org/wiki/Cyclic_code)。[CRC 很受欢迎，因为它们易于在二进制硬件](https://en.wikipedia.org/wiki/Computer_hardware)中实现，易于进行数学分析，并且特别擅长检测由传输通道中的[噪声引起的常见错误。](https://en.wikipedia.org/wiki/Noise_(electronics))因为校验值的长度是固定的，所以生成它的[函数偶尔会被用作](https://en.wikipedia.org/wiki/Function_(mathematics))[哈希函数](https://en.wikipedia.org/wiki/Hash_function)。

当其校验值是*n*位长时，CRC 称为*n*位 CRC 。对于给定的*n*，可能有多个 CRC，每个都有不同的多项式。这样的多项式具有最高次数*n*，这意味着它有*n* + 1项。换句话说，多项式的长度为*n* + 1；它的编码需要*n* + 1位。请注意，**大多数多项式规范会丢弃[MSB](https://en.wikipedia.org/wiki/Most_significant_bit)或[LSB](https://en.wikipedia.org/wiki/Least_significant_bit)，因为它们始终为 1。**

最简单的错误检测系统，即[奇偶校验位](https://en.wikipedia.org/wiki/Parity_bit)，实际上是一个 1 位的 CRC：它使用生成多项式 *x* + 1（两项），[[3\]](https://en.wikipedia.org/wiki/Cyclic_redundancy_check#cite_note-Ergen-2008-3)并命名为 CRC-1。

## 2.1 数据完整性

CRC 的设计目标是防止通信通道上的常见错误类型，并提供快速且合理的数据完整性保证。然而，CRC 不适合防止故意篡改数据，因为攻击者可以重新计算 CRC 并替换数据而不被检测到。

### 安全性建议

- 使用**消息认证码（MAC）** 或**数字签名** 来防止恶意篡改。
- 结合密码学哈希函数（如 SHA 系列）增强安全性。

## 2.2 计算方法

CRC 的计算基于多项式除法。以下是计算步骤的详细说明：

### 示例

假设需要编码的消息为 `11010011101100`，使用生成多项式 `x³ + x + 1`（二进制表示为 `1011`），计算 3 位 CRC。

1. **消息填充**
   在消息后添加 `n` 个零（`n` 为 CRC 长度）。

   ```txt
   消息：11010011101100

   填充后：11010011101100 000
   ```

2. **多项式除法**
   使用模 2 除法（不考虑进位和借位）进行计算。

   ```txt
   11010011101100 000 ÷ 1011 = 商 + 余数
   ```

3. **结果**
   最终得到的余数即为 CRC 值。例如，假设余数为 `101`，则 CRC 值为 `101`。

### 注意事项

- 生成多项式的最高次幂决定 CRC 的长度。
- 多项式的 MSB 和 LSB 通常固定为 1，因此在描述时可能省略。

In this example, we shall encode 14 bits of message with a 3-bit CRC, with a polynomial *x*3 + *x* + 1. The polynomial is written in binary as the coefficients; a 3rd-degree polynomial has 4 coefficients (1*x*3 + 0*x*2 + 1*x* + 1). In this case, the coefficients are 1, 0, 1 and 1. The result of the calculation is 3 bits long, which is why it is called a 3-bit CRC. However, you need 4 bits to explicitly state the polynomial.

Start with the message to be encoded:

```
11010011101100
```

This is first padded with zeros corresponding to the bit length *n* of the CRC. This is done so that the resulting code word is in [systematic](https://en.wikipedia.org/wiki/Systematic_code) form. Here is the first calculation for computing a 3-bit CRC:

```
11010011101100 000 <--- input right padded by 3 bits
1011               <--- divisor (4 bits) = x³ + x + 1
------------------
01100011101100 000 <--- result
```

The algorithm acts on the bits directly above the divisor in each step. The result for that iteration is the bitwise XOR of the polynomial divisor with the bits above it. The bits not above the divisor are simply copied directly below for that step. The divisor is then shifted right to align with the highest remaining 1 bit in the input, and the process is repeated until the divisor reaches the right-hand end of the input row. Here is the entire calculation:

```
11010011101100 000 <--- input right padded by 3 bits
1011               <--- divisor
01100011101100 000 <--- result (note the first four bits are the XOR with the divisor beneath, the rest of the bits are unchanged)
 1011              <--- divisor ...
00111011101100 000
  1011
00010111101100 000
   1011
00000001101100 000 <--- note that the divisor moves over to align with the next 1 in the dividend (since quotient for that step was zero)
       1011             (in other words, it doesn't necessarily move one bit per iteration)
00000000110100 000
        1011
00000000011000 000
         1011
00000000001110 000
          1011
00000000000101 000
           101 1
-----------------
00000000000000 100 <--- remainder (3 bits).  Division algorithm stops here as dividend is equal to zero.
```

每一次Xor都是固定不动的生成项与其对应的数据首位“消1”。

那我们就可以假想出一个与生成项长度一致的“盒子”，取出一部分的数据出来若首位是1时就进行一次Xor，遇到0则左移到1为止，左移造成的右端的空缺用0补充。而这里0希望理解为一种“存储”，它“存储” 生成项中未和数据进行计算的那一部分，按顺序先后附加被计算数据的后面，当先一部分的数据全部计算之后，实际上“盒子”中剩下都是未和数据计算的部分的“和”11011 xor 10110 = 11011 xor ( 10000 xor 00100 xor 00010)(这里实际上就是Xor的交换律到后面就会体会到他的强大)

![图片描述](https://segmentfault.com/img/bVbn5m4)

## 2.3 数学公式

CRC为[校验和](https://zh.wikipedia.org/wiki/校验和)的一种，是两个字节数据流采用二进制除法（没有进位，使用[XOR](https://zh.wikipedia.org/wiki/XOR)来代替减法）相除所得到的余数。其中被除数是需要计算校验和的信息数据流的二进制表示；除数是一个长度为![(n+1)](https://wikimedia.org/api/rest_v1/media/math/render/svg/b30a29cfd35628469f9dbffea4804f5b422f3037)的预定义（短）的二进制数，通常用多项式的系数来表示。在做除法之前，要在信息数据之后先加上![n](https://wikimedia.org/api/rest_v1/media/math/render/svg/a601995d55609f2d9f5e233e36fbe9ea26011b3b)个0。

CRC是基于[有限域](https://zh.wikipedia.org/wiki/有限域)**GF(2)**（即[除以2](https://zh.wikipedia.org/wiki/除以2)的[同余](https://zh.wikipedia.org/wiki/同余)）的[多项式环](https://zh.wikipedia.org/wiki/多项式环)。简单的来说，就是所有系数都为0或1（又叫做二进制）的多项式系数的集合，并且集合对于所有的代数操作都是封闭的。例如：

![(x^{3}+x)+(x+1)=x^{3}+2x+1\equiv x^{3}+1](https://wikimedia.org/api/rest_v1/media/math/render/svg/ac743b44be7cd4295a10a0b4796fc7d9c11fb47e)

2会变成0，因为对系数的加法运算都会再取2的模数。乘法也是类似的：

![(x^{2}+x)(x+1)=x^{3}+2x^{2}+x\equiv x^{3}+x](https://wikimedia.org/api/rest_v1/media/math/render/svg/c05b762b1f89934a5e740d72a767cea43842d8a0)

同样可以对多项式作除法并且得到商和余数。例如，如果用*x*3 + *x*2 + *x*除以*x* + 1。会得到：

![{\frac {(x^{3}+x^{2}+x)}{(x+1)}}=(x^{2}+1)-{\frac {1}{(x+1)}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/7f54294988f879cfcf02f798b6f4d2501b6de8f9)

也就是说，

![(x^{3}+x^{2}+x)=(x^{2}+1)(x+1)-1](https://wikimedia.org/api/rest_v1/media/math/render/svg/e702a4b8a7d4906525d0a79fe3e33e4de48e7d04)

等价于

![(x^{2}+x+1)x=(x^{2}+1)(x+1)-1](https://wikimedia.org/api/rest_v1/media/math/render/svg/dc73dd92435b032e7eb76cd613ebeeafe5971baf)

这里除法得到了商*x*2 + 1和余数-1，因为是奇数所以最后一位是1。

字符串中的每一位其实就对应了这样类型的多项式的系数。为了得到CRC，首先将其乘以![x^{n}](https://wikimedia.org/api/rest_v1/media/math/render/svg/150d38e238991bc4d0689ffc9d2a852547d2658d)，这里n是一个固定多项式的**[阶数](https://zh.wikipedia.org/w/index.php?title=多項式的階&action=edit&redlink=1)**，然后再将其除以这个固定的多项式，余数的系数就是CRC。

在上面的等式中，![x^{2}+x+1](https://wikimedia.org/api/rest_v1/media/math/render/svg/78235883dfed13f5c0c7b6fb5aa82c002a1ac649)表示了本来的信息位是`111`, ![x+1](https://wikimedia.org/api/rest_v1/media/math/render/svg/16df430ed7a23df9b160a5bbd957f306a0c3baa7)是所谓的**钥匙**，而余数1也就是![x^{0}](https://wikimedia.org/api/rest_v1/media/math/render/svg/1871ffeb57c11624b375dbb7157d5887c706eb87)就是CRC. key的最高次为1,所以将原来的信息乘上![x^{1}](https://wikimedia.org/api/rest_v1/media/math/render/svg/92004fb9ae631cb2d99594f120855e50b75578e3)来得到![x^{3}+x^{2}+x](https://wikimedia.org/api/rest_v1/media/math/render/svg/ca4a78b924c532d1222a6cd4ff9cb742c805279b)，也可视为原来的信息位补1个零成为`1110`。

一般来说，其形式为：

![M(x)\cdot x^{n}=Q(x)\cdot K(x)-R(x)](https://wikimedia.org/api/rest_v1/media/math/render/svg/f72ac19eb6168035011ca0ea3e4c849bf4108f9c)

这里M(x)是原始的信息多项式。K(x)是![n](https://wikimedia.org/api/rest_v1/media/math/render/svg/a601995d55609f2d9f5e233e36fbe9ea26011b3b)阶的“钥匙”多项式。![M(x)\cdot x^{n}](https://wikimedia.org/api/rest_v1/media/math/render/svg/6cd6a76fb2ef2ab419b5000c04b80db5426fe8fb)表示了将原始信息后面加上![n](https://wikimedia.org/api/rest_v1/media/math/render/svg/a601995d55609f2d9f5e233e36fbe9ea26011b3b)个0。R(x)是余数多项式，即是CRC“校验和”。在通信中，发送者在原始的信息数据M后附加上�![n](https://wikimedia.org/api/rest_v1/media/math/render/svg/a601995d55609f2d9f5e233e36fbe9ea26011b3b)位的R（替换本来附加的0）再发送。接收者收到M和R后，检查![M(x)\cdot x^{n}+R(x)](https://wikimedia.org/api/rest_v1/media/math/render/svg/0afb8cbd152e34ce6a156732934a54c46b9831fb)是否能被![K(x)](https://wikimedia.org/api/rest_v1/media/math/render/svg/19a621a6fbcc971649157a42ea42a599c22eee94)整除。如果是，那么接收者认为该信息是正确的。值得注意的是![M(x)\cdot x^{n}+R(x)](https://wikimedia.org/api/rest_v1/media/math/render/svg/0afb8cbd152e34ce6a156732934a54c46b9831fb)就是发送者所想要发送的数据。这个串又叫做*codeword*.

CRCs经常被叫做“[校验和](https://zh.wikipedia.org/wiki/校验和)”，但是这样的说法严格来说并不是准确的，因为技术上来说，校验“和”是通过加法来计算的，而不是CRC这里的除法。

## 2.4 应用场景

- **数据传输** ：确保网络数据包的完整性。
- **存储设备** ：检测硬盘或闪存中的数据损坏。
- **嵌入式系统** ：验证固件更新的正确性。

------

# 总结

本文详细介绍了字节序和循环冗余校验（CRC）的相关知识。通过理解字节序的概念及其在不同系统中的应用，可以更好地处理跨平台数据传输问题。而 CRC 作为一种高效的错误检测机制，能够有效保障数据的完整性，但在安全性要求较高的场景下需结合其他加密技术使用。

