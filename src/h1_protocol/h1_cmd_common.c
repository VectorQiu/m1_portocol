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

/* Private function prototypes ---------------------------------------------- */
extern etype_e h1_protocol_tx_data(u8 source_id, u8 target_id, bool reliable_tx,
                                   u8* data, size_t data_len);

/* Public functions --------------------------------------------------------- */
etype_e h1_cmd_ping_req(void* param, void* data) {
    (void)param;
    H1_FRAME_PARSE(rx_data, data, frame_head, ping_req, h1_cmd_ping_req_t);

    link_hex("Ping Req", 16, ping_req->data, ping_req->size);

    frame_head->send_attr = H1_PROTO_SEND_ATTR_RESP;
    h1_protocol_tx_data(rx_data->target_id, rx_data->source_id, false,
                        (u8*)frame_head, rx_data->data_len);

    return E_STATE_OK;
}

etype_e h1_cmd_ping_resp(void* param, void* data) {
    (void)param;
    H1_FRAME_PARSE(rx_data, data, frame_head, ping_resp, h1_cmd_ping_resp_t);
    link_hex("Ping Resp", 16, ping_resp->data, ping_resp->size);
    return E_STATE_OK;
}

etype_e h1_cmd_set_module_status_req(void* param, void* data) {
    (void)param;
    H1_FRAME_PARSE(rx_data, data, frame_head, req, h1_set_module_status_req_t);

    h1_set_module_status_resp_t resp = {0};
    resp.id = req->id;
    resp.result = 1;
    switch (req->id) {
        case H1_MODULE_ID_NONE:
            /* code */
            break;

        default:
            break;
    }

    H1_SEND_RESP(rx_data->target_id, rx_data->source_id, &resp,
                 h1_set_module_status_resp_t, frame_head->cmd_type,
                 frame_head->cmd_id, H1_PROTO_RESP_ATTR_NO_ACK);
    return E_STATE_OK;
}

etype_e h1_cmd_set_module_status_resp(void* param, void* data) {
    (void)param;
    H1_FRAME_PARSE(rx_data, data, frame_head, resp,
                   h1_set_module_status_resp_t);

    return E_STATE_OK;
}

etype_e h1_cmd_get_module_status_req(void* param, void* data) {
    (void)param;
    H1_FRAME_PARSE(rx_data, data, frame_head, req, h1_get_module_status_req_t);

    h1_get_module_status_resp_t resp = {0};
    resp.id = req->id;
    resp.result = 1;
    switch (req->id) {
        case H1_MODULE_ID_NONE:
            /* code */
            break;

        default:
            break;
    }

    H1_SEND_RESP(rx_data->target_id, rx_data->source_id, &resp,
                 h1_get_module_status_resp_t, frame_head->cmd_type,
                 frame_head->cmd_id, H1_PROTO_RESP_ATTR_NO_ACK);
    return E_STATE_OK;
}

etype_e h1_cmd_get_module_status_resp(void* param, void* data) {
    (void)param;
    H1_FRAME_PARSE(rx_data, data, frame_head, resp,
                   h1_get_module_status_resp_t);

    if (resp->result != 0) {
        return E_STATE_ERROR;
    }

    switch (resp->id) {
        case H1_MODULE_ID_NONE:
            /* code */
            break;

        default:
            break;
    }

    return E_STATE_OK;
}

etype_e h1_cmd_set_device_info_req(void* param, void* data) {
    (void)param;
    H1_FRAME_PARSE(rx_data, data, frame_head, req, h1_set_device_info_req_t);

    h1_set_device_info_resp_t resp = {0};
    resp.id = req->id;
    resp.result = 1;

    switch (req->id) {
        case H1_DEVICE_INFO_ID_NONE:
            /* code */
            break;

        default:
            break;
    }
    H1_SEND_RESP(rx_data->target_id, rx_data->source_id, &resp,
                 h1_set_device_info_resp_t, frame_head->cmd_type,
                 frame_head->cmd_id, H1_PROTO_RESP_ATTR_NO_ACK);
    return E_STATE_OK;
}

etype_e h1_cmd_set_device_info_resp(void* param, void* data) {
    (void)param;
    H1_FRAME_PARSE(rx_data, data, frame_head, resp, h1_set_device_info_resp_t);

    return E_STATE_OK;
}

