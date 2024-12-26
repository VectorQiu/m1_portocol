/**
 * \file            h1_protocol.c
 * \brief           H1 Protocol
 * \date            2024-12-17
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
 * This file is part of the H1 Protocol library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         v0.0.1
 */
/* includes ----------------------------------------------------------------- */
#include "./h1_protocol/h1_protocol.h"
/* Private configuration ---------------------------------------------------- */

/* Private definitions ------------------------------------------------------ */

/* Private typedefs --------------------------------------------------------- */
/* Private function prototypes ---------------------------------------------- */

/* Private variables -------------------------------------------------------- */
h1_callback_table_t h1_callback_table[] = {
    {H1_PROTO_TYPE_COMMON, H1_CMD_ID_PING, h1_cmd_ping_req, h1_cmd_ping_resp},
};

/* Public variables --------------------------------------------------------- */

/* Public functions --------------------------------------------------------- */
void h1_cmd_callback_handle(void* data) {
    m1_rx_data_t* rx_data = data;
    if (rx_data->data_len < sizeof(h1_frame_head_t)) {
        return;
    }

    h1_frame_head_t* head = (h1_frame_head_t*)(rx_data->data);
    for (size_t i = 0; i < ARRAY_SIZE(h1_callback_table); i++) {
        if ((head->cmd_type == h1_callback_table[i].cmd_type)
            && (head->cmd_id == h1_callback_table[i].cmd_id)) {
            if (head->send_attr == H1_PROTO_SEND_ATTR_REQ) {
                if (h1_callback_table[i].req_handle) {
                    h1_callback_table[i].req_handle(data);
                }
            } else if (head->send_attr == H1_PROTO_SEND_ATTR_RESP) {
                if (h1_callback_table[i].resp_handle) {
                    h1_callback_table[i].resp_handle(data);
                }
            }
        }
    }
}

etype_e h1_protocol_tx_data(u8 source_id, u8 target_id, bool reliable_tx,
                            u8* data, size_t data_len) {
    m1_tx_data_t tx_data = {0};

    tx_data.version = M1_FRAME_VERSION_0;
    tx_data.source_id = source_id;
    tx_data.target_id = &target_id;
    tx_data.target_id_len = 1;
    tx_data.reliable_tx = reliable_tx ? M1_RELIABLE_TX : M1_RELIABLE_NONE;
    tx_data.data_type = H1_PROTOCOL_TYPE;
    tx_data.data = data;
    tx_data.data_len = data_len;

    return m1_protocol_tx_data(&tx_data);
}

/* Private functions -------------------------------------------------------- */

/* ----------------------------- end of file -------------------------------- */
