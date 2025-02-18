/**
 * \file            crc16.c
 * \brief           Cyclic Redundancy Check (CRC16)
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
 * This file is part of the crc16 library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         v0.0.1
 */
/* includes ----------------------------------------------------------------- */
#include "crc/crc16.h"
#include "crc/bit_utils.h"

/* Public functions --------------------------------------------------------- */
void crc16_init(crc16_ctx_t* ctx, crc16_param_model_e model) {
    switch (model) {
    case CRC16_IBM_MODEL:      // CRC-16 IBM
        ctx->init = 0x0000;    // Initial value
        ctx->poly = 0x8005;    // Polynomial (x^16 + x^15 + x^2 + 1)
        ctx->xor_out = 0x0000; // Final XOR value
        ctx->ref_in = true;    // Reverse input bits
        ctx->ref_out = true;   // Reverse output bits
        break;

    case CRC16_MAXIM_MODEL:    // CRC-16 MAXIM/Dallas
        ctx->init = 0x0000;    // Initial value
        ctx->poly = 0x8005;    // Polynomial (x^16 + x^15 + x^2 + 1)
        ctx->xor_out = 0xFFFF; // Final XOR value
        ctx->ref_in = true;    // Reverse input bits
        ctx->ref_out = true;   // Reverse output bits
        break;

    case CRC16_USB_MODEL:      // CRC-16 USB
        ctx->init = 0xFFFF;    // Initial value
        ctx->poly = 0x8005;    // Polynomial (x^16 + x^15 + x^2 + 1)
        ctx->xor_out = 0xFFFF; // Final XOR value
        ctx->ref_in = true;    // Reverse input bits
        ctx->ref_out = true;   // Reverse output bits
        break;

    case CRC16_MODBUS_MODEL:   // CRC-16 MODBUS
        ctx->init = 0xFFFF;    // Initial value
        ctx->poly = 0x8005;    // Polynomial (x^16 + x^15 + x^2 + 1)
        ctx->xor_out = 0x0000; // Final XOR value
        ctx->ref_in = true;    // Reverse input bits
        ctx->ref_out = true;   // Reverse output bits
        break;

    case CRC16_CCITT_MODEL:    // CRC-16 CCITT (0x0000)
        ctx->init = 0x0000;    // Initial value
        ctx->poly = 0x1021;    // Polynomial (x^16 + x^12 + x^5 + 1)
        ctx->xor_out = 0x0000; // Final XOR value
        ctx->ref_in = true;    // Do not reverse input bits
        ctx->ref_out = true;   // Do not reverse output bits
        break;

    case CRC16_CCITT_FALSE_MODEL: // CRC-16 CCITT_FALSE (0xFFFF)
        ctx->init = 0xFFFF;       // Initial value
        ctx->poly = 0x1021;       // Polynomial (x^16 + x^12 + x^5 + 1)
        ctx->xor_out = 0x0000;    // Final XOR value
        ctx->ref_in = false;      // Do not reverse input bits
        ctx->ref_out = false;     // Do not reverse output bits
        break;

    case CRC16_X25_MODEL:      // CRC-16 X25
        ctx->init = 0xFFFF;    // Initial value
        ctx->poly = 0x1021;    // Polynomial (x^16 + x^12 + x^5 + 1)
        ctx->xor_out = 0xFFFF; // Final XOR value
        ctx->ref_in = true;    // Reverse input bits
        ctx->ref_out = true;   // Reverse output bits
        break;

    case CRC16_XMODEM_MODEL:   // CRC-16 XMODEM
        ctx->init = 0x0000;    // Initial value
        ctx->poly = 0x1021;    // Polynomial (x^16 + x^12 + x^5 + 1)
        ctx->xor_out = 0x0000; // Final XOR value
        ctx->ref_in = false;   // Do not reverse input bits
        ctx->ref_out = false;  // Do not reverse output bits
        break;

    case CRC16_DNP_MODEL:      // CRC-16 DNP
        ctx->init = 0x0000;    // Initial value
        ctx->poly = 0x3D65;    // Polynomial (x^16 + x^13 + x^12 + x^11 + x^10 +
                               // x^8 + x^6 + x^5 + x^2 + 1)
        ctx->xor_out = 0xFFFF; // Final XOR value
        ctx->ref_in = true;    // Reverse input bits
        ctx->ref_out = true;   // Reverse output bits
        break;

    case CRC16_NONE_MODEL: // No CRC (dummy)
    default:
        ctx->init = 0x0000;    // Default initial value
        ctx->poly = 0x0000;    // Default polynomial (no operation)
        ctx->xor_out = 0x0000; // Default XOR value
        ctx->ref_in = false;   // Do not reverse input bits
        ctx->ref_out = false;  // Do not reverse output bits
        break;
    }
}

void crc16_update(crc16_ctx_t* ctx, const uint8_t* buf, uint32_t len) {
    uint16_t crc = ctx->init;

    for (uint32_t i = 0; i < len; i++) {
        uint16_t data = buf[i];

        if (ctx->ref_in) {
            data = reverse_bits((uint8_t)data);
        }

        crc ^= (data << 8);

        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ ctx->poly;
            } else {
                crc <<= 1;
            }
        }
    }

    if (ctx->ref_out) {
        crc = reverse_bits_16(crc);
    }

    ctx->init = crc ^ ctx->xor_out;
}

uint16_t crc16_final(crc16_ctx_t* ctx) {
    return ctx->init;
}

uint16_t crc16_calculate(crc16_param_model_e model, const uint8_t* buf,
                         uint32_t len) {
    crc16_ctx_t ctx; // Declare the CRC16 context structure

    // Initialize the CRC16 context based on the specified model
    crc16_init(&ctx, model);

    // Update the CRC16 calculation with the input data buffer
    crc16_update(&ctx, buf, len);

    // Retrieve and return the final CRC16 checksum value
    return crc16_final(&ctx);
}

void crc16_pack_buf(crc16_param_model_e model, uint8_t* buf, uint32_t len) {
    if (buf == NULL || len <= sizeof(uint16_t)) {
        return; // Not enough space for CRC
    }
    uint16_t crc16 = crc16_calculate(model, buf, len - sizeof(uint16_t));

    *(buf + len - 2) = crc16 & 0xFF;
    *(buf + len - 1) = (crc16 >> 8) & 0xFF;
}

bool crc16_verify_buf(crc16_param_model_e model, const uint8_t* buf,
                      uint32_t len) {
    if (buf == NULL || len <= sizeof(uint16_t)) {
        return false; // Not enough space for CRC
    }

    uint16_t stored_crc = (*(buf + len - 1) << 8) | (*(buf + len - 2));
    uint16_t calculated_crc = crc16_calculate(model, buf,
                                              len - sizeof(uint16_t));
    return (stored_crc == calculated_crc);
}

/* ----------------------------- end of file -------------------------------- */