etype_e h1_cmd_get_device_info_req(void* param, void* data) {
    (void)param;
    H1_FRAME_PARSE(rx_data, data, frame_head, req, h1_get_device_info_req_t);

    u8 resp_frame_buf[sizeof(h1_frame_head_t) + 64] = {0};
    memset(resp_frame_buf, 0, sizeof(resp_frame_buf));
    h1_frame_head_t* resp_frame_head = (h1_frame_head_t*)resp_frame_buf;
    resp_frame_head->send_attr = H1_PROTO_SEND_ATTR_RESP;
    resp_frame_head->resp_attr = H1_PROTO_RESP_ATTR_NO_ACK;
    resp_frame_head->cmd_type = H1_PROTO_TYPE_COMMON;
    resp_frame_head->cmd_id = H1_CMD_ID_GET_DEVICE_INFO;
    h1_get_device_info_resp_t* resp =
        (h1_get_device_info_resp_t*)resp_frame_head->data;

    resp->id = req->id;
    resp->result = 1;
    switch (req->id) {
        case H1_DEVICE_INFO_ID_NONE:
            /* code */
            break;

        default:
            break;
    }

    if (strlen((char*)resp->data) > 0) {
        resp->size = strlen((char*)resp->data) + 1;
    } else {
        resp->size = 0;
    }
    u8 data_len = sizeof(h1_get_device_info_resp_t) + resp->size;
    return h1_protocol_tx_data(rx_data->target_id, rx_data->source_id, false,
                               resp_frame_buf,
                               sizeof(h1_frame_head_t) + data_len);
}

etype_e h1_cmd_get_device_info_resp(void* param, void* data) {
    (void)param;
    H1_FRAME_PARSE(rx_data, data, frame_head, resp, h1_get_device_info_resp_t);

    if (!(resp->result =
              0 && resp->size > 0 && strlen((char*)resp->data) > 0)) {
        return E_STATE_ERROR;
    }

    switch (resp->id) {
        case H1_DEVICE_INFO_ID_NONE:
            /* code */
            break;

        default:
            break;
    }

    return E_STATE_OK;
}

etype_e h1_set_module_status(u8 target_id, h1_set_module_status_req_t* req) {
    H1_SEND_REQ(HOST_ID_SOURCE, target_id, req, h1_set_module_status_req_t,
                H1_PROTO_TYPE_COMMON, H1_CMD_ID_SET_MODULE_STATUS,
                H1_PROTO_RESP_ATTR_ACK_NOW);
    return E_STATE_OK;
}

etype_e h1_get_module_status(u8 target_id, h1_get_module_status_req_t* req) {
    H1_SEND_REQ(HOST_ID_SOURCE, target_id, req, h1_get_module_status_req_t,
                H1_PROTO_TYPE_COMMON, H1_CMD_ID_GET_MODULE_STATUS,
                H1_PROTO_RESP_ATTR_ACK_NOW);
    return E_STATE_OK;
}

etype_e h1_set_device_info(u8 target_id, h1_set_device_info_req_t* req) {
    u16 data_len = sizeof(h1_set_device_info_req_t) + req->size;
    if (req->size > 64) {
        return E_STATE_NO_SPACE;
    }
    u8 frame_buf[sizeof(h1_frame_head_t) + 64] = {0};
    h1_frame_head_t* frame_head = (h1_frame_head_t*)frame_buf;
    frame_head->send_attr = H1_PROTO_SEND_ATTR_REQ;
    frame_head->resp_attr = H1_PROTO_RESP_ATTR_ACK_NOW;
    frame_head->cmd_type = H1_PROTO_TYPE_COMMON;
    frame_head->cmd_id = H1_CMD_ID_SET_DEVICE_INFO;
    memcpy(frame_head->data, req, data_len);
    return h1_protocol_tx_data(HOST_ID_SOURCE, target_id, false, frame_buf,
                               sizeof(h1_frame_head_t) + data_len);
}

etype_e h1_get_device_info(u8 target_id, h1_get_device_info_req_t* req) {
    u8 frame_buf[sizeof(h1_frame_head_t) + sizeof(h1_get_device_info_req_t)] = {
        0};
    h1_frame_head_t* frame_head = (h1_frame_head_t*)frame_buf;
    frame_head->send_attr = H1_PROTO_SEND_ATTR_REQ;
    frame_head->resp_attr = H1_PROTO_RESP_ATTR_ACK_NOW;
    frame_head->cmd_type = H1_PROTO_TYPE_COMMON;
    frame_head->cmd_id = H1_CMD_ID_GET_DEVICE_INFO;
    memcpy(frame_head->data, req, sizeof(h1_get_device_info_req_t));
    return h1_protocol_tx_data(HOST_ID_SOURCE, target_id, false, frame_buf,
                               sizeof(frame_buf));
}

/* Private functions -------------------------------------------------------- */

/* ----------------------------- end of file -------------------------------- */
