/**
 * \file            m1_layer_datalink.c
 * \brief           Implements the data link layer functionalities for the M1
 *                  protocol stack.
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
#include "./m1_protocol/m1_layer_datalink.h"

#include <string.h>
#include "./crc/crc16_lookup.h"
#include "./crc/crc8_lookup.h"
#include "./m1_protocol/m1_layer_network.h"
#include "./m1_protocol/m1_protocol_def.h"
#include "./m1_protocol/m1_rx_parse.h"

/* private function prototypes ---------------------------------------------- */
static void m1_frame_parse(m1_rx_parse_node_t* node, u8* buf, size_t len);

/* public functions --------------------------------------------------------- */

/**
 * \brief           Handles data reception at the data link layer.
 *
 * \param[in]       freq: The frequency at which this function is called.
 * \note            This function processes the data received by invoking the
 *                  appropriate parsing routines for each node in the receive
 *                  parse list.
 */
void m1_datalink_receive(u32 freq) {
    /*! Keeps track of the function invocation count. */
    static u32 run_cnt = 0;
    u8* rx_buf = m1.datalink_rx_buf; /*!< Pointer to the receive buffer. */

    single_list_t* rx_parse_node = m1.rx_parse_head.next;
    while (rx_parse_node) {
        m1_rx_parse_node_t* ops =
            single_list_entry(rx_parse_node, m1_rx_parse_node_t, node);
        size_t rx_len = m1.datalink_rx_buf_len;
        if (run_cnt % (freq / ops->item.read_freq) == 0) {
            etype_e ret = ops->item.rx->rx(rx_buf, &rx_len);
            if (ret == E_STATE_OK && rx_len) {
                m1_frame_parse(ops, rx_buf, rx_len);
            }
        }
        rx_parse_node = rx_parse_node->next;
    }
    run_cnt++;
}

/**
 * \brief           Sends a packet over the data link layer.
 *
 * \param[in]       packet: The packet to send.
 * \return          E_STATE_OK if the packet was sent successfully, or an error
 *                  code otherwise.
 * \note            Constructs a frame by adding headers and CRC, then sends
 *                  the frame using the transport function.
 */
etype_e m1_datalink_send(m1_packet_t* packet) {
    etype_e ret = E_STATE_OK;
    size_t frame_len =
        sizeof(m1_frame_head_t) + packet->data->data_len + sizeof(u16);
    u8* frame_buf = (u8*)MemoryPoolAlloc(m1.tx_pool, frame_len);
    if (frame_buf == NULL) {
        /** Memory allocation failed. */
        return E_STATE_NO_SPACE;
    }
    memset(frame_buf, 0, frame_len);

    m1_frame_head_t* frame_head = (m1_frame_head_t*)frame_buf;

    /* Populate frame header fields */
    frame_head->sof = M1_FRAME_HEAD_SOF;
    frame_head->version = packet->version;
    frame_head->data_type = packet->data_type;
    frame_head->source_id = packet->source_id;
    frame_head->target_id = packet->target_id;
    frame_head->attr.lsb.reliable = packet->reliable_tx;
    frame_head->attr.lsb.fragment = packet->fragment;
    frame_head->attr.lsb.encrypt = packet->encrypt;
    frame_head->attr.lsb.priority = packet->priority;
    frame_head->attr.msb.compress = packet->compress;
    frame_head->data_len_lsb = packet->data->data_len & 0xFF;
    frame_head->data_len_msb = (packet->data->data_len & 0xFF00) >> 8;
    frame_head->seq_num = packet->seq_num;
    frame_head->ack_num = packet->ack_num;

    /** Calculate CRC for the frame header */
    crc8_lookup_pack_buf(CRC8_MAXIM_LOOKUP_MODEL, frame_buf,
                         sizeof(m1_frame_head_t));
    memcpy(frame_buf + sizeof(m1_frame_head_t), packet->data->data,
           packet->data->data_len);
    crc16_lookup_pack_buf(CRC16_MODBUS_LOOKUP_MODEL, frame_buf, frame_len);

    /* Transmit the frame */
    ret = packet->tx->tx(frame_buf, frame_len);

    MemoryPoolFree(m1.tx_pool, frame_buf);
    if (ret != E_STATE_OK) {
        if (m1.tx_abnormal_cb != NULL) {
            m1.tx_abnormal_cb(packet);
        }
    }
    return ret;
}

