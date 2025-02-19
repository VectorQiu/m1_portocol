/**
 * \file            m1_layer_transport.c
 * \brief           Transport layer implementation with optimized structure and
 *                  reliability.
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
#include "./m1_protocol/m1_layer_transport.h"

#include <string.h>
#include "./m1_protocol/m1_format_data.h"
#include "./m1_protocol/m1_layer_network.h"
#include "./m1_protocol/m1_protocol_def.h"
#include "./m1_protocol/m1_rx_parse.h"

/* private config ----------------------------------------------------------- */

/* private define ----------------------------------------------------------- */
/*! Timeout period (in milliseconds) to wait for acknowledgment */
#define ACK_WAIT_TIME_MS 1000
/*! Maximum retry count for reliable transmission*/
#define MAX_RETRY_COUNT  5

/* private function prototypes ---------------------------------------------- */
static void handle_ack_retries(u32 freq);
static etype_e handle_wait_ack_packet(single_list_t* node);
static etype_e process_acknowledgment(m1_frame_head_t* frame_head);
static etype_e send_ack_to_source_host(m1_frame_head_t* frame_head);
static m1_packet_data_t* allocate_ack_data(m1_packet_data_t* src_data);

/* public functions --------------------------------------------------------- */
/**
 * \brief           Executes periodic transport layer tasks.
 *
 * This function performs tasks such as retransmission handling, timeouts, and
 * state management. It should be called periodically by the application.
 *
 * \param[in]       freq: Frequency of the data reception process in Hz.
 */
void m1_transport_run(u32 freq) {
    /*! Handle retries for packets awaiting acknowledgment */
    handle_ack_retries(freq);
}

/**
 * \brief           Receives data from the network layer.
 *
 * This function processes raw data received from the network layer, extracts
 * useful information, and prepares it for further processing at higher layers.
 *
 * \param[in]       frame_buf: Pointer to the buffer containing the raw
 *                  frame data.
 * \param[in]       frame_len: Length of the raw frame data.
 * \return          Returns an error code of type `etype_e` indicating the
 *                  success or failure of the operation.
 *                   - `ETYPE_OK` on success.
 *                   - Other error codes indicating specific failures.
 */
etype_e m1_transport_receive(u8* frame_buf, size_t frame_len) {
    (void)frame_len; /*!< Frame length is unused here */
    m1_frame_head_t* frame_head =
        (m1_frame_head_t*)frame_buf; /*!< Cast frame buffer to frame header */

    if (frame_head->attr.lsb.reliable == M1_RELIABLE_TX) {
        send_ack_to_source_host(
            frame_head); /*!< Send acknowledgment for reliable packet */
    } else if (frame_head->attr.lsb.reliable == A1_RELIABLE_TX_ACK) {
        process_acknowledgment(
            frame_head); /*!< Process received acknowledgment */
    }

    size_t rx_data_len =
        frame_head->data_len_msb << 8 |
        frame_head->data_len_lsb; /*!< Calculate received data length */
    m1_rx_data_t rx_data = {
        .source_id = frame_head->source_id,
        .target_id = frame_head->target_id,
        .data_len = rx_data_len,
        .data =
            frame_buf + sizeof(m1_frame_head_t), /*!< Extract data pointer */
    };

    if (frame_head->data_type >= M1_DATA_TYPE_MAX) {
        return E_STATE_INVAL; /*!< Return invalid state for unknown data types
                               */
    }

    if (rx_data_len) {
        m1_rx_parse_callback_t rx_parse_cb =
            m1.rx_parse_cb[frame_head->data_type]; /*!< Fetch the parsing
                                                      callback */
        if (rx_parse_cb) {
            rx_parse_cb(&rx_data); /*!< Call the parsing callback */
        } else {
            return E_STATE_NOT_EXIST; /*!< Return error if callback is not found
                                       */
        }
    }

    return E_STATE_OK; /*!< Return success */
}

/**
 * \brief           Sends data to the network layer.
 *
 * This function prepares the data for transmission by encapsulating it in the
 * appropriate transport layer structure and sends it to the network layer for
 * further processing.
 *
 * \param[in]       tx_data: Pointer to the data structure containing
 *                  information about the transmission.
 * \return          Returns an error code of type `etype_e` indicating the
 *                  success or failure of the operation.
 *                   - `ETYPE_OK` on success.
 *                   - Other error codes indicating specific failures.
 */
