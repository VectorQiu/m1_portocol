/**
 * \file            m1_typedef.h
 * \brief           Generic type definitions
 * \date            2024-12-10
 */

/*
 * Copyright (c) 2024 Vector Qiu
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
 * This file is part of the m1 protocol library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         V0.0.1
 */
#ifndef __M1_TYPEDEF_H__
#define __M1_TYPEDEF_H__

/* includes ----------------------------------------------------------------- */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "./m1_protocol/single_list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1 typedef manager
 * \brief           m1 typedef manager
 * \{
 */

/* public config ------------------------------------------------------------ */

/* public define ------------------------------------------------------------ */
#define m1_malloc malloc
#define m1_free   free

#include <stdio.h>
#define link_raw(...) printf(__VA_ARGS__)
#define link_hex(name, width, buf, size)                                       \
    do {                                                                       \
        link_raw("%s:\n", name);                                                \
        for (size_t i = 0; i < size; i++) {                                    \
            link_raw("%02X ", ((unsigned char*)buf)[i]);                        \
            if ((i + 1) % width == 0) {                                        \
                link_raw("\n");                                                 \
            }                                                                  \
        }                                                                      \
        if (size % width != 0) {                                               \
            link_raw("\n");                                                     \
        }                                                                      \
    } while (0)

#define link_debug(...)   link_raw(__VA_ARGS__)
#define link_info(...)    link_raw(__VA_ARGS__)
#define link_warning(...) link_raw(__VA_ARGS__)
#define link_error(...)   link_raw(__VA_ARGS__)

#ifndef U8_MAX

typedef float f32;
typedef double f64;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef size_t usize;

typedef volatile int32_t vi32;
typedef volatile int16_t vi16;
typedef volatile int8_t vi8;

typedef volatile uint32_t vu32;
typedef volatile uint16_t vu16;
typedef volatile uint8_t vu8;

#define U8_MAX  UINT8_MAX
#define I8_MAX  INT8_MAX
#define I8_MIN  INT8_MIN
#define U16_MAX UINT16_MAX
#define I16_MAX INT16_MAX
#define I16_MIN INT16_MIN
#define U32_MAX UINT32_MAX
#define I32_MAX INT32_MAX
#define I32_MIN INT32_MIN

#endif

/**
 * \defgroup        Bit Operation
 * \brief           Bit Operation Macros
 * \{
 */

#ifndef SWAP16
/**
 * \brief           Swaps the byte order of a 16-bit value.
 * \param[in]       x: The 16-bit integer to be swapped.
 * \return          The 16-bit integer after the byte order swap.
 */
#define SWAP16(x) (((x) >> 8) | ((x) << 8))
#endif

#ifndef SWAP32
/**
 * \brief           Swaps the byte order of a 32-bit value.
 * \param[in]       x: The 32-bit integer to be swapped.
 * \return          The 32-bit integer after the byte order swap.
 */
#define SWAP32(x)                                                              \
    (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8)       \
     | ((x) << 24))
#endif

#ifndef BIT
/**
 * \brief           Generate a bit mask for a single bit.
 * \param[in]       x: The bit position (0-based).
 * \return          A mask with the specified bit set.
 */
#define BIT(x) (1U << (x))
#endif

#ifndef BITS
/**
 * \brief           Generate a bit mask for a range of bits.
 * \param[in]       start: The starting bit position (0-based).
 * \param[in]       len: The number of bits in the range.
 * \return          A mask with the specified bit range set.
 */
#define BITS(start, len) (((1U << (len)) - 1) << (start))
#endif

#ifndef BIT_SET
/**
 * \brief           Set a specific bit in a value.
 * \param[in]       value: The variable to modify.
 * \param[in]       bit: The bit position to set (0-based).
 */
#define BIT_SET(value, bit) ((value) |= (1U << (bit)))
#endif

#ifndef BIT_CLEAR
/**
 * \brief           Clear a specific bit in a value.
 * \param[in]       value: The variable to modify.
 * \param[in]       bit: The bit position to set (0-based).
 */
#define BIT_CLEAR(value, bit) ((value) &= ~(1U << (bit)))
#endif

#ifndef BIT_TOGGLE
/**
 * \brief           Toggle a specific bit in a value.
 *
 * \param[in]       value: The variable to modify.
 * \param[in]       bit: The bit position to set (0-based).
 */
#define BIT_TOGGLE(value, bit) ((value) ^= (1U << (bit)))
#endif

