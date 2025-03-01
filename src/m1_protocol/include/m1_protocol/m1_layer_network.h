/**
 * \file            m1_layer_network.h
 * \brief           Header file for the M1 network layer.
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
#ifndef __M1_LAYER_NETWORK_H__
#define __M1_LAYER_NETWORK_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_format_packet.h" /*!< Defines packet structures and attributes for the M1 protocol. */
#include "./m1_protocol/m1_typedef.h" /*!< General type definitions for the M1 protocol. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1_layer_network_manager
 * \brief           M1 protocol network layer manager.
 * \{
 */

/**
 * \brief           Receives and processes frames at the network layer.
 *
 * This function processes a frame received from the lower layers and converts
 * it into a higher-level representation according to the M1 protocol.
 *
 * \param[in]       frame_buf: Pointer to the buffer containing the received
 * frame.
 * \param[in]       frame_len: Length of the received frame in bytes.
 * \return          An error type indicating the status of the frame processing.
 *                   - `ETYPE_OK` if the processing is successful.
 *                   - An appropriate error code otherwise.
 */
etype_e m1_network_receive(u8* frame_buf, size_t frame_len);

/**
 * \brief           Sends a packet through the network layer.
 *
 * This function is responsible for transmitting a packet over the network
 * layer. It ensures that the packet is properly prepared and optionally adds a
 * sequence number.
 *
 * \param[in]       packet: Pointer to the packet structure to be sent.
 * \param[in]       add_seq_num: Boolean flag indicating whether to add a
 * sequence number.
 * \return          An error type indicating the status of the transmission.
 *                   - `ETYPE_OK` if the transmission is successful.
 *                   - An appropriate error code otherwise.
 */
etype_e m1_network_send(m1_packet_t* packet, bool add_seq_num);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_LAYER_NETWORK_H__ */

/* ----------------------------- end of file -------------------------------- */