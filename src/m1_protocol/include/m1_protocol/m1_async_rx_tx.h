/**
 * \file            m1_async_rx_tx.h
 * \brief           Header file for asynchronous RX (receive) and TX (transmit)
 *                  operations in M1 protocol.
 * \date            2024-12-12
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
 * This file is part of the m1 protocol library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         V0.0.1
 */
#ifndef __M1_ASYNC_RX_TX_H__
#define __M1_ASYNC_RX_TX_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_typedef.h" /*!< Includes the M1 protocol type definitions. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1 async rx tx manager
 * \brief           M1 protocol asynchronous RX and TX management module.
 * \{
 */

/**
 * \brief           Structure for managing asynchronous RX operations.
 *
 * This structure defines a function pointer for receiving data asynchronously.
 * The user is expected to implement the \ref rx function to handle RX logic.
 */
typedef struct rx_async {
    /**
     * \brief       Function to perform asynchronous data reception.
     * \param[out]  buf: Pointer to the buffer where received data will be
     *              stored.
     * \param[out]  len: Pointer to a variable to store the length of received
     *              data.
     * \return      Status of the RX operation as an \ref etype_e value.
     *
     * The implementation of this function should:
     *   - Handle non-blocking data reception.
     *   - Ensure `buf` and `len` are updated with valid data and length.
     *   - Return an appropriate error code if the operation fails.
     */
    etype_e (*rx)(u8* buf, size_t* len);
} rx_async_t;

/**
 * \brief           Structure for managing asynchronous TX operations.
 *
 * This structure defines function pointers for transmitting data asynchronously
 * and retrieving the current state of the TX module.
 */
typedef struct tx_async {
    /**
     * \brief       Function to perform asynchronous data transmission.
     * \param[in]   buf: Pointer to the buffer containing data to be
     *              transmitted.
     * \param[in]   len: Length of the data to be transmitted.
     * \return      Status of the TX operation as an \ref etype_e value.
     *
     * The implementation of this function should:
     *   - Handle non-blocking data transmission.
     *   - Validate input parameters (`buf` and `len`).
     *   - Return an appropriate error code if the operation fails.
     */
    etype_e (*tx)(u8* buf, size_t len);

    /**
     * \brief       Function to get the current state of the TX module.
     * \return      Current state of the TX module as an \ref etype_e value.
     *
     * The implementation of this function should:
     *   - Provide the current operational state (e.g., idle, busy, error).
     *   - Return an appropriate state code as defined in \ref etype_e.
     */
    etype_e (*get_state)(void);
} tx_async_t;

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_ASYNC_RX_TX_H__ */

/* ----------------------------- end of file -------------------------------- */
