/**
 * \file            h1_protocol_def.h
 * \brief           description
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
 * This file is part of the your library name library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         _version_
 */
#ifndef __H1_PROTOCOL_DEF_H__
#define __H1_PROTOCOL_DEF_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        h1 protocol def manager
 * \brief           h1 protocol def manager
 * \{
 */

/* Public configuration ----------------------------------------------------- */

/* Public definitions ------------------------------------------------------- */
#define HOST_ID_MASTER (0x10)
#define HOST_ID_SLAVE  (0x11)
#define HOST_ID_PC     (0x12)

/* Public typedefs ---------------------------------------------------------- */
typedef enum {
    H1_PROTO_TYPE_COMMON = 0,
    M1_PROTO_TYPE_PILLOW = 1,
} h1_protocol_type_e;

typedef enum h1_send_attr {
    H1_PROTO_SEND_ATTR_REQ = 0,
    H1_PROTO_SEND_ATTR_RESP = 1,
} h1_send_attr_e;

typedef enum h1_resp_attr_e {
    H1_PROTO_RESP_ATTR_NO_ACK = 0,     /*不响应*/
    H1_PROTO_RESP_ATTR_ACK_NOW = 1,    /*立即响应*/
    H1_PROTO_RESP_ATTR_ACK_FINISH = 2, /*执行完成后响应*/
} h1_resp_attr_e;

#pragma pack(1)

typedef struct h1_frame_head {
    u8 send_attr     : 2;
    u8 resp_attr     : 2;
    u8 reserved_attr : 4;
    u8 cmd_type;
    u8 cmd_id;
    u8 data[];
} h1_frame_head_t;

#pragma pack()

typedef struct {
    u8 cmd_type;
    u8 cmd_id;
    etype_e (*req_handle)(void* data);
    etype_e (*resp_handle)(void* data);
} h1_callback_table_t;

/* Public functions --------------------------------------------------------- */
#define H1_FRAME_PARSE(rx_data, data, frame_head, frame_data, frame_type)      \
    m1_rx_data_t* rx_data = data;                                              \
    h1_frame_head_t* frame_head = (h1_frame_head_t*)rx_data->data;             \
    frame_type* frame_data = (frame_type*)frame_head->data;                    \
    (void)rx_data;                                                             \
    (void)frame_head;                                                          \
    (void)frame_data;

#define H1_SEND(source_id, target_id, reliable_tx, data, data_type, cmd_type,  \
                cmd_id, req_attr, resp_attr)                                   \
    do {                                                                       \
        u8 frame_buf[sizeof(h1_frame_head_t) + sizeof(data_type)] = {0};       \
        h1_frame_head_t* frame_head = (h1_frame_head_t*)buffer;                \
        frame_head->send_attr = req_attr;                                      \
        frame_head->resp_attr = resp_attr;                                     \
        frame_head->cmd_type = cmd_type;                                       \
        frame_head->cmd_id = cmd_id;                                           \
        memcpy(frame_head->data, req, sizeof(req_type));                       \
        h1_protocol_tx_data(source_id, target_id, reliable_tx, frame_buf,      \
                            sizeof(frame_buf));                                \
    } while (0)

#define H1_SEND_REQ(source_id, target_id, data, data_type, cmd_type, cmd_id,   \
                    resp_attr)                                                 \
    H1_SEND(source_id, target_id, false, data, data_type, cmd_type, cmd_id,    \
            H1_PROTO_SEND_ATTR_REQ, resp_attr)

#define H1_SEND_RELIABLE_REQ(source_id, target_id, data, data_type, cmd_type,  \
                             cmd_id, resp_attr)                                \
    H1_SEND(source_id, target_id, true, data, data_type, cmd_type, cmd_id,     \
            H1_PROTO_SEND_ATTR_REQ, resp_attr)

#define H1_SEND_RESP(source_id, target_id, data, data_type, cmd_type, cmd_id,  \
                     resp_attr)                                                \
    H1_SEND(source_id, target_id, false, data, data_type, cmd_type, cmd_id,    \
            H1_PROTO_SEND_ATTR_RESP, resp_attr)

#define H1_SEND_RELIABLE_RESP(source_id, target_id, data, data_type, cmd_type, \
                              cmd_id, resp_attr)                               \
    H1_SEND(source_id, target_id, true, data, data_type, cmd_type, cmd_id,     \
            H1_PROTO_SEND_ATTR_RESP, resp_attr)
/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __H1_PROTOCOL_DEF_H__ */

/* ----------------------------- end of file -------------------------------- */
