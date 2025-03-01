/**
 * \file            m1_format_data.h
 * \brief           Header file for managing M1 protocol data formatting.
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
#ifndef __M1_FORMAT_DATA_H__
#define __M1_FORMAT_DATA_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_format_packet.h" /*!< Provides functions for packet formatting. */
#include "./m1_protocol/m1_typedef.h" /*!< Defines types used in the M1 protocol. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1_format_data_manager
 * \brief           M1 protocol data formatting manager.
 * \{
 */

/**
 * \brief           Structure for transmitting data in the M1 protocol.
 *
 * This structure is used to define the attributes and payload of a data frame
 * to be transmitted.
 */
typedef struct m1_tx_data {
    m1_frame_version_e version; /*!< Version of the M1 protocol frame. */

    /* Host information */
    u8 source_id;     /*!< ID of the source device. */
    u8* target_id;    /*!< Pointer to the array of target device IDs. */
    u8 target_id_len; /*!< Length of the target ID array. */

    /* Frame attributes */
    m1_reliable_tx_e reliable_tx; /*!< Specifies if the transmission is reliable
                                     (e.g., acknowledgment required). */
    m1_fragment_e fragment;       /*!< Indicates if the data is fragmented. */
    m1_encrypt_e encrypt;         /*!< Specifies if the data is encrypted. */
    m1_priority_e priority;       /*!< Priority level of the data frame. */
    m1_compress_e compress;       /*!< Indicates if the data is compressed. */

    /* Frame data */
    u8* data;                 /*!< Pointer to the payload data. */
    size_t data_len;          /*!< Length of the payload data. */
    m1_data_type_e data_type; /*!< Type of the data being transmitted. */
} m1_tx_data_t;

/**
 * \brief           Structure for receiving data in the M1 protocol.
 *
 * This structure is used to define the attributes and payload of a received
 * data frame.
 */
typedef struct m1_rx_data {
    u8 source_id; /*!< ID of the source device. */
    u8 target_id; /*!< ID of the target device. */
    u8* data;     /*!< Pointer to the received payload data. */
    u16 data_len; /*!< Length of the received payload data. */
} m1_rx_data_t;

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_FORMAT_DATA_H__ */

/* ----------------------------- end of file -------------------------------- */
