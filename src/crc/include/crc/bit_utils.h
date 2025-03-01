/**
 * \file            bit_utils.h
 * \brief           Provides utility functions for bit manipulation.
 * \date            2025-02-14
 *
 * This file contains functions for reversing the bits of different data types
 * (8-bit, 16-bit, 32-bit) to assist with bitwise operations.
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
 * This file is part of the CRC library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         v0.0.1
 */
#ifndef __BIT_UTILS_H__
#define __BIT_UTILS_H__

/* includes ----------------------------------------------------------------- */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 *
 * \defgroup        bit_utils_manager Bit manipulation utilities
 * \brief           Provides functions for bit reversal of various data types.
 * \{
 */

/* Public functions --------------------------------------------------------- */

/**
 * \brief           Reverses the bits of an 8-bit unsigned integer.
 *
 * This function takes an 8-bit data input and reverses the order of its bits.
 * For example, 0x01 (00000001) becomes 0x80 (10000000).
 *
 * \param[in]       data: 8-bit unsigned integer to be reversed.
 * \return          8-bit unsigned integer with the bits reversed.
 */
uint8_t reverse_bits(uint8_t data);

/**
 * \brief           Reverses the bits of a 16-bit unsigned integer.
 *
 * This function takes a 16-bit data input and reverses the order of its bits.
 * For example, 0x0001 (0000000000000001) becomes 0x8000 (1000000000000000).
 *
 * \param[in]       data: 16-bit unsigned integer to be reversed.
 * \return          16-bit unsigned integer with the bits reversed.
 */
uint16_t reverse_bits_16(uint16_t data);

/**
 * \brief           Reverses the bits of a 32-bit unsigned integer.
 *
 * This function takes a 32-bit data input and reverses the order of its bits.
 * For example, 0x00000001 (00000000000000000000000000000001) becomes
 * 0x80000000 (10000000000000000000000000000000).
 *
 * \param[in]       data: 32-bit unsigned integer to be reversed.
 * \return          32-bit unsigned integer with the bits reversed.
 */
uint32_t reverse_bits_32(uint32_t data);
/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BIT_UTILS_H__ */

/* ----------------------------- end of file -------------------------------- */
