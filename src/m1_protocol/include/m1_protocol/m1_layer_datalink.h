/**
 * \file            m1_layer_datalink.h
 * \brief           Header file for the M1 data link layer.
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
#ifndef __M1_LAYER_DATALINK_H__
#define __M1_LAYER_DATALINK_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_format_packet.h" /*!< Defines packet structures and attributes for the M1 protocol. */
#include "./m1_protocol/m1_typedef.h" /*!< General type definitions for the M1 protocol. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1_layer_datalink_manager
 * \brief           M1 protocol data link layer manager.
 * \{
 */

/**
 * \brief           Sends a packet through the data link layer.
 *
 * This function is responsible for transmitting a packet over the data link
 * layer. It ensures that the packet is properly formatted and transmitted
 * according to the M1 protocol requirements.
 *
 * \param[in]       packet: Pointer to the packet structure to be sent.
 * \return          An error type indicating the status of the transmission.
 *                   - `E_STATE_OK` if the transmission is successful.
 *                   - An appropriate error code otherwise.
 */
etype_e m1_datalink_send(m1_packet_t* packet);

/**
 * \brief           Receives and processes packets at the data link layer.
 *
 * This function listens for incoming packets at the specified frequency and
 * processes them according to the M1 protocol.
 *
 * \param[in]       freq: Frequency of the data reception process in Hz.
 */
void m1_datalink_receive(u32 freq);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_LAYER_DATALINK_H__ */

/* ----------------------------- end of file -------------------------------- */