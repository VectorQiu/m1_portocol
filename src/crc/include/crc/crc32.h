/**
 * \file            crc32.h
 * \brief           Cyclic Redundancy Check (CRC32)
 * \date            2025-02-14
 *
 * This file provides functions for CRC32 calculation, including initialization,
 * update, finalization, and verification using different CRC32 models.
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
#ifndef __CRC32_H__
#define __CRC32_H__

/* includes ----------------------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        crc32 manager
 * \brief           Manages CRC32 calculation and related operations.
 * \{
 */

/* Public typedefs ---------------------------------------------------------- */
/**
 * \brief           Enumeration of available CRC32 models.
 *
 * This enum defines the different CRC32 models supported by this library.
 */
typedef enum {
    CRC32_MODEL = 0,   /*!< Standard CRC32 model */
    CRC32_MPEG2_MODEL, /*!< MPEG2 CRC32 model */
    CRC32_NONE_MODEL,  /*!< No specific CRC32 model */
} crc32_param_model_e;

/**
 * \brief           CRC32 context structure for CRC32 calculation.
 *
 * This structure holds the parameters needed for CRC32 calculation, such as the
 * initial value, final XOR value, polynomial, and bit reversal options.
 */
typedef struct {
    uint32_t init;    /*!< Initial value for CRC32 calculation */
    uint32_t xor_out; /*!< Final XOR value to apply to the result */
    uint32_t poly;    /*!< Polynomial used for CRC32 calculation */
    bool ref_in;      /*!< Whether to reverse the input data bits */
    bool ref_out;     /*!< Whether to reverse the output data bits */
} crc32_ctx_t;

/* Public functions --------------------------------------------------------- */
/**
 * \brief           Initialize the CRC32 context with the specified model.
 *
 * This function initializes the CRC32 context based on the specified CRC32
 * model, setting up initial values and CRC parameters.
 *
 * \param[in,out]   ctx: Pointer to the CRC32 context structure to be
 *                  initialized
 * \param[in]       model: The CRC32 model to use for initialization
 */
void crc32_init(crc32_ctx_t* ctx, crc32_param_model_e model);

/**
 * \brief           Update the CRC32 checksum with new data.
 *
 * This function updates the CRC32 checksum in the provided context with the new
 * data from the input buffer.
 *
 * \param[in,out]   ctx: Pointer to the CRC32 context structure to update
 * \param[in]       buf: Pointer to the data buffer to process
 * \param[in]       len: Length of the data buffer in bytes
 */
void crc32_update(crc32_ctx_t* ctx, const uint8_t* buf, uint32_t len);

/**
 * \brief           Finalize the CRC32 calculation and return the checksum.
 *
 * This function completes the CRC32 calculation and returns the final checksum
 * stored in the provided context.
 *
 * \param[in]       ctx: Pointer to the CRC32 context structure
 * \return          The final CRC32 checksum
 */
uint32_t crc32_final(crc32_ctx_t* ctx);

/**
 * \brief           Calculate the CRC32 checksum for a given data buffer using a
 *                  specific model.
 *
 * This function calculates the CRC32 checksum for the provided data buffer
 * using the specified CRC32 model.
 *
 * \param[in]       model: The CRC32 model to use for calculation
 * \param[in]       buf: Pointer to the data buffer to process
 * \param[in]       len: Length of the data buffer in bytes
 * \return          The calculated CRC32 checksum
 */
uint32_t crc32_calculate(crc32_param_model_e model, const uint8_t* buf,
                         uint32_t len);

/**
 * \brief           Pack the CRC32 model parameters into a buffer.
 *
 * This function serializes the CRC32 model configuration into the provided
 * buffer.
 *
 * \param[in]       model: The CRC32 model whose parameters will be packed
 * \param[out]      buf: Pointer to the buffer where the packed data will be
 *                  stored
 * \param[in]       len: Length of the buffer
 */
void crc32_pack_buf(crc32_param_model_e model, uint8_t* buf, uint32_t len);

/**
 * \brief           Verify the CRC32 checksum of a data buffer.
 *
 * This function verifies whether the CRC32 checksum of the provided buffer is
 * correct according to the specified CRC32 model.
 *
 * \param[in]       model: The CRC32 model to use for verification
 * \param[in]       buf: Pointer to the data buffer to verify
 * \param[in]       len: Length of the data buffer in bytes
 * \return          `true` if the checksum is correct, `false` otherwise
 */
bool crc32_verify_buf(crc32_param_model_e model, const uint8_t* buf,
                      uint32_t len);
/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CRC32_H__ */

/* ----------------------------- end of file -------------------------------- */
