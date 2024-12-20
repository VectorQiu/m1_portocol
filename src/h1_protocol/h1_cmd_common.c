/**
 * \file            h1_cmd_common.c
 * \brief           H1 common command set
 * \date            2024-12-18
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
#include "./h1_protocol/h1_cmd_common.h"


/* Public functions --------------------------------------------------------- */
etype_e h1_cmd_ping_req(void* data) {
    H1_FRAME_PARSE(rx_data, data, frame_head, ping_req, h1_cmd_ping_req_t);

    link_hex("Ping Req", 16, ping_req->data, ping_req->size);

    frame_head->send_attr = H1_PROTO_SEND_ATTR_RESP;
    h1_protocol_tx_data(rx_data->target_id, rx_data->source_id, false,
                        (u8*)frame_head, rx_data->data_len);

    return E_STATE_OK;
}

etype_e h1_cmd_ping_resp(void* data) {
    H1_FRAME_PARSE(rx_data, data, frame_head, ping_resp, h1_cmd_ping_resp_t);
    link_hex("Ping Resp", 16, ping_resp->data, ping_resp->size);
    return E_STATE_OK;
}

/* ----------------------------- end of file -------------------------------- */
