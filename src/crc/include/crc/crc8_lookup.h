/**
 * \file            crc8_lookup.h
 * \brief           Provides functions and lookup tables for CRC-8 (Cyclic
 *                  Redundancy Check) calculations.
 * \date            2025-02-14
 *
 * This file defines functions for initializing, updating, calculating, and
 * verifying CRC-8 checksums using lookup tables. Different CRC-8 models are
 * supported, including standard, ITU, ROHC, and Maxim models.
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
#ifndef __CRC8_LOOKUP_H__
#define __CRC8_LOOKUP_H__

/* includes ----------------------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        crc8_lookup_manager CRC8 Lookup Table Manager
 * \brief           Manages the CRC8 lookup table and provides functions for
 *                  CRC8 calculations.
 * \{
 */

/* Public typedefs ---------------------------------------------------------- */
/**
 * \brief           Enumeration of CRC-8 lookup models.
 *
 * This enumeration defines the different CRC-8 models available for use.
 */
typedef enum {
    CRC8_LOOKUP_MODEL = 0,   /*!< Standard CRC8 model */
    CRC8_ITU_LOOKUP_MODEL,   /*!< ITU CRC8 model */
    CRC8_ROHC_LOOKUP_MODEL,  /*!< ROHC CRC8 model */
    CRC8_MAXIM_LOOKUP_MODEL, /*!< Maxim CRC8 model */
    CRC8_NONE_LOOKUP_MODEL   /*!< No CRC8 model */
} crc8_lookup_param_model_e;

/**
 * \brief           CRC8 lookup context structure
 *
 * This structure holds the context for CRC8 lookup table calculation, including
 * model-specific parameters such as the initial value, polynomial, final XOR
 * value, input/output bit reversal, and the lookup table.
 * http://www.ip33.com/crc.html
 */
typedef struct {
    uint8_t init;    /*!< Initial value used for CRC8 calculation */
    uint8_t poly;    /*!< Polynomial used for CRC8 calculation */
    uint8_t xor_out; /*!< Final XOR value to be applied to the result */
    bool ref_in;  /*!< Flag indicating whether input data should be bit-reversed
                   */
    bool ref_out; /*!< Flag indicating whether output data should be
                     bit-reversed */
    uint16_t table_len; /*!< Length of the CRC8 lookup table */
    uint8_t* table;     /*!< Pointer to the lookup table for CRC8 calculation */
} crc8_lookup_ctx_t;

/* Public functions --------------------------------------------------------- */
/**
 * \brief           Initializes the CRC8 lookup context with the specified
 *                  model.
 *
 * This function initializes the CRC8 context based on the specified CRC8 model,
 * populating the context structure with the appropriate parameters.
 *
 * \param[in]       ctx: Pointer to the CRC8 lookup context structure to be
 *                  initialized.
 * \param[in]       model: The CRC8 model to be used for initialization.
 */
void crc8_lookup_init(crc8_lookup_ctx_t* ctx, crc8_lookup_param_model_e model);

/**
 * \brief           Updates the CRC8 checksum with additional data.
 *
 * This function updates the current CRC8 checksum using the provided data
 * buffer. It processes the data byte by byte, applying the CRC8 lookup table
 * algorithm.
 *
 * \param[in]       ctx: Pointer to the CRC8 lookup context.
 * \param[in]       buf: Pointer to the data buffer to be processed.
 * \param[in]       len: Length of the data buffer in bytes.
 */
void crc8_lookup_update(crc8_lookup_ctx_t* ctx, const uint8_t* buf,
                        uint32_t len);

/**
 * \brief           Finalizes the CRC8 checksum calculation.
 *
 * This function completes the CRC8 calculation and returns the final checksum.
 *
 * \param[in]       ctx: Pointer to the CRC8 lookup context.
 * \return          The final CRC8 checksum.
 */
uint8_t crc8_lookup_final(crc8_lookup_ctx_t* ctx);

/**
 * \brief       Calculates the CRC8 checksum for a given data buffer using a
 * specific model.
 *
 * This function calculates the CRC8 checksum for the specified data buffer and
 * CRC8 model. It does not require prior initialization.
 *
 * \param[in]       model: The CRC8 model to be used for calculation.
 * \param[in]       buf: Pointer to the data buffer to be processed.
 * \param[in]       len: Length of the data buffer in bytes.
 * \return          The calculated CRC8 checksum.
 */
uint8_t crc8_lookup_calculate(crc8_lookup_param_model_e model,
                              const uint8_t* buf, uint32_t len);

/**
 * \brief           Packs the CRC8 lookup table into a buffer.
 *
 * This function packs the lookup table for the specified CRC8 model into a data
 * buffer.
 *
 * \param[in]       model: The CRC8 model whose lookup table is to be packed.
 * \param[out]      buf: Pointer to the buffer to hold the packed lookup table.
 * \param[in]       len: Length of the buffer.
 */
void crc8_lookup_pack_buf(crc8_lookup_param_model_e model, uint8_t* buf,
                          uint32_t len);

/**
 * \brief           Verifies the CRC8 checksum of a given data buffer.
 *
 * This function verifies the CRC8 checksum of a data buffer using the specified
 * CRC8 model. It checks if the checksum in the buffer matches the expected
 * value.
 *
 * \param[in]       model: The CRC8 model to be used for verification.
 * \param[in]       buf: Pointer to the data buffer to be verified.
 * \param[in]       len: Length of the data buffer in bytes.
 * \return          True if the checksum is valid, false otherwise.
 */
bool crc8_lookup_verify_buf(crc8_lookup_param_model_e model, const uint8_t* buf,
                            uint32_t len);

/**
 * \brief           Generates the CRC8 lookup table for a given polynomial.
 *
 * This function generates a CRC8 lookup table based on the specified
 * polynomial. The table can then be used for fast CRC8 calculations.
 *
 * \param[in]       polynomial: The polynomial to be used for table generation.
 * \param[out]      table: Pointer to the table where the generated lookup
 *                  table will be stored.
 * \param[in]       table_len: The length of the table to be generated.
 */
void crc8_generate_table(uint8_t polynomial, uint8_t table[],
                         uint32_t table_len);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CRC8_LOOKUP_H__ */

/* ----------------------------- end of file -------------------------------- */
