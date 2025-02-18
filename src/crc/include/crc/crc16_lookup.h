/**
 * \file            crc16_lookup.h
 * \brief           Cyclic Redundancy Check (CRC16) Look-Up Table
 * \date            2025-02-14
 *
 * This file provides functions for CRC16 calculation using various models
 * such as IBM CRC16, Maxim CRC16, USB CRC16, Modbus CRC16, and others. It
 * includes functions for initializing the CRC16 context, updating CRC16
 * checksums, finalizing the checksum, and verifying data integrity using a
 * lookup table.
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
#ifndef __CRC16_LOOKUP_H__
#define __CRC16_LOOKUP_H__

/* includes ----------------------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        crc16_lookup_manager CRC16 Lookup Manager
 * \brief           Manages CRC16 checksum calculation using lookup tables.
 * \{
 */

/* Public typedefs ---------------------------------------------------------- */
/**
 * \brief           Enumeration of available CRC16 lookup models.
 *
 * This enum defines the different CRC16 lookup models supported by this
 * library.
 */
typedef enum {
    CRC16_IBM_LOOKUP_MODEL = 0,     /*!< IBM CRC16 model */
    CRC16_MAXIM_LOOKUP_MODEL,       /*!< Maxim CRC16 model */
    CRC16_USB_LOOKUP_MODEL,         /*!< USB CRC16 model */
    CRC16_MODBUS_LOOKUP_MODEL,      /*!< Modbus CRC16 model */
    CRC16_CCITT_LOOKUP_MODEL,       /*!< CCITT CRC16 model */
    CRC16_CCITT_FALSE_LOOKUP_MODEL, /*!< CCITT False CRC16 model */
    CRC16_X25_LOOKUP_MODEL,         /*!< X25 CRC16 model */
    CRC16_XMODEM_LOOKUP_MODEL,      /*!< XMODEM CRC16 model */
    CRC16_DNP_LOOKUP_MODEL,         /*!< DNP CRC16 model */
    CRC16_NONE_LOOKUP_MODEL         /*!< No specific CRC16 model */
} crc16_lookup_param_model_e;

/**
 * \brief           CRC16 parameter model structure for lookup table-based
 *                  calculation.
 *
 * This structure holds the parameters needed for CRC16 lookup table-based
 * calculation. It defines the initial value, final XOR value, polynomial,
 * bit reversal flags, table length, and the lookup table itself.
 * http://www.ip33.com/crc.html
 */
typedef struct {
    uint16_t init;      /*!< Initial value for the CRC16 calculation */
    uint16_t xor_out;   /*!< Final XOR value to apply to the result */
    uint16_t poly;      /*!< Polynomial used in CRC16 calculation */
    bool ref_in;        /*!< Whether to reverse the input data bits */
    bool ref_out;       /*!< Whether to reverse the output data bits */
    uint16_t table_len; /*!< Length of the CRC16 lookup table */
    uint16_t* table;    /*!< Pointer to the CRC16 lookup table */
} crc16_lookup_ctx_t;

/* Public functions --------------------------------------------------------- */
/**
 * \brief           Initialize the CRC16 context with the specified model.
 *
 * This function initializes the CRC16 context based on the specified CRC16
 * model. It sets the initial CRC value, XOR value, polynomial, bit reversal
 * options, and prepares the lookup table.
 *
 * \param[in,out]   ctx: Pointer to the CRC16 context structure to be
 *                  initialized.
 * \param[in]       model: The CRC16 model to be used for initialization.
 */
void crc16_lookup_init(crc16_lookup_ctx_t* ctx,
                       crc16_lookup_param_model_e model);

/**
 * \brief           Update the CRC16 calculation with new data.
 *
 * This function processes the input data buffer and updates the CRC16 checksum
 * stored in the context using the CRC16 lookup table and parameters set by
 * `crc16_lookup_init`.
 *
 * \param[in,out]   ctx:Pointer to the CRC16 context structure containing the
 *                  current state.
 * \param[in]       buf: Pointer to the input data buffer to process.
 * \param[in]       len: Length of the input data buffer in bytes.
 */
void crc16_lookup_update(crc16_lookup_ctx_t* ctx, const uint8_t* buf,
                         uint32_t len);

/**
 * \brief           Finalize the CRC16 calculation and return the checksum.
 *
 * This function completes the CRC16 calculation and returns the final checksum
 * based on the current context. It should be called after all data has been
 * processed using `crc16_lookup_update`.
 *
 * \param[in]       ctx: Pointer to the CRC16 context structure.
 * \return          The final CRC16 checksum.
 */
uint16_t crc16_lookup_final(crc16_lookup_ctx_t* ctx);

/**
 * \brief           Calculate the CRC16 checksum for a given data buffer using a
 *                  specific model.
 *
 * This function calculates the CRC16 checksum for the provided data buffer and
 * CRC16 model, returning the calculated checksum. It does not require prior
 * initialization.
 *
 * \param[in]       model: The CRC16 model to use for calculation.
 * \param[in]       buf: Pointer to the data buffer to process.
 * \param[in]       len: Length of the data buffer in bytes.
 * \return          The calculated CRC16 checksum.
 */
uint16_t crc16_lookup_calculate(crc16_lookup_param_model_e model,
                                const uint8_t* buf, uint32_t len);

/**
 * \brief           Pack the CRC16 parameters and lookup table into a buffer.
 *
 * This function packs the CRC16 model parameters and the lookup table (if
 * applicable) into the provided buffer. It can be used to serialize the CRC16
 * model configuration.
 *
 * \param[in]       model: The CRC16 model whose configuration is to be packed.
 * \param[out]      buf: Pointer to the buffer where the packed data will be
 *                  stored.
 * \param[in]       len: Length of the buffer.
 */
void crc16_lookup_pack_buf(crc16_lookup_param_model_e model, uint8_t* buf,
                           uint32_t len);

/**
 * \brief           Verify the CRC16 checksum of a given data buffer.
 *
 * This function verifies the CRC16 checksum of the data buffer using the
 * specified CRC16 model. It checks whether the calculated checksum matches the
 * expected value.
 *
 * \param[in]       model: The CRC16 model to use for verification.
 * \param[in]       buf: Pointer to the data buffer to verify.
 * \param[in]       len: Length of the data buffer in bytes.
 * \return          True if the checksum is valid, false otherwise.
 */
bool crc16_lookup_verify_buf(crc16_lookup_param_model_e model,
                             const uint8_t* buf, uint32_t len);

/**
 * \brief           Generate the CRC16 lookup table for a given polynomial.
 *
 * This function generates a CRC16 lookup table based on the provided polynomial
 * and stores it in the provided table array. The table can be used for fast
 * CRC16 calculation.
 *
 * \param[in]       polynomial: The polynomial to use for the CRC16 calculation.
 * \param[out]      table: Pointer to the lookup table to be generated.
 * \param[in]       table_len: Length of the lookup table.
 */
void crc16_generate_table(uint16_t polynomial, uint16_t table[],
                          uint32_t table_len);
/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CRC16_LOOKUP_H__ */

/* ----------------------------- end of file -------------------------------- */
