/**
 * \file            crc8_lookup.c
 * \brief           Cyclic Redundancy Check (CRC8) Look-Up Table
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
#include "crc/crc8_lookup.h"
#include "crc/bit_utils.h"

/* Private variables -------------------------------------------------------- */
/**
 * \brief           polynomial: 0x07
 *
 *  Polynomial (x^8 + x^2 + x^1 + 1): 0x07
 */
static const uint8_t crc8_poly_0x07_table[16] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
    0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
};

/**
 * \brief           polynomial: 0x31
 *
 * Polynomial (x^8 + x^5 + x^4 + 1): 0x31
 */
static const uint8_t crc8_poly_0x31_table[16] = {
    0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
    0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E,
};

/* Public functions --------------------------------------------------------- */
void crc8_lookup_init(crc8_lookup_ctx_t* ctx, crc8_lookup_param_model_e model) {
    switch (model) {
    case CRC8_LOOKUP_MODEL:   // Standard CRC-8
        ctx->init = 0x00;     // Initial value
        ctx->poly = 0x07;     // Polynomial (x^8 + x^2 + x^1 + 1)
        ctx->xor_out = 0x00;  // Final XOR value
        ctx->ref_in = false;  // Do not reverse input bits
        ctx->ref_out = false; // Do not reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint8_t*)crc8_poly_0x07_table;
        break;
    case CRC8_ITU_LOOKUP_MODEL: // CRC-8 ITU
        ctx->init = 0x00;       // Initial value
        ctx->poly = 0x07;       // Polynomial (x^8 + x^2 + x^1 + 1)
        ctx->xor_out = 0x55;    // Final XOR value
        ctx->ref_in = false;    // Do not reverse input bits
        ctx->ref_out = false;   // Do not reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint8_t*)crc8_poly_0x07_table;
        break;
    case CRC8_ROHC_LOOKUP_MODEL: // CRC-8 ROHC
        ctx->init = 0xFF;        // Initial value
        ctx->poly = 0x07;        // Polynomial (x^8 + x^2 + x^1 + 1)
        ctx->xor_out = 0x00;     // Final XOR value
        ctx->ref_in = true;      // Reverse input bits
        ctx->ref_out = true;     // Reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint8_t*)crc8_poly_0x07_table;
        break;
    case CRC8_MAXIM_LOOKUP_MODEL: // CRC-8 Maxim/Dallas
        ctx->init = 0x00;         // Initial value
        ctx->poly = 0x31;         // Polynomial (x^8 + x^5 + x^4 + 1)
        ctx->xor_out = 0x00;      // Final XOR value
        ctx->ref_in = true;       // Reverse input bits
        ctx->ref_out = true;      // Reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint8_t*)crc8_poly_0x31_table;
        break;
    case CRC8_NONE_LOOKUP_MODEL: // No CRC (dummy)
    default:
        ctx->init = 0x00;    // Default initial value
        ctx->xor_out = 0x00; // Default XOR value
        ctx->table_len = 0;
        ctx->table = NULL;
        break;
    }
}

void crc8_lookup_update(crc8_lookup_ctx_t* ctx, const uint8_t* buf,
                        uint32_t len) {
    if (ctx->table == NULL || ctx->table_len < 16) {
        return;
    }

    uint8_t crc = ctx->init;
    for (uint32_t i = 0; i < len; i++) {
        uint8_t data = buf[i];

        // Reverse input bits if required by the model
        if (ctx->ref_in) {
            data = reverse_bits(data);
        }
        // Update the CRC value using the lookup table (Step 1: Process the high
        // 4 bits)
        crc = ctx->table[((data ^ crc) >> 4) & 0x0F] ^ (crc << 4);
        // Explanation:
        // - (data ^ crc): XOR the current byte with the current CRC value
        // - >> 4: Shift right by 4 bits to extract the high 4 bits
        // - & 0x0F: Mask to ensure the result is within the range 0-15
        // - ctx->table[...]: Look up the corresponding value in the precomputed
        // table
        // - ^ (crc << 4): XOR the table value with the left-shifted CRC value

        // Update the CRC value using the lookup table (Step 2: Process the low
        // 4 bits)
        crc = ctx->table[(((data << 4) ^ crc) >> 4) & 0x0F] ^ (crc << 4);
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
        crc = reverse_bits(crc);
    }

    ctx->init = crc ^ ctx->xor_out;
}

uint8_t crc8_lookup_final(crc8_lookup_ctx_t* ctx) {
    return ctx->init; // Return the final CRC value stored in the context
}

uint8_t crc8_lookup_calculate(crc8_lookup_param_model_e model,
                              const uint8_t* buf, uint32_t len) {
    if (buf == NULL || len == 0) {
        return 0;
    }

    crc8_lookup_ctx_t ctx; // Declare the CRC8 context structure

    // Initialize the CRC8 context based on the specified model
    crc8_lookup_init(&ctx, model);

    // Update the CRC8 calculation with the input data buffer
    crc8_lookup_update(&ctx, buf, len);

    // Retrieve and return the final CRC8 checksum value
    return crc8_lookup_final(&ctx);
}

void crc8_lookup_pack_buf(crc8_lookup_param_model_e model, uint8_t* buf,
                          uint32_t len) {
    if (buf == NULL || len <= sizeof(uint8_t)) {
        return; // Not enough space for CRC
    }

    // Calculate the CRC for the data portion (excluding the last byte)
    uint8_t crc = crc8_lookup_calculate(model, buf, len - sizeof(uint8_t));
    // Store the CRC value at the end of the buffer
    buf[len - sizeof(uint8_t)] = crc;
}

bool crc8_lookup_verify_buf(crc8_lookup_param_model_e model, const uint8_t* buf,
                            uint32_t len) {
    // Ensure there is enough space for the CRC value
    if (buf == NULL || len <= sizeof(uint8_t)) {
        return false; // Not enough space for CRC
    }

    // Extract the stored CRC value from the end of the buffer
    uint8_t stored_crc = buf[len - sizeof(uint8_t)];

    // Calculate the CRC for the data portion (excluding the last byte)
    uint8_t calculated_crc = crc8_lookup_calculate(model, buf,
                                                   len - sizeof(uint8_t));

    // Compare the stored CRC with the calculated CRC
    return (stored_crc == calculated_crc);
}

void crc8_generate_table(uint8_t polynomial, uint8_t table[],
                         uint32_t table_len) {
    // Iterate over all possible byte values (0 to 255)
    for (uint32_t i = 0; i < table_len; i++) {
        uint8_t crc = 0; // Initialize the CRC value to 0

        // XOR the current byte (i) with the initial CRC value
        crc ^= (uint8_t)i;

        // Perform the CRC calculation for each bit in the byte
        for (uint8_t j = 0; j < 8; j++) {
            // Check if the most significant bit (MSB) is set
            if (crc & 0x80) {
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
