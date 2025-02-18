/**
 * \file            crc32.c
 * \brief           Cyclic Redundancy Check (CRC32)
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
 * This file is part of the crc32 library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         v0.0.1
 */
/* includes ----------------------------------------------------------------- */
#include "crc/crc32.h"
#include "crc/bit_utils.h"

/* Public functions --------------------------------------------------------- */
void crc32_init(crc32_ctx_t* ctx, crc32_param_model_e model) {
    switch (model) {
    case CRC32_MODEL:           // CRC-32
        ctx->init = 0xFFFFFFFF; // Initial value
        ctx->poly =
            0x04C11DB7; // Polynomial (x^32 + x^26 + x^23 + x^22 + x^16 + x^12 +
                        // x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1)
        ctx->xor_out = 0xFFFFFFFF; // Final XOR value
        ctx->ref_in = true;        // Reverse input bits
        ctx->ref_out = true;       // Reverse output bits
        break;

    case CRC32_MPEG2_MODEL:     // CRC-32 MPEG-2
        ctx->init = 0xFFFFFFFF; // Initial value
        ctx->poly =
            0x04C11DB7; // Polynomial (x^32 + x^26 + x^23 + x^22 + x^16 + x^12 +
                        // x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1)
        ctx->xor_out = 0x00000000; // Final XOR value
        ctx->ref_in = false;       // Do not reverse input bits
        ctx->ref_out = false;      // Do not reverse output bits
        break;

    case CRC32_NONE_MODEL: // No CRC (dummy)
    default:
        ctx->init = 0x00000000;    // Default initial value
        ctx->poly = 0x00000000;    // Default polynomial (no operation)
        ctx->xor_out = 0x00000000; // Default XOR value
        ctx->ref_in = false;       // Do not reverse input bits
        ctx->ref_out = false;      // Do not reverse output bits
        break;
    }
}

void crc32_update(crc32_ctx_t* ctx, const uint8_t* buf, uint32_t len) {
    uint32_t crc = ctx->init;

    for (uint32_t i = 0; i < len; i++) {
        uint32_t data = buf[i];

        if (ctx->ref_in) {
            data = reverse_bits((uint8_t)data);
        }

        crc ^= (data << 24);

        for (int j = 0; j < 8; j++) {
            if (crc & 0x80000000) {
                crc = (crc << 1) ^ ctx->poly;
            } else {
                crc <<= 1;
            }
        }
    }

    if (ctx->ref_out) {
        crc = reverse_bits_32(crc);
    }

    ctx->init = crc ^ ctx->xor_out;
}

uint32_t crc32_final(crc32_ctx_t* ctx) {
    return ctx->init;
}

uint32_t crc32_calculate(crc32_param_model_e model, const uint8_t* buf,
                         uint32_t len) {
    crc32_ctx_t ctx; // Declare the CRC32 context structure

    // Initialize the CRC32 context based on the specified model
    crc32_init(&ctx, model);

    // Update the CRC32 calculation with the input data buffer
    crc32_update(&ctx, buf, len);

    // Retrieve and return the final CRC32 checksum value
    return crc32_final(&ctx);
}

void crc32_pack_buf(crc32_param_model_e model, uint8_t* buf, uint32_t len) {
    if (buf == NULL || len <= sizeof(uint32_t)) {
        return; // Not enough space for CRC
    }

    uint32_t crc32 = crc32_calculate(model, buf, len - sizeof(uint32_t));
    *(buf + len - 4) = (uint8_t)(crc32 & 0xFF);
    *(buf + len - 3) = (uint8_t)((crc32 >> 8) & 0xFF);
    *(buf + len - 2) = (uint8_t)((crc32 >> 16) & 0xFF);
    *(buf + len - 1) = (uint8_t)((crc32 >> 24) & 0xFF);
}

bool crc32_verify_buf(crc32_param_model_e model, const uint8_t* buf,
                      uint32_t len) {
    if (buf == NULL || len <= sizeof(uint32_t)) {
        return false; // Not enough space for CRC
    }

    uint32_t stored_crc = (*(buf + len - 1) << 24) | (*(buf + len - 2) << 16)
                          | (*(buf + len - 3) << 8) | (*(buf + len - 4));
    uint32_t calculated_crc = crc32_calculate(model, buf,
                                              len - sizeof(uint32_t));
    return (stored_crc == calculated_crc);
}

/* ----------------------------- end of file -------------------------------- */
