/**
 * \file            crc8.h
 * \brief           Cyclic Redundancy Check (CRC8) calculation and management
 * \date            2025-02-14
 *
 * This file provides functions for CRC8 calculation using different models
 * such as standard CRC8, ITU CRC8, ROHC CRC8, and Maxim CRC8. It includes
 * functions for initializing the CRC8 context, updating CRC8 checksums,
 * finalizing the checksum, and verifying data integrity.
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
#ifndef __CRC8_H__
#define __CRC8_H__

/* includes ----------------------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        crc8_manager CRC8 Manager
 * \brief           Manages CRC8 calculations and provides functions for CRC8
 *                  checksum verification, initialization, and updating.
 * \{
 */

/* Public typedefs ---------------------------------------------------------- */

/**
 * \brief           Enumeration of available CRC8 models.
 *
 * This enum defines the different CRC8 models supported by this library.
 */
typedef enum {
    CRC8_MODEL = 0,   /*!< Standard CRC8 model */
    CRC8_ITU_MODEL,   /*!< ITU CRC8 model */
    CRC8_ROHC_MODEL,  /*!< ROHC CRC8 model */
    CRC8_MAXIM_MODEL, /*!< Maxim CRC8 model */
    CRC8_NONE_MODEL   /*!< No specific CRC8 model */
} crc8_param_model_e;

/**
 * \brief           CRC8 parameter model structure.
 *
 * This structure holds the parameters needed for CRC8 calculation. It defines
 * the initial value, final XOR value, polynomial, and flags for input and
 * output bit reversal. These parameters are used to configure the CRC8
 * calculation model.
 * http://www.ip33.com/crc.html
 */
typedef struct {
    uint8_t init;    /*!< Initial value for the CRC8 calculation */
    uint8_t xor_out; /*!< Final XOR value to apply to the result */
    uint8_t poly;    /*!< Polynomial used in CRC8 calculation */
    bool ref_in;     /*!< Whether to reverse the input data bits */
    bool ref_out;    /*!< Whether to reverse the output data bits */
} crc8_ctx_t;

/* Public functions --------------------------------------------------------- */
/**
 * \brief           Initializes the CRC8 context with the specified model.
 *
 * This function initializes the CRC8 context based on the specified CRC8 model.
 * It sets the initial CRC value, XOR value, polynomial, and bit reversal
 * options according to the selected model.
 *
 * \param[in,out]   ctx: Pointer to the CRC8 context structure to be
 *                  initialized.
 * \param[in]       model: The CRC8 model to be used for initialization.
 */
void crc8_init(crc8_ctx_t* ctx, crc8_param_model_e model);

/**
 * \brief           Update the CRC8 calculation with new data.
 *
 * This function processes the input data buffer and updates the CRC8 checksum
 * stored in the context. It applies the CRC8 algorithm using the parameters
 * initialized by `crc8_init`. Input data may be reversed if required by the
 * model.
 *
 * \param[in,out]   ctx: Pointer to the CRC8 context structure containing the
 *                        current state.
 * \param[in]       buf: Pointer to the input data buffer to process.
 * \param[in]       len: Length of the input data buffer in bytes.
 */
void crc8_update(crc8_ctx_t* ctx, const uint8_t* buf, uint32_t len);

/**
 * \brief           Finalize the CRC8 calculation and return the checksum.
 *
 * This function completes the CRC8 calculation and returns the final checksum
 * based on the current context. It should be called after all data has been
 * processed using `crc8_update`.
 *
 * \param[in]       ctx: Pointer to the CRC8 context structure.
 * \return          The final CRC8 checksum.
 */
uint8_t crc8_final(crc8_ctx_t* ctx);

/**
 * \brief           Calculate the CRC8 checksum for a given data buffer using a
 *                  specific model.
 *
 * This function calculates the CRC8 checksum for the provided data buffer and
 * CRC8 model, returning the calculated checksum. It does not require prior
 * initialization.
 *
 * \param[in]       model: The CRC8 model to use for calculation.
 * \param[in]       buf: Pointer to the data buffer to process.
 * \param[in]       len: Length of the data buffer in bytes.
 * \return          The calculated CRC8 checksum.
 */
uint8_t crc8_calculate(crc8_param_model_e model, const uint8_t* buf,
                       uint32_t len);

/**
 * \brief           Pack the CRC8 parameters and lookup table into a buffer.
 *
 * This function packs the CRC8 model parameters and the lookup table (if
 * applicable) into the provided buffer. It can be used to serialize the CRC8
 * model configuration.
 *
 * \param[in]       model: The CRC8 model whose configuration is to be packed.
 * \param[out]      buf: Pointer to the buffer where the packed data will be
 *                  stored.
 * \param[in]       len: Length of the buffer.
 */
void crc8_pack_buf(crc8_param_model_e model, uint8_t* buf, uint32_t len);

/**
 * \brief           Verify the CRC8 checksum of a given data buffer.
 *
 * This function verifies the CRC8 checksum of the data buffer using the
 * specified CRC8 model. It checks whether the calculated checksum matches the
 * expected value.
 *
 * \param[in]       model: The CRC8 model to use for verification.
 * \param[in]       buf: Pointer to the data buffer to verify.
 * \param[in]       len: Length of the data buffer in bytes.
 * \return          True if the checksum is valid, false otherwise.
 */
bool crc8_verify_buf(crc8_param_model_e model, const uint8_t* buf,
                     uint32_t len);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CRC8_H__ */

/* ----------------------------- end of file -------------------------------- */