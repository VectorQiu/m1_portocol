/**
 * \file            m1_layer_network.c
 * \brief           Provides network layer functionalities for the M1 protocol
 *                  stack, including packet routing and forwarding.
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
#include "./m1_protocol/m1_layer_network.h"

#include "./m1_protocol/m1_layer_datalink.h"
#include "./m1_protocol/m1_layer_transport.h"
#include "./m1_protocol/m1_protocol_def.h"

/* private function prototypes ---------------------------------------------- */
static tx_async_t* find_route(u16 target_id);

/* public functions --------------------------------------------------------- */

/**
 * \brief           Processes a received frame at the network layer.
 *
 * This function checks the target ID of the frame header to determine whether
 * the packet is addressed to the local node or needs to be forwarded to another
 * node via routing.
 *
 * \param[in]       frame_buf: Pointer to the buffer containing the received
 * frame.
 * \param[in]       frame_len: Length of the received frame in bytes.
 * \return          E_STATE_OK if the packet is successfully processed or
 * forwarded.
 * \return          E_STATE_NOT_EXIST if the target node does not exist in the
 * routing table.
 */
etype_e m1_network_receive(u8* frame_buf, size_t frame_len) {
    m1_frame_head_t* frame_head = (m1_frame_head_t*)frame_buf;

    // Check if the frame is addressed to this node
    for (size_t i = 0; i < m1.source_id_len; ++i) {
        if (frame_head->target_id == m1.source_id[i]) {
            return m1_transport_receive(frame_buf, frame_len);
        }
    }

    // Routing logic for forwarding packets
    tx_async_t* route_tx = find_route(frame_head->target_id);
    if (route_tx) {
        return route_tx->tx(frame_buf, frame_len);
    }

    // Target node does not exist in the routing table
    // TODO: Handle non-existent destination node appropriately

    return E_STATE_NOT_EXIST;
}

/**
 * \brief           Sends a packet at the network layer.
 *
 * This function determines the appropriate route for the packet based on the
 * target ID and appends a sequence number if required.
 *
 * \param[in]       packet: Pointer to the packet to be sent.
 * \param[in]       add_seq_num: Flag indicating whether to add a sequence
 *                  number to the packet.
 * \return          E_STATE_OK if the packet is successfully sent.
 * \return          E_STATE_NOT_EXIST if the target node does not exist in the
 *                  routing table.
 */
etype_e m1_network_send(m1_packet_t* packet, bool add_seq_num) {
    tx_async_t* route_tx = NULL;

    // Find the route for the target ID
    for (size_t i = 0; i < m1.route_item_len; ++i) {
        if (packet->target_id == m1.route_item[i].target_id) {
            route_tx = m1.route_item[i].tx;
            if (add_seq_num) {
                packet->seq_num = m1.seq_num[i]++;
            }
            break;
        }
    }

    if (route_tx) {
        packet->tx = route_tx;
        return m1_datalink_send(packet);
    }

    // Target node does not exist in the routing table
    // TODO: Handle non-existent destination node appropriately

    return E_STATE_NOT_EXIST;
}

/* private functions -------------------------------------------------------- */
/**
 * \brief           Finds the transmission route for a given target ID.
 *
 * This function searches the routing table to find the transmission mechanism
 * (tx_async_t) corresponding to the specified target ID. If no matching route
 * is found, it returns NULL.
 *
 * \param[in]       target_id: The ID of the target node for which the route is
 *                  being searched.
 * \return          Pointer to the tx_async_t structure if a matching route is
 *                  found.
 * \return          NULL if no route matches the specified target ID.
 */
static tx_async_t* find_route(u16 target_id) {
    for (size_t i = 0; i < m1.route_item_len; ++i) {
        if (target_id == m1.route_item[i].target_id) {
            return m1.route_item[i].tx;
        }
    }
    return NULL;
}

/* ----------------------------- end of file -------------------------------- */
