/**
 * \file            m1_layer_transport.h
 * \brief           M1 transport layer interface for data transmission and
 *                  reception.
 * \date            2024-12-11
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
#ifndef __M1_LAYER_TRANSPORT_H__
#define __M1_LAYER_TRANSPORT_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_format_data.h" /*!< Provides data structure definitions for M1 protocol. */
#include "./m1_protocol/m1_typedef.h" /*!< Includes common type definitions for the M1 protocol. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1_layer_transport_manager
 * \brief           M1 transport layer manager responsible for managing the
 *                  reliable transmission and reception of data.
 * \{
 */

/**
 * \brief           Executes periodic transport layer tasks.
 *
 * This function performs tasks such as retransmission handling, timeouts, and
 * state management. It should be called periodically by the application.
 *
 * \param[in]       freq: Frequency of the data reception process in Hz.
 */
void m1_transport_run(u32 freq);

/**
 * \brief           Receives data from the network layer.
 *
 * This function processes raw data received from the network layer, extracts
 * useful information, and prepares it for further processing at higher layers.
 *
 * \param[in]       frame_buf: Pointer to the buffer containing the raw
 *                  frame data.
 * \param[in]       frame_len: Length of the raw frame data.
 * \return          Returns an error code of type `etype_e` indicating the
 *                  success or failure of the operation.
 *                   - `ETYPE_OK` on success.
 *                   - Other error codes indicating specific failures.
 */
etype_e m1_transport_receive(u8* frame_buf, size_t frame_len);

/**
 * \brief           Sends data to the network layer.
 *
 * This function prepares the data for transmission by encapsulating it in the
 * appropriate transport layer structure and sends it to the network layer for
 * further processing.
 *
 * \param[in]       tx_data: Pointer to the data structure containing
 *                  information about the transmission.
 * \return          Returns an error code of type `etype_e` indicating the
 *                  success or failure of the operation.
 *                   - `ETYPE_OK` on success.
 *                   - Other error codes indicating specific failures.
 */
etype_e m1_transport_send(m1_tx_data_t* tx_data);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_LAYER_TRANSPORT_H__ */

/* ----------------------------- end of file -------------------------------- */