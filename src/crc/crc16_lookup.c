/**
 * \file            crc16_lookup_lookup.c
 * \brief           Cyclic Redundancy Check (CRC16) Look-Up Table
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
#include "crc/crc16_lookup.h"
#include "crc/bit_utils.h"

/* Private variables -------------------------------------------------------- */
/**
 * \brief           polynomial: 0x8005
 *
 * Polynomial (x^16 + x^15 + x^2 + 1): 0x8005
 */
static const uint16_t crc16_poly_0x8005_table[16] = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
};

/**
 * \brief           polynomial: 0x1021
 *
 * Polynomial (x^16 + x^12 + x^5 + 1): 0x1021
 */
static const uint16_t crc16_poly_0x1021_table[16] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
};

/**
 * \brief           polynomial: 0x3D65
 *
 * Polynomial (x^16 + x^13 + x^12 + x^11 + x^10 + x^8 + x^6 + x^5 + x^2 + 1):
 * 0x3D65
 */
static const uint16_t crc16_poly_0x3D65_table[16] = {
    0x0000, 0x3D65, 0x7ACA, 0x47AF, 0xF594, 0xC8F1, 0x8F5E, 0xB23B,
    0xD64D, 0xEB28, 0xAC87, 0x91E2, 0x23D9, 0x1EBC, 0x5913, 0x6476,
};

/* Public functions --------------------------------------------------------- */
void crc16_lookup_init(crc16_lookup_ctx_t* ctx,
                       crc16_lookup_param_model_e model) {
    switch (model) {
    case CRC16_IBM_LOOKUP_MODEL: // CRC-16 IBM
        ctx->init = 0x0000;      // Initial value
        ctx->poly = 0x8005;      // Polynomial (x^16 + x^15 + x^2 + 1)
        ctx->xor_out = 0x0000;   // Final XOR value
        ctx->ref_in = true;      // Reverse input bits
        ctx->ref_out = true;     // Reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint16_t*)crc16_poly_0x8005_table;
        break;

    case CRC16_MAXIM_LOOKUP_MODEL: // CRC-16 MAXIM/Dallas
        ctx->init = 0x0000;        // Initial value
        ctx->poly = 0x8005;        // Polynomial (x^16 + x^15 + x^2 + 1)
        ctx->xor_out = 0xFFFF;     // Final XOR value
        ctx->ref_in = true;        // Reverse input bits
        ctx->ref_out = true;       // Reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint16_t*)crc16_poly_0x8005_table;
        break;

    case CRC16_USB_LOOKUP_MODEL: // CRC-16 USB
        ctx->init = 0xFFFF;      // Initial value
        ctx->poly = 0x8005;      // Polynomial (x^16 + x^15 + x^2 + 1)
        ctx->xor_out = 0xFFFF;   // Final XOR value
        ctx->ref_in = true;      // Reverse input bits
        ctx->ref_out = true;     // Reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint16_t*)crc16_poly_0x8005_table;
        break;

    case CRC16_MODBUS_LOOKUP_MODEL: // CRC-16 MODBUS
        ctx->init = 0xFFFF;         // Initial value
        ctx->poly = 0x8005;         // Polynomial (x^16 + x^15 + x^2 + 1)
        ctx->xor_out = 0x0000;      // Final XOR value
        ctx->ref_in = true;         // Reverse input bits
        ctx->ref_out = true;        // Reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint16_t*)crc16_poly_0x8005_table;
        break;

    case CRC16_CCITT_LOOKUP_MODEL: // CRC-16 CCITT (0x0000)
        ctx->init = 0x0000;        // Initial value
        ctx->poly = 0x1021;        // Polynomial (x^16 + x^12 + x^5 + 1)
        ctx->xor_out = 0x0000;     // Final XOR value
        ctx->ref_in = true;        // Do not reverse input bits
        ctx->ref_out = true;       // Do not reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint16_t*)crc16_poly_0x1021_table;
        break;

    case CRC16_CCITT_FALSE_LOOKUP_MODEL: // CRC-16 CCITT_FALSE (0xFFFF)
        ctx->init = 0xFFFF;              // Initial value
        ctx->poly = 0x1021;              // Polynomial (x^16 + x^12 + x^5 + 1)
        ctx->xor_out = 0x0000;           // Final XOR value
        ctx->ref_in = false;             // Do not reverse input bits
        ctx->ref_out = false;            // Do not reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint16_t*)crc16_poly_0x1021_table;
        break;

    case CRC16_X25_LOOKUP_MODEL: // CRC-16 X25
        ctx->init = 0xFFFF;      // Initial value
        ctx->poly = 0x1021;      // Polynomial (x^16 + x^12 + x^5 + 1)
        ctx->xor_out = 0xFFFF;   // Final XOR value
        ctx->ref_in = true;      // Reverse input bits
        ctx->ref_out = true;     // Reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint16_t*)crc16_poly_0x1021_table;
        break;

    case CRC16_XMODEM_LOOKUP_MODEL: // CRC-16 XMODEM
        ctx->init = 0x0000;         // Initial value
        ctx->poly = 0x1021;         // Polynomial (x^16 + x^12 + x^5 + 1)
        ctx->xor_out = 0x0000;      // Final XOR value
        ctx->ref_in = false;        // Do not reverse input bits
        ctx->ref_out = false;       // Do not reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint16_t*)crc16_poly_0x1021_table;
        break;

    case CRC16_DNP_LOOKUP_MODEL: // CRC-16 DNP
        ctx->init = 0x0000;      // Initial value
        ctx->poly = 0x3D65;    // Polynomial (x^16 + x^13 + x^12 + x^11 + x^10 +
                               // x^8 + x^6 + x^5 + x^2 + 1)
        ctx->xor_out = 0xFFFF; // Final XOR value
        ctx->ref_in = true;    // Reverse input bits
        ctx->ref_out = true;   // Reverse output bits
        ctx->table_len = 16;
        ctx->table = (uint16_t*)crc16_poly_0x3D65_table;
        break;

    case CRC16_NONE_LOOKUP_MODEL: // No CRC (dummy)
    default:
        ctx->init = 0x0000;    // Default initial value
        ctx->poly = 0x0000;    // Default polynomial (no operation)
        ctx->xor_out = 0x0000; // Default XOR value
        ctx->ref_in = false;   // Do not reverse input bits
        ctx->ref_out = false;  // Do not reverse output bits
        ctx->table_len = 0;
        ctx->table = NULL;
        break;
    }
}