#ifndef BIT_CHECK
/**
 * \brief           Check if a specific bit is set.
 * \param[in]       value: The variable to check.
 * \param[in]       bit: The bit position to set (0-based).
 * \return          Non-zero if the bit is set, otherwise 0.
 */
#define BIT_CHECK(value, bit) ((value) & (1U << (bit)))
#endif

/**
 * \}
 */

/**
 * \defgroup        Register Operation
 * \brief           Register Operation Macros
 * \{
 */

#ifndef REG_WRITE
/**
 * \brief           Write a value to a memory-mapped register.
 * \param[in]       addr: The address of the register.
 * \param[in]       value: The value to write.
 */
#define REG_WRITE(addr, value) (*((volatile uint32_t*)(addr)) = (value))
#endif

#ifndef REG_READ
/**
 * \brief           Read the value of a memory-mapped register.
 * \param[in]       addr: The address of the register.
 * \return          The value of the register.
 */
#define REG_READ(addr) (*((volatile uint32_t*)(addr)))
#endif

#ifndef REG_SET_BIT
/**
 * \brief           Set a specific bit in a memory-mapped register.
 * \param[in]       addr: The address of the register.
 * \param[in]       bit: The bit position to set (0-based).
 */
#define REG_SET_BIT(addr, bit) (*((volatile uint32_t*)(addr)) |= (1U << (bit)))
#endif

#ifndef REG_CLR_BIT
/**
 * \brief           Clear a specific bit in a memory-mapped register.
 * \param[in]       addr: The address of the register.
 * \param[in]       bit: The bit position to clear (0-based).
 */
#define REG_CLR_BIT(addr, bit) (*((volatile uint32_t*)(addr)) &= ~(1U << (bit)))
#endif

/**
 * \}
 */

/**
 * \defgroup        Memory alignment
 * \brief           Memory alignment Macros
 * \{
 */

#ifndef BYTES_ALIGN
/**
 * \brief           Align a value to a specified boundary.
 *
 * \param[in]       value: The value to align.
 * \param[in]       alignment: The alignment boundary (must be a power of 2).
 * \return          The aligned value.
 */
#define BYTES_ALIGN(value, alignment)                                          \
    (((value) + (alignment) - 1) & ~((alignment) - 1))
#endif

/**
 * \}
 */

/**
 * \defgroup        Mathematical
 * \brief           Mathematical macros
 * \{
 */

#ifndef MIN
/**
 * \brief           Get the minimum of two values.
 * \param[in]       a: The first value.
 * \param[in]       b: The second value.
 * \return          The smaller of the two values.
 */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
/**
 * \brief           Get the maximum of two values.
 * \param[in]       a: The first value.
 * \param[in]       b: The second value.
 * \return          The larger of the two values.
 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef ABS
/**
 * \brief           Get the absolute value of a number.
 * \param[in]       x: The input number.
 * \return          The absolute value of the input.
 */
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

#ifndef ARRAY_SIZE
/**
 * \brief           Get the number of elements in an array.
 * \param           arr: The array to evaluate.
 * \return          The number of elements in the array.
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

/**
 * \}
 */

/* public typedef enum ------------------------------------------------------ */
/**
 * \brief Enumeration of possible states and error codes.
 */
typedef enum {
    E_STATE_OK = 0,        /*!< State is okay and no errors occurred. */
    E_STATE_INVAL,         /*!< Invalid argument passed to the function. */
    E_STATE_NO_DATA,       /*!< No data available to process. */
    E_STATE_NO_SPACE,      /*!< Insufficient space for the operation. */
    E_STATE_ADDR,          /*!< Address-related error, e.g., invalid address. */
    E_STATE_BUSY,          /*!< Device or resource is currently busy. */
    E_STATE_IO,            /*!< Input/output error occurred. */
    E_STATE_NOT_IMPLEMENT, /*!< Functionality is not implemented. */
    E_STATE_ARGUMENT_BIG,  /*!< Argument list is too large for processing. */
    E_STATE_TIMEOUT,       /*!< Operation timed out before completion. */
    E_STATE_NOT_EXIST,     /*!< Requested item does not exist. */
    E_STATE_ERROR,         /*!< Generic error occurred. */
    E_STATE_REPEATED,      /*!< Non-continuous or repeated data encountered. */
} etype_e;

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_TYPEDEF_H__ */

/* ----------------------------- end of file -------------------------------- */
