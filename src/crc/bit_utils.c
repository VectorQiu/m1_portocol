/**
 * \file            bit_utils.c
 * \brief           bit utils
 * \date            2025-02-14
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
 * This file is part of the crc library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         v0.0.1
 */
/* includes ----------------------------------------------------------------- */
#include "crc/bit_utils.h"

/* Public functions --------------------------------------------------------- */
uint8_t reverse_bits(uint8_t data) {
    uint8_t result = 0; // Initialize the result to 0
    for (int i = 0; i < 8; i++) {
        result = (result << 1) | (data & 1); // Shift result left and add the
                                             // least significant bit of data
        data >>= 1; // Shift data right to process the next bit
    }
    return result; // Return the reversed byte
}

uint16_t reverse_bits_16(uint16_t data) {
    uint16_t result = 0;
    for (int i = 0; i < 16; i++) {
        result = (result << 1) | (data & 1);
        data >>= 1;
    }
    return result;
}

uint32_t reverse_bits_32(uint32_t data) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (data & 1);
        data >>= 1;
    }
    return result;
}

/* ----------------------------- end of file -------------------------------- */