etype_e m1_transport_send(m1_tx_data_t* tx_data) {
    if (!m1.init_ok) {
        return E_STATE_NOT_IMPLEMENT; /*!< Return error if transport layer is
                                         not initialized */
    }

    m1_packet_t packet = {0}; /*!< Initialize the packet structure */
    m1_packet_data_t packet_data = {
        .data = tx_data->data,
        .data_len = tx_data->data_len,
    };

    packet.data = &packet_data;
    packet.version = tx_data->version;
    packet.source_id = tx_data->source_id
                           ? tx_data->source_id
                           : m1.source_id[0]; /*!< Assign source ID */
    packet.data_type = tx_data->data_type;
    /* Packet attributes */
    packet.reliable_tx = tx_data->reliable_tx;
    packet.fragment = tx_data->fragment;
    packet.encrypt = tx_data->encrypt;
    packet.priority = tx_data->priority;
    packet.compress = tx_data->compress;

    m1_packet_data_t* ack_data = NULL;
    if (packet.reliable_tx == M1_RELIABLE_TX) {
        packet.retry_num = MAX_RETRY_COUNT;
        packet.wait_time_ms = ACK_WAIT_TIME_MS;
        ack_data = allocate_ack_data(&packet_data);
        if (!ack_data) {
            return E_STATE_NO_SPACE;
        }
    }

    for (size_t i = 0; i < tx_data->target_id_len; ++i) {
        packet.target_id = tx_data->target_id[i]; /*!< Assign target ID */

        if (packet.reliable_tx == M1_RELIABLE_TX) {
            for (size_t i = 0; i < m1.route_item_len; ++i) {
                if (packet.target_id == m1.route_item[i].target_id) {
                    packet.seq_num = m1.seq_num[i];
                    break;
                }
            }
            /*! Allocate memory for acknowledgment waiting packet */
            m1_packet_t* wait_ack_packet =
                (m1_packet_t*)MemoryPoolAlloc(m1.tx_pool, sizeof(m1_packet_t));
            if (!wait_ack_packet) {
                break; /*!< Exit loop if memory allocation fails */
            }
            memcpy(wait_ack_packet, &packet, sizeof(m1_packet_t));
            wait_ack_packet->data = ack_data;
            ack_data->reference_counter++;
            single_list_append(
                &m1.wait_ack_packet_head,
                &wait_ack_packet->node); /*!< Append to waiting list */
        }

        if (m1_network_send(&packet, true) != E_STATE_OK) {
            // Log or handle send failure.
        }
    }

    return E_STATE_OK;
}

/* private functions -------------------------------------------------------- */
/**
 * \brief           Handle acknowledgment retries for packets in the waiting
 *                  list.
 *
 * \param           freq: Frequency (in Hz) of the function calls, used to
 *                  adjust timeout.
 */
static void handle_ack_retries(u32 freq) {
    if (freq == 0) {
        return; /*!< Do nothing if frequency is zero */
    }

    single_list_t* current_node = m1.wait_ack_packet_head.next;
    while (current_node) {
        m1_packet_t* packet = single_list_entry(
            current_node, m1_packet_t, node);  /*!< Get packet from node */
        packet->wait_time_ms -= (1000 / freq); /*!< Adjust wait time */

        if (packet->wait_time_ms <= 0) {
            if (--packet->retry_num <= 0) {
                link_warning("retry transport packet: %d timeout.\n",
                             packet->seq_num); /*!< Log timeout warning */
                single_list_t* next_node = current_node->next;
                /*! Remove node from list */
                handle_wait_ack_packet(current_node);
                current_node = next_node;
                continue;
            } else {
                link_warning("wait [0x%02x] ack [%d] timeout, retry[%d].\n",
                             packet->target_id, packet->seq_num,
                             packet->retry_num); /*!< Log retry information */
                packet->wait_time_ms = ACK_WAIT_TIME_MS; /*!< Reset wait time */
                m1_network_send(packet, false); /*!< Retry sending packet */
            }
        }

        current_node = current_node->next; /*!< Move to the next node */
    }
}

/**
 * \brief           Handle a specific packet node in the acknowledgment waiting
 *                  list.
 *
 * \param           node: Pointer to the node in the acknowledgment waiting
 *                  list.
 * \return          Returns an error code of type `etype_e` indicating the
 *                  success or failure of the operation.
 *                   - `ETYPE_OK` on success.
 *                   - Other error codes indicating specific failures.
 * \note            Multi-threaded calls are not allowed, and there is no
 *                  resource mutual exclusion protection.
 */