void crc16_lookup_update(crc16_lookup_ctx_t* ctx, const uint8_t* buf,
                         uint32_t len) {
    if (ctx->table == NULL || ctx->table_len < 16) {
        return;
    }

    uint16_t crc = ctx->init;
    for (uint32_t i = 0; i < len; i++) {
        uint8_t data = buf[i];

        // Reverse input bits if required by the model
        if (ctx->ref_in) {
            data = reverse_bits(data);
        }

        uint16_t temp = data;
        // Update the CRC value using the lookup table (Step 1: Process the high
        // 4 bits)
        crc = ctx->table[(((temp << 8) ^ crc) >> 12) & 0x0F] ^ (crc << 4);
        // Explanation:
        // - (data ^ crc): XOR the current byte with the current CRC value
        // - >> 4: Shift right by 4 bits to extract the high 4 bits
        // - & 0x0F: Mask to ensure the result is within the range 0-15
        // - ctx->table[...]: Look up the corresponding value in the precomputed
        // table
        // - ^ (crc << 4): XOR the table value with the left-shifted CRC value

        // Update the CRC value using the lookup table (Step 2: Process the low
        // 4 bits)
        crc = ctx->table[(((temp << 12) ^ crc) >> 12) & 0x0F] ^ (crc << 4);
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
        crc = reverse_bits_16(crc);
    }

    ctx->init = crc ^ ctx->xor_out;
}

uint16_t crc16_lookup_final(crc16_lookup_ctx_t* ctx) {
    return ctx->init;
}

uint16_t crc16_lookup_calculate(crc16_lookup_param_model_e model,
                                const uint8_t* buf, uint32_t len) {
    crc16_lookup_ctx_t ctx; // Declare the CRC16 context structure

    // Initialize the CRC16 context based on the specified model
    crc16_lookup_init(&ctx, model);

    // Update the CRC16 calculation with the input data buffer
    crc16_lookup_update(&ctx, buf, len);

    // Retrieve and return the final CRC16 checksum value
    return crc16_lookup_final(&ctx);
}

void crc16_lookup_pack_buf(crc16_lookup_param_model_e model, uint8_t* buf,
                           uint32_t len) {
    if (buf == NULL || len <= sizeof(uint16_t)) {
        return; // Not enough space for CRC
    }
    uint16_t crc16_lookup = crc16_lookup_calculate(model, buf,
                                                   len - sizeof(uint16_t));

    *(buf + len - 2) = crc16_lookup & 0xFF;
    *(buf + len - 1) = (crc16_lookup >> 8) & 0xFF;
}

bool crc16_lookup_verify_buf(crc16_lookup_param_model_e model,
                             const uint8_t* buf, uint32_t len) {
    if (buf == NULL || len <= sizeof(uint16_t)) {
        return false; // Not enough space for CRC
    }

    uint16_t stored_crc = (*(buf + len - 1) << 8) | (*(buf + len - 2));
    uint16_t calculated_crc = crc16_lookup_calculate(model, buf,
                                                     len - sizeof(uint16_t));
    return (stored_crc == calculated_crc);
}

void crc16_generate_table(uint16_t polynomial, uint16_t table[],
                          uint32_t table_len) {
    // Iterate over all possible byte values (0 to 255)
    for (uint32_t i = 0; i < table_len; i++) {
        uint16_t crc = 0; // Initialize the CRC value to 0
        uint16_t data = (uint16_t)i;

        // XOR the current byte (i) with the initial CRC value
        crc ^= (data << 8);

        // Perform the CRC calculation for each bit in the byte
        for (uint8_t j = 0; j < 8; j++) {
            // Check if the most significant bit (MSB) is set
            if (crc & 0x8000) {
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
