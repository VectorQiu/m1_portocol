/**
 * \file            m1_format_packet.h
 * \brief           Header file for managing M1 protocol packets.
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
#ifndef __M1_FORMAT_PACKET_H__
#define __M1_FORMAT_PACKET_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_async_rx_tx.h" /*!< Provides asynchronous RX and TX operations. */
#include "./m1_protocol/m1_format_frame.h" /*!< Defines M1 protocol frame structure and attributes. */
#include "./m1_protocol/m1_typedef.h" /*!< General type definitions for the M1 protocol. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1_format_packet_manager
 * \brief           M1 protocol packet manager.
 * \{
 */

/**
 * \brief           Structure for storing packet data in the M1 protocol.
 *
 * This structure holds the actual data content of a packet, along with a
 * reference counter.
 */
typedef struct m1_packet_data {
    u8 reference_counter; /*!< Reference count for the packet data. */
    size_t data_len;      /*!< Length of the data buffer. */
    u8* data;             /*!< Pointer to the data buffer. */
} m1_packet_data_t;

/**
 * \brief           Structure representing an M1 protocol packet.
 *
 * This structure includes all the fields required to manage an M1 protocol
 * packet, including source and target identifiers, attributes, and data.
 */
typedef struct m1_packet {
    u8 source_id; /*!< ID of the source device. */
    u8 target_id; /*!< ID of the target device. */
    u8 seq_num;   /*!< Sequence number of the packet. */
    u8 ack_num;   /*!< Acknowledgment number for the packet. */

    m1_frame_version_e version; /*!< Version of the M1 protocol frame. */

    /* Frame attributes */
    m1_reliable_tx_e
        reliable_tx; /*!< Specifies if reliable transmission is enabled. */
    m1_fragment_e fragment; /*!< Indicates if the packet is fragmented. */
    m1_encrypt_e encrypt;   /*!< Specifies if the packet data is encrypted. */
    m1_priority_e priority; /*!< Priority level of the packet. */
    m1_compress_e compress; /*!< Indicates if the packet data is compressed. */
    m1_data_type_e data_type; /*!< Type of the data contained in the packet. */

    /* Frame data */
    m1_packet_data_t* data; /*!< Pointer to the packet data structure. */

    /* Retransmission */
    u8 retry_num;     /*!< Number of retransmission attempts. */
    i32 wait_time_ms; /*!< Wait time in milliseconds before retransmission. */

    tx_async_t* tx; /*!< Pointer to the asynchronous TX manager. */
    single_list_t
        node; /*!< Node for managing the packet in a single-linked list. */
} m1_packet_t;

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_FORMAT_PACKET_H__ */

/* ----------------------------- end of file -------------------------------- */
