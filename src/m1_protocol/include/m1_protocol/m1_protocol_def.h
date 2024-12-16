/**
 * \file            m1_protocol_def.h
 * \brief           Definition and configuration for the M1 protocol.
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
#ifndef __M1_PROTOCOL_DEF_H__
#define __M1_PROTOCOL_DEF_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_format_data.h" /*!< Includes definitions for formatting M1 protocol data. */
#include "./m1_protocol/m1_route.h" /*!< Includes routing logic for the M1 protocol. */
#include "./m1_protocol/m1_rx_parse.h" /*!< Includes parsing logic for received data. */
#include "./m1_protocol/m1_typedef.h" /*!< Includes common type definitions for the M1 protocol. */
#include "./memory_pool/memory_pool.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1_protocol_def_manager
 * \brief           Manager for M1 protocol definitions and configurations.
 * \{
 */

/* Public configuration ----------------------------------------------------- */

/* Public definitions ------------------------------------------------------- */

/* Public typedefs ---------------------------------------------------------- */
/**
 * \brief           Callback function type for handling abnormal transmissions.
 *
 * This function is triggered when an abnormal transmission occurs, such as a
 * timeout or failure to send.
 * \param[in]       packet  Pointer to the packet that encountered an abnormal
 *                  transmission.
 */
typedef void (*m1_tx_abnormal_callback_t)(m1_packet_t* packet);

/**
 * \brief           Structure representing internal data for the M1 protocol.
 *
 * This structure maintains the internal state and configuration of the M1
 * protocol, including routing, buffer management, and callback functions.
 */
typedef struct m1_internal_data {
    bool init_ok; /*!< Indicates if the M1 protocol is initialized successfully.
                   */
    const char* name;     /*!< Name or identifier for the protocol instance. */
    u8* source_id;        /*!< Pointer to the source ID. */
    size_t source_id_len; /*!< Length of the source ID. */

    MemoryPool* tx_pool; /* tx memory pool */
    size_t tx_pool_size; /* tx memory pool size */
    // 互斥锁

    m1_route_item_t* route_item; /*!< Pointer to the routing table. */
    size_t route_item_len;       /*!< Number of entries in the routing table. */
    u8* seq_num; /*!< Array to record frame sequence numbers for each route
                    node. */

    single_list_t rx_parse_head; /*!< Head node of the list for received data
                                    parsing. \ref m1_rx_node_t */
    u8* datalink_rx_buf; /*!< Buffer for receiving data from the data link
                            layer. */
    size_t datalink_rx_buf_len; /*!< Length of the data link layer receive
                                   buffer. */

    m1_rx_parse_callback_t
        rx_parse_cb[M1_DATA_TYPE_MAX]; /*!< Array of callbacks for parsing
                                          received data by type. */
    m1_tx_abnormal_callback_t
        tx_abnormal_cb; /*!< Callback for handling abnormal transmissions. */

    single_list_t wait_ack_packet_head; /*!< Head node of the list for packets
                                           waiting for acknowledgment. */
} m1_t;

/* Public variables --------------------------------------------------------- */
/**
 * \brief           Global M1 protocol instance.
 *
 * This variable holds the internal state and configuration of the M1 protocol.
 */
extern m1_t m1;

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_PROTOCOL_DEF_H__ */

/* ----------------------------- end of file -------------------------------- */
