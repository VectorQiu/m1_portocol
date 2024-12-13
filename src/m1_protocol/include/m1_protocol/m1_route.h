/**
 * \file            m1_route.h
 * \brief           Header file for managing M1 routing configurations,
 *                  including link definitions and routing tables.
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
#ifndef __M1_ROUTE_H__
#define __M1_ROUTE_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_async_rx_tx.h" /*!< Provides asynchronous transmission and reception structures. */
#include "./m1_protocol/m1_typedef.h" /*!< Includes type definitions used across the protocol. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1_route_manager
 * \brief           M1 route manager for defining and managing network routes.
 * \{
 */

/**
 * \brief           Enumeration for different types of communication links.
 */
typedef enum {
    M1_LINK_TYPE_UART = 0, /*!< Link type for UART communication. */
} m1_link_type_e;

/* public typedef struct ---------------------------------------------------- */
/**
 * \brief           Structure defining a routing item for M1 protocol.
 */
typedef struct m1_route_item {
    char* link_name; /*!< Name of the communication link, e.g., "UART1". */
    m1_link_type_e link_type; /*!< Type of the link (e.g., UART). */
    u8 target_id;             /*!< ID of the target device for this route. */
    char* host_name; /*!< Name of the host associated with this link. */
    tx_async_t* tx;  /*!< Pointer to the asynchronous transmission instance. */
    rx_async_t* rx;  /*!< Pointer to the asynchronous reception instance. */
    u16 read_freq;   /*!< Frequency (in Hz) for reading data from this link. */
    size_t max_pkg_size; /*!< Maximum allowable package size for this link. */
} m1_route_item_t;

/**
 * \brief           Node structure for routing, used in linked list
 * implementations.
 */
typedef struct m1_route_node {
    m1_route_item_t item; /*!< Routing item data. */
    single_list_t
        node; /*!< Single linked list node for maintaining a routing table. */
} m1_route_node_t;

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_ROUTE_H__ */

/* ----------------------------- end of file -------------------------------- */
