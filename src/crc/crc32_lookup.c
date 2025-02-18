/**
 * \file            crc32_lookup_lookup.c
 * \brief           Cyclic Redundancy Check (CRC32) Look-Up Table
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
#include "crc/crc32_lookup.h"
#include "crc/bit_utils.h"

/* Private variables -------------------------------------------------------- */
/**
 * \brief       polynomial: 0x04C11DB7
 *
 * Polynomial Polynomial (x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10
 * + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1): 0x04C11DB7
 */
static const uint32_t crc32_poly_0x04C11DB7_table[16] = {
    0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B,
    0x1A864DB2, 0x1E475005, 0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61,
    0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
};

/* Public functions --------------------------------------------------------- */
void crc32_lookup_init(crc32_lookup_ctx_t* ctx,
                       crc32_lookup_param_model_e model) {
    switch (model) {
    case CRC32_LOOKUP_MODEL:    // CRC-32
        ctx->init = 0xFFFFFFFF; // Initial value
        ctx->poly =
            0x04C11DB7; // Polynomial (x^32 + x^26 + x^23 + x^22 + x^16 + x^12 +
                        // x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1)
        ctx->xor_out = 0xFFFFFFFF; // Final XOR value
        ctx->ref_in = true;        // Reverse input bits
        ctx->ref_out = true;       // Reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint32_t*)crc32_poly_0x04C11DB7_table;
        break;

    case CRC32_MPEG2_LOOKUP_MODEL: // CRC-32 MPEG-2
        ctx->init = 0xFFFFFFFF;    // Initial value
        ctx->poly =
            0x04C11DB7; // Polynomial (x^32 + x^26 + x^23 + x^22 + x^16 + x^12 +
                        // x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1)
        ctx->xor_out = 0x00000000; // Final XOR value
        ctx->ref_in = false;       // Do not reverse input bits
        ctx->ref_out = false;      // Do not reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint32_t*)crc32_poly_0x04C11DB7_table;
        break;

    case CRC32_NONE_LOOKUP_MODEL: // No CRC (dummy)
    default:
        ctx->init = 0x00000000;    // Default initial value
        ctx->poly = 0x00000000;    // Default polynomial (no operation)
        ctx->xor_out = 0x00000000; // Default XOR value
        ctx->ref_in = false;       // Do not reverse input bits
        ctx->ref_out = false;      // Do not reverse output bits
        break;
    }
}

void crc32_lookup_update(crc32_lookup_ctx_t* ctx, const uint8_t* buf,
                         uint32_t len) {
    if (ctx->table == NULL || ctx->table_len < 16) {
        return;
    }

    uint32_t crc = ctx->init;
    for (uint32_t i = 0; i < len; i++) {
        uint8_t data = buf[i];

        // Reverse input bits if required by the model
        if (ctx->ref_in) {
            data = reverse_bits(data);
        }

        uint32_t temp = data;
        // Update the CRC value using the lookup table (Step 1: Process the high
        // 4 bits)
        crc = ctx->table[(((temp << 24) ^ crc) >> 28) & 0x0F] ^ (crc << 4);
        // Explanation:
        // - (data ^ crc): XOR the current byte with the current CRC value
        // - >> 4: Shift right by 4 bits to extract the high 4 bits
        // - & 0x0F: Mask to ensure the result is within the range 0-15
        // - ctx->table[...]: Look up the corresponding value in the precomputed
        // table
        // - ^ (crc << 4): XOR the table value with the left-shifted CRC value

        // Update the CRC value using the lookup table (Step 2: Process the low
        // 4 bits)
        crc = ctx->table[(((temp << 28) ^ crc) >> 28) & 0x0F] ^ (crc << 4);
        // Explanation:
        // - (data << 4): Shift the current byte left by 4 bits to process the
        // low 4 bits
        // - ^ crc: XOR the shifted byte with the current CRC value
        // - >> 4: Shift right by 4 bits to extract the relevant 4 bits
        // - & 0x0F: Mask to ensure the result is within the range 0-15
        // - ctx->table[...]: Look up the corresponding value in the precomputed
        // table
        // - ^ (crc << 4): XOR the table value with the left-shifted CRC value
    }

    // Reverse output bits if required by the model
    if (ctx->ref_out) {
        crc = reverse_bits_32(crc);
    }

    ctx->init = crc ^ ctx->xor_out;
}

uint32_t crc32_lookup_final(crc32_lookup_ctx_t* ctx) {
    return ctx->init;
}

uint32_t crc32_lookup_calculate(crc32_lookup_param_model_e model,
                                const uint8_t* buf, uint32_t len) {
    crc32_lookup_ctx_t ctx; // Declare the CRC32 context structure

    // Initialize the CRC32 context based on the specified model
    crc32_lookup_init(&ctx, model);

    // Update the CRC32 calculation with the input data buffer
    crc32_lookup_update(&ctx, buf, len);

    // Retrieve and return the final CRC32 checksum value
    return crc32_lookup_final(&ctx);
}

void crc32_lookup_pack_buf(crc32_lookup_param_model_e model, uint8_t* buf,
                           uint32_t len) {
    if (buf == NULL || len <= sizeof(uint32_t)) {
        return; // Not enough space for CRC
    }

    uint32_t crc32_lookup = crc32_lookup_calculate(model, buf,
                                                   len - sizeof(uint32_t));
    *(buf + len - 4) = (uint8_t)(crc32_lookup & 0xFF);
    *(buf + len - 3) = (uint8_t)((crc32_lookup >> 8) & 0xFF);
    *(buf + len - 2) = (uint8_t)((crc32_lookup >> 16) & 0xFF);
    *(buf + len - 1) = (uint8_t)((crc32_lookup >> 24) & 0xFF);
}

bool crc32_lookup_verify_buf(crc32_lookup_param_model_e model,
                             const uint8_t* buf, uint32_t len) {
    if (buf == NULL || len <= sizeof(uint32_t)) {
        return false; // Not enough space for CRC
    }

    uint32_t stored_crc = (*(buf + len - 1) << 24) | (*(buf + len - 2) << 16)
                          | (*(buf + len - 3) << 8) | (*(buf + len - 4));
    uint32_t calculated_crc = crc32_lookup_calculate(model, buf,
                                                     len - sizeof(uint32_t));
    return (stored_crc == calculated_crc);
}

void crc32_generate_table(uint32_t polynomial, uint32_t table[],
                          uint32_t table_len) {
    // Iterate over all possible byte values (0 to 255)
    for (uint32_t i = 0; i < table_len; i++) {
        uint32_t crc = 0; // Initialize the CRC value to 0
        uint32_t data = i;

        // XOR the current byte (i) with the initial CRC value
        crc ^= (data << 24);

        // Perform the CRC calculation for each bit in the byte
        for (uint8_t j = 0; j < 8; j++) {
            // Check if the most significant bit (MSB) is set
            if (crc & 0x80000000) {
                // If MSB is set, shift left and apply the polynomial
                crc = (crc << 1) ^ polynomial;
            } else {
                // If MSB is not set, just shift left
                crc <<= 1;
            }
        }

        // Store the computed CRC value in the lookup table
        table[i] = crc;
    }
}

/* ----------------------------- end of file -------------------------------- */