static etype_e handle_wait_ack_packet(single_list_t* node) {
    m1_packet_t* packet_node = single_list_entry(node, m1_packet_t, node);
    if (!packet_node) {
        link_error("Invalid packet node!");
        return E_STATE_NOT_EXIST; /*!< Return error if node is invalid */
    }

    /*! Free data if reference count reaches zero */
    if (--packet_node->data->reference_counter == 0) {
        MemoryPoolFree(m1.tx_pool, packet_node->data);
    }

    /*! Remove node from list */
    single_list_remove(&m1.wait_ack_packet_head, node);
    MemoryPoolFree(m1.tx_pool, packet_node); /*!< Free packet memory */

    return E_STATE_OK; /*!< Return success */
}

/**
 * \brief           Process acknowledgment packets received from other hosts.
 *
 * \param           frame_head: Pointer to the frame header of the
 *                  acknowledgment packet.
 * \return          Returns an error code of type `etype_e` indicating the
 *                  success or failure of the operation.
 *                   - `ETYPE_OK` on success.
 *                   - Other error codes indicating specific failures..
 */
static etype_e process_acknowledgment(m1_frame_head_t* frame_head) {
    single_list_t* current_node = m1.wait_ack_packet_head.next;
    while (current_node) {
        m1_packet_t* packet = single_list_entry(
            current_node, m1_packet_t, node); /*!< Get packet from node */
        if (packet->seq_num == frame_head->ack_num &&
            packet->target_id == frame_head->source_id &&
            packet->source_id == frame_head->target_id) {
            link_info(
                "receiver reliable ack from 0x%02x",
                frame_head->source_id); /*!< Log acknowledgment received */
            return handle_wait_ack_packet(
                current_node); /*!< Handle acknowledged packet */
        }
        current_node = current_node->next; /*!< Move to the next node */
    }

    link_warning("ACK for seq_num [%d] not found!",
                 frame_head->ack_num); /*!< Log missing acknowledgment */
    return E_STATE_ERROR; /*!< Return error if acknowledgment is not found */
}

/**
 * \brief           Send an acknowledgment packet to the source host.
 *
 * \param           frame_head: Pointer to the frame header of the received
 *                  packet.
 * \return          Returns an error code of type `etype_e` indicating the
 *                  success or failure of the operation.
 *                   - `ETYPE_OK` on success.
 *                   - Other error codes indicating specific failures.
 */
static etype_e send_ack_to_source_host(m1_frame_head_t* frame_head) {
    m1_packet_data_t packet_data = {0}; /*!< Initialize packet data */
    m1_packet_t packet = {
        .source_id = frame_head->target_id, /*!< Set source ID to target ID of
                                               received packet */
        .target_id = frame_head->source_id, /*!< Set target ID to source ID of
                                               received packet */
        .seq_num = 0,
        .ack_num = frame_head->seq_num,
        .version = frame_head->version,
        .reliable_tx = A1_RELIABLE_TX_ACK, /*!< Set acknowledgment type */
        .fragment = frame_head->attr.lsb.fragment,
        .encrypt = frame_head->attr.lsb.encrypt,
        .priority = frame_head->attr.lsb.priority,
        .compress = frame_head->attr.msb.compress,
        .data_type = frame_head->data_type,
        .data = &packet_data, /*!< Set data pointer to empty packet data */
    };
    return m1_network_send(&packet, false);
}

/**
 * \brief           Allocate memory for acknowledgment packet data.
 *
 * \param           src_data: Pointer to the source packet data.
 * \return          Pointer to the allocated acknowledgment packet data.
 */
static m1_packet_data_t* allocate_ack_data(m1_packet_data_t* src_data) {
    /*! Allocate memory for acknowledgment data */
    m1_packet_data_t* ack_data = (m1_packet_data_t*)MemoryPoolAlloc(
        m1.tx_pool, sizeof(m1_packet_data_t) + src_data->data_len);
    if (!ack_data) {
        link_error("Memory allocation failed for ACK data!");
        return NULL; /*!< Return NULL if memory allocation fails */
    }

    ack_data->data =
        (u8*)ack_data + sizeof(m1_packet_data_t); /*!< Assign data pointer */
    ack_data->data_len = src_data->data_len;      /*!< Set data length */
    memcpy(ack_data->data, src_data->data,
           src_data->data_len);      /*!< Copy data */
    ack_data->reference_counter = 0; /*!< Initialize reference counter */

    return ack_data; /*!< Return pointer to allocated acknowledgment data */
}

/* ----------------------------- end of file -------------------------------- */
