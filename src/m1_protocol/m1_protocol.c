/**
 * \file            m1_protocol.c
 * \brief           Implementation of the M1 Protocol, which includes
 *                  initialization, data transmission, and protocol runtime
 *                  logic.
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

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_protocol.h"

#include <string.h>
#include "./m1_protocol/m1_layer_datalink.h"
#include "./m1_protocol/m1_layer_transport.h"

/* public variables --------------------------------------------------------- */
/**
 * \brief Global instance of the M1 protocol structure.
 */
m1_t m1;

/* private function prototypes ---------------------------------------------- */
/**
 * \brief           Append a new RX parse node to the parse list.
 *
 * \param[in]       route: Pointer to the route item structure.
 * \return          `E_STATE_OK` on success, or an error code otherwise.
 */
static etype_e append_rx_parse_node(m1_route_item_t* route);

/**
 * \brief           Check if an RX parse node already exists for the specified
 *                  route.
 *
 * \param[in]       route: Pointer to the route item structure.
 * \return          true if the RX node exists, false otherwise.
 */
static bool is_rx_node_existed(m1_route_item_t* route);

/* public functions --------------------------------------------------------- */
/**
 * \brief           Get the current route table.
 *
 * \param[out]      table: Pointer to store the route table address.
 * \param[out]      len: Pointer to store the route table length.
 * \return          E_STATE_OK on success, or E_STATE_INVAL if inputs are
 *                  invalid.
 */
etype_e m1_get_route_table(m1_route_item_t** table, size_t* len) {
    if (!table || !len) {
        return E_STATE_INVAL;
    }

    *table = m1.route_item;
    *len = m1.route_item_len;
    return E_STATE_OK;
}

/**
 * \brief           Initialize the M1 protocol.
 *
 * \param[in]       name: Name of the M1 protocol instance.
 * \param[in]       tx_pool_size: Tx memory pool size.
 * \param[in]       route_table: Pointer to the routing table used by the
 *                  protocol.
 * \param[in]       route_len: Number of entries in the routing table.
 * \param[in]       rx_cb_table: Pointer to the callback table for processing
 *                  received data.
 * \param[in]       rx_cb_len: Number of callbacks in the table.
 * \param[in]       source_id: Pointer to the source device ID.
 * \param[in]       source_id_len: Length of the source device ID.
 * \return          etype_e indicating success or failure of initialization.
 */
etype_e m1_protocol_init(const char name[], size_t tx_pool_size,
                         m1_route_item_t* route_table, size_t route_len,
                         m1_rx_parse_callback_item_t* rx_cb_table,
                         size_t rx_cb_len, u8* source_id,
                         size_t source_id_len) {
    if (!(tx_pool_size && route_table && route_len && rx_cb_table && rx_cb_len
          && source_id && source_id_len)) {
        return E_STATE_INVAL;
    }

    if (m1.init_ok) {
        return E_STATE_OK;
    }

    m1.name = name;

    /* Initialize memory pool */
    m1.tx_pool_size = tx_pool_size;
    m1.tx_pool = MemoryPoolInit(m1.tx_pool_size, m1.tx_pool_size);
    if (!m1.tx_pool) {
        return E_STATE_NO_SPACE;
    }

    /* Initialize route table */
    m1.route_item = route_table;
    m1.route_item_len = route_len;

    /* Initialize RX parse callbacks */
    for (size_t i = 0; i < rx_cb_len; i++) {
        m1.rx_parse_cb[rx_cb_table[i].data_type] = rx_cb_table[i].cb;
    }

    /* Copy source ID */
    m1.source_id = m1_malloc(source_id_len);
    if (!m1.source_id) {
        return E_STATE_NO_SPACE;
    }
    memcpy(m1.source_id, source_id, source_id_len);
    m1.source_id_len = source_id_len;

    /* Initialize RX parse node list */
    single_list_init(&m1.rx_parse_head);
    for (size_t i = 0; i < m1.route_item_len; i++) {
        if (m1.route_item[i].rx && !is_rx_node_existed(&m1.route_item[i])) {
            etype_e result = append_rx_parse_node(&m1.route_item[i]);
            if (result != E_STATE_OK) {
                return result;
            }
        }
    }

    /* Initialize data link layer RX buffer */
    m1.datalink_rx_buf_len = 128;
    m1.datalink_rx_buf = m1_malloc(sizeof(u8) * m1.datalink_rx_buf_len);
    if (!m1.datalink_rx_buf) {
        return E_STATE_NO_SPACE;
    }

    /* Initialize sequence numbers */
    m1.seq_num = m1_malloc(sizeof(u8) * m1.route_item_len);
    if (!m1.seq_num) {
        return E_STATE_NO_SPACE;
    }
    memset(m1.seq_num, 0, sizeof(u8) * m1.route_item_len);

    /* Initialize wait ACK packet list */
    single_list_init(&m1.wait_ack_packet_head);

    m1.init_ok = true;

    return E_STATE_OK;
}

/**
 * \brief           Transmit data using the M1 protocol.
 *
 * \param[in]       tx_data: Pointer to the structure containing data to be
 *                  transmitted.
 * \return          etype_e indicating success or failure of the transmission.
 */
etype_e m1_protocol_tx_data(m1_tx_data_t* tx_data) {
    if (!tx_data) {
        return E_STATE_INVAL;
    }
    return m1_transport_send(tx_data);
}

/**
 * \brief           Run the periodic tasks of the M1 protocol.
 *
 * This function should be called regularly to handle time-dependent tasks such
 * as retransmissions, acknowledgments, and other protocol-specific processes.
 *
 * \param[in]       freq: Frequency of execution in Hz.
 */
void m1_protocol_run(u32 freq) {
    m1_datalink_receive(freq);
    m1_transport_run(freq);
}

/* private functions -------------------------------------------------------- */
/**
 * \brief           Append a new RX parse node for a given route.
 *
 * \param[in]       route: Pointer to the route item.
 * \return          `E_STATE_OK` on success, or E_STATE_NO_SPACE on allocation
 *                  failure.
 */
static etype_e append_rx_parse_node(m1_route_item_t* route) {
    m1_rx_parse_node_t* rx_parse_node = m1_malloc(sizeof(m1_rx_parse_node_t));
    if (!rx_parse_node) {
        return E_STATE_NO_SPACE;
    }

    memset(rx_parse_node, 0, sizeof(m1_rx_parse_node_t));
    rx_parse_node->item.parse.cache_len = BYTES_ALIGN(route->max_pkg_size, 4);
    rx_parse_node->item.parse.cache = m1_malloc(
        rx_parse_node->item.parse.cache_len);
    if (!rx_parse_node->item.parse.cache) {
        m1_free(rx_parse_node);
        return E_STATE_NO_SPACE;
    }

    rx_parse_node->item.rx = route->rx;
    rx_parse_node->item.read_freq = route->read_freq;
    single_list_append(&m1.rx_parse_head, &rx_parse_node->node);

    return E_STATE_OK;
}

/**
 * \brief           Check if an RX node already exists for a given route.
 *
 * \param[in]       route: Pointer to the route item.
 * \return          true if an RX node exists, false otherwise.
 */
static bool is_rx_node_existed(m1_route_item_t* route) {
    single_list_t* rx_parse_node = m1.rx_parse_head.next;
    while (rx_parse_node) {
        m1_rx_parse_node_t* ops = single_list_entry(rx_parse_node,
                                                    m1_rx_parse_node_t, node);
        if (route->rx == ops->item.rx) {
            return true;
        }
        rx_parse_node = rx_parse_node->next;
    }
    return false;
}

/* ----------------------------- end of file -------------------------------- */
