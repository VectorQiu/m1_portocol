/**
 * \file            crc8.c
 * \brief           Cyclic Redundancy Check (CRC8)
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
 * This file is part of the crc8 library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         v0.0.1
 */
/* includes ----------------------------------------------------------------- */
#include "crc/crc8.h"
#include "crc/bit_utils.h"

/* Public functions --------------------------------------------------------- */
void crc8_init(crc8_ctx_t* ctx, crc8_param_model_e model) {
    switch (model) {
    case CRC8_MODEL:          // Standard CRC-8
        ctx->init = 0x00;     // Initial value
        ctx->poly = 0x07;     // Polynomial (x^8 + x^2 + x^1 + 1)
        ctx->xor_out = 0x00;  // Final XOR value
        ctx->ref_in = false;  // Do not reverse input bits
        ctx->ref_out = false; // Do not reverse output bits
        break;
    case CRC8_ITU_MODEL:      // CRC-8 ITU
        ctx->init = 0x00;     // Initial value
        ctx->poly = 0x07;     // Polynomial (x^8 + x^2 + x^1 + 1)
        ctx->xor_out = 0x55;  // Final XOR value
        ctx->ref_in = false;  // Do not reverse input bits
        ctx->ref_out = false; // Do not reverse output bits
        break;
    case CRC8_ROHC_MODEL:    // CRC-8 ROHC
        ctx->init = 0xFF;    // Initial value
        ctx->poly = 0x07;    // Polynomial (x^8 + x^2 + x^1 + 1)
        ctx->xor_out = 0x00; // Final XOR value
        ctx->ref_in = true;  // Reverse input bits
        ctx->ref_out = true; // Reverse output bits
        break;
    case CRC8_MAXIM_MODEL:   // CRC-8 Maxim/Dallas
        ctx->init = 0x00;    // Initial value
        ctx->poly = 0x31;    // Polynomial (x^8 + x^5 + x^4 + 1)
        ctx->xor_out = 0x00; // Final XOR value
        ctx->ref_in = true;  // Reverse input bits
        ctx->ref_out = true; // Reverse output bits
        break;
    case CRC8_NONE_MODEL: // No CRC (dummy)
    default:
        ctx->init = 0x00;     // Default initial value
        ctx->poly = 0x00;     // Default polynomial (no operation)
        ctx->xor_out = 0x00;  // Default XOR value
        ctx->ref_in = false;  // Do not reverse input bits
        ctx->ref_out = false; // Do not reverse output bits
        break;
    }
}

void crc8_update(crc8_ctx_t* ctx, const uint8_t* buf, uint32_t len) {
    // Start with the current CRC value from the context
    uint8_t crc = ctx->init;

    for (uint32_t i = 0; i < len; i++) {
        uint8_t data = buf[i];

        // Reverse input bits if required by the model
        if (ctx->ref_in) {
            data = reverse_bits(data);
        }

        // XOR the current byte with the CRC value
        crc ^= data;

        // Perform the CRC calculation for each bit in the byte
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) { // Check if the most significant bit is set
                crc = (crc << 1) ^ ctx->poly; // Apply the polynomial
            } else {
                crc <<= 1; // Shift left if no polynomial application is needed
            }
        }
    }

    // Reverse output bits if required by the model
    if (ctx->ref_out) {
        crc = reverse_bits(crc);
    }

    // Apply the final XOR value and store the result back in the context
    ctx->init = crc ^ ctx->xor_out;
}

uint8_t crc8_final(crc8_ctx_t* ctx) {
    return ctx->init; // Return the final CRC value stored in the context
}

uint8_t crc8_calculate(crc8_param_model_e model, const uint8_t* buf,
                       uint32_t len) {
    if (buf == NULL || len == 0) {
        return 0;
    }

    crc8_ctx_t ctx; // Declare the CRC8 context structure

    // Initialize the CRC8 context based on the specified model
    crc8_init(&ctx, model);

    // Update the CRC8 calculation with the input data buffer
    crc8_update(&ctx, buf, len);

    // Retrieve and return the final CRC8 checksum value
    return crc8_final(&ctx);
}

void crc8_pack_buf(crc8_param_model_e model, uint8_t* buf, uint32_t len) {
    if (buf == NULL || len <= sizeof(uint8_t)) {
        return; // Not enough space for CRC
    }

    // Calculate the CRC for the data portion (excluding the last byte)
    uint8_t crc = crc8_calculate(model, buf, len - sizeof(uint8_t));
    // Store the CRC value at the end of the buffer
    buf[len - sizeof(uint8_t)] = crc;
}

bool crc8_verify_buf(crc8_param_model_e model, const uint8_t* buf,
                     uint32_t len) {
    // Ensure there is enough space for the CRC value
    if (buf == NULL || len <= sizeof(uint8_t)) {
        return false; // Not enough space for CRC
    }

    // Extract the stored CRC value from the end of the buffer
    uint8_t stored_crc = buf[len - sizeof(uint8_t)];

    // Calculate the CRC for the data portion (excluding the last byte)
    uint8_t calculated_crc = crc8_calculate(model, buf, len - sizeof(uint8_t));

    // Compare the stored CRC with the calculated CRC
    return (stored_crc == calculated_crc);
}

/* ----------------------------- end of file -------------------------------- */