/* private functions -------------------------------------------------------- */

/**
 * \brief           Parses a received frame and processes it.
 * \param[in]       node: The receive parsing node.
 * \param[in]       buf: The received data buffer.
 * \param[in]       len: The length of the received data.
 * \note            This function handles frame structure validation, including
 *                  SOF, header, and CRC checks, and forwards valid frames to
 *                  the network layer.
 */
static void m1_frame_parse(m1_rx_parse_node_t* node, u8* buf, size_t len) {
    if (node == NULL) {
        return;
    }
    m1_parse_t* parse = &node->item.parse;
    m1_frame_head_t* frame_head = NULL;
    size_t data_len = 0;
    size_t frame_len = 0;

    M1_STATS_RX_NODE_TOTAL_BYTES(node, len);
    for (size_t i = 0; i < len; i++) {
        switch (parse->step) {
            case M1_PARSE_FRAME_SOF:
                if (buf[i] == M1_FRAME_HEAD_SOF) {
                    parse->step = M1_PARSE_FRAME_HEAD;
                    parse->index = 0;
                    memset(parse->cache, 0, parse->cache_len);
                    parse->cache[parse->index] = buf[i];
                    parse->index++;
                    M1_STATS_RX_NODE_SOF_OK(node);
                } else {
                    M1_STATS_RX_NODE_NOT_FRAME_BYTES(node);
                }
                break;

            case M1_PARSE_FRAME_HEAD:
                if (parse->index < sizeof(m1_frame_head_t) - 1) {
                    parse->cache[parse->index] = buf[i];
                    parse->index++;
                } else { /* head crc8 */
                    parse->cache[parse->index] = buf[i];
                    parse->index++;
                    if (crc8_lookup_verify_buf(CRC8_MAXIM_LOOKUP_MODEL,
                                               parse->cache,
                                               sizeof(m1_frame_head_t))) {
                        parse->step = M1_PARSE_FRAME_DATA;
                        M1_STATS_RX_NODE_CRC8_OK(node);
                    } else {
                        parse->step = M1_PARSE_FRAME_SOF;
                        M1_STATS_RX_NODE_CRC8_ERR(node);
                    }
                }
                break;

            case M1_PARSE_FRAME_DATA:
                frame_head = (m1_frame_head_t*)parse->cache;
                data_len =
                    frame_head->data_len_msb << 8 | frame_head->data_len_lsb;
                frame_len = sizeof(m1_frame_head_t) + data_len + sizeof(u16);

                /* Check if the cache can hold the entire frame */
                if (frame_len > parse->cache_len) {
                    if (parse->index < frame_len - 1) {
                        parse->index++;
                    } else {
                        parse->step = M1_PARSE_FRAME_SOF;
                        M1_STATS_RX_NODE_LEN_OVERFLOW(node);
                    }
                    break;
                }

                if (parse->index < frame_len - 1) {
                    parse->cache[parse->index] = buf[i];
                    parse->index++;
                } else {
                    parse->cache[parse->index] = buf[i];
                    parse->index++;
                    if (crc16_lookup_verify_buf(CRC16_MODBUS_LOOKUP_MODEL,
                                                parse->cache, frame_len)) {
                        m1_network_receive(parse->cache, frame_len);
                        M1_STATS_RX_NODE_CRC16_OK(node);
                    } else {
                        M1_STATS_RX_NODE_CRC16_ERR(node);
                    }
                    parse->step = M1_PARSE_FRAME_SOF;
                }
                break;

            default:
                break;
        }
    }
}

/* ----------------------------- end of file -------------------------------- */
