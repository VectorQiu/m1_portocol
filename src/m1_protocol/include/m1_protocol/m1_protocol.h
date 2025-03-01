/**
 * \file            m1_protocol.h
 * \brief           Header file for the M1 protocol manager, providing functions
 *                  for initialization, data transmission, and periodic
 *                  operations.
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
#ifndef __M1_PROTOCOL_H__
#define __M1_PROTOCOL_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_protocol_def.h" /*!< Includes the definitions and data structures for the M1 protocol. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1_protocol_manager
 * \brief           M1 protocol manager for data communication.
 * \{
 */

/* public functions --------------------------------------------------------- */
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
                         size_t rx_cb_len, u8* source_id, size_t source_id_len);

/**
 * \brief           Transmit data using the M1 protocol.
 *
 * \param[in]       tx_data: Pointer to the structure containing data to be
 *                  transmitted.
 * \return          etype_e indicating success or failure of the transmission.
 */
etype_e m1_protocol_tx_data(m1_tx_data_t* tx_data);

/**
 * \brief           Run the periodic tasks of the M1 protocol.
 *
 * This function should be called regularly to handle time-dependent tasks such
 * as retransmissions, acknowledgments, and other protocol-specific processes.
 *
 * \param[in]       freq: Frequency of execution in Hz.
 */
void m1_protocol_run(u32 freq);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_PROTOCOL_H__ */

/* ----------------------------- end of file -------------------------------- */
