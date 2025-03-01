/**
 * \file            m1_rx_parse.h
 * \brief           Header file for managing RX data parsing in the M1 protocol.
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
#ifndef __M1_RX_PARSE_H__
#define __M1_RX_PARSE_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_async_rx_tx.h"  /*!< Asynchronous RX/TX handling. */
#include "./m1_protocol/m1_format_data.h"  /*!< Data format definitions. */
#include "./m1_protocol/m1_format_frame.h" /*!< Frame format definitions. */
#include "./m1_protocol/m1_statistic.h" /*!< Statistic tracking for parsing. */
#include "./m1_protocol/m1_typedef.h"   /*!< Type definitions used across M1. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1_rx_parse_manager
 * \brief           Manager for parsing received M1 protocol data.
 * \{
 */

/* public typedef enum ------------------------------------------------------ */
/**
 * \brief           Parsing steps for the RX frame.
 */
typedef enum {
    M1_PARSE_FRAME_SOF = 0, /*!< Step for detecting Start of Frame (SOF). */
    M1_PARSE_FRAME_HEAD,    /*!< Step for parsing the frame header. */
    M1_PARSE_FRAME_DATA,    /*!< Step for parsing the frame data. */
} m1_parse_step_e;

/* public typedef struct ---------------------------------------------------- */
/**
 * \brief           Callback type for handling parsed RX data.
 * \param[in]       rx_data: Pointer to the parsed RX data structure.
 */
typedef void (*m1_rx_parse_callback_t)(m1_rx_data_t* rx_data);

/**
 * \brief           Callback item structure for associating data type with a
 *                  callback.
 */
typedef struct m1_rx_callback_item {
    m1_data_type_e
        data_type; /*!< Data type this callback is responsible for. */
    m1_rx_parse_callback_t cb; /*!< Pointer to the callback function. */
} m1_rx_parse_callback_item_t;

/**
 * \brief           Node structure for managing RX callback items in a linked
 * list.
 */
typedef struct m1_rx_callback_node {
    m1_rx_parse_callback_item_t item; /*!< RX callback item data. */
    single_list_t node; /*!< Node for linked list implementation. */
} m1_rx_callback_node_t;

/**
 * \brief           Structure for maintaining parsing state.
 */
typedef struct m1_parse {
    m1_parse_step_e step; /*!< Current parsing step. */
    u32 cache_len;        /*!< Length of cached data. */
    u8* cache;            /*!< Pointer to the cache buffer. */
    u32 index;            /*!< Current index in the cache. */
} m1_parse_t;

/**
 * \brief           RX parse item structure for managing parsing per RX
 * instance.
 */
typedef struct m1_rx_parse_item {
    rx_async_t* rx;   /*!< Pointer to the asynchronous RX instance. */
    m1_parse_t parse; /*!< Parsing state information. */
    u16 read_freq; /*!< Frequency at which data is read from this RX instance.
                    */
} m1_rx_parse_item_t;

/**
 * \brief           Node structure for RX parse items, including statistics.
 */
typedef struct m1_rx_parse_node {
    m1_rx_parse_item_t item;   /*!< RX parse item data. */
    single_list_t node;        /*!< Node for linked list implementation. */
    m1_stats_rx_parse_t stats; /*!< Statistics for RX parsing performance. */
} m1_rx_parse_node_t;

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_RX_PARSE_H__ */

/* ----------------------------- end of file -------------------------------- */
