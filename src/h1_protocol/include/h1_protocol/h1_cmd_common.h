/**
 * \file            h1_cmd_common.h
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
#ifndef __H1_CMD_COMMON_H__
#define __H1_CMD_COMMON_H__

/* includes ----------------------------------------------------------------- */
#include "./h1_protocol/h1_protocol_def.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        h1 cmd common manager
 * \brief           h1 cmd common manager
 * \{
 */

/* Public definitions ------------------------------------------------------- */
#define H1_CMD_ID_PING              (0x00)
#define H1_CMD_ID_SET_MODULE_STATUS (0x10)
#define H1_CMD_ID_GET_MODULE_STATUS (0x11)
#define H1_CMD_ID_SET_DEVICE_INFO   (0x12)
#define H1_CMD_ID_GET_DEVICE_INFO   (0x13)

/* Public typedefs ---------------------------------------------------------- */
#pragma pack(1)

typedef struct {
    u8 size;
    u8 data[];
} h1_cmd_ping_req_t;

typedef struct {
    u8 size;
    u8 data[];
} h1_cmd_ping_resp_t;

typedef enum {
    H1_MODULE_ID_NONE = 0,
} h1_module_id_t;

typedef struct {
    u8 id;     /*模块ID*/
    u8 status; /*模块状态*/
    i32 args;  /*模块状态补充参数*/
} h1_set_module_status_req_t;

typedef struct {
    u8 result; /*0 成功 other 失败*/
    u8 id;     /*模块ID*/
} h1_set_module_status_resp_t;

typedef struct {
    u8 id; /*模块ID*/
} h1_get_module_status_req_t;

typedef struct {
    u8 result; /*0 成功 other 失败*/
    u8 id;     /*模块ID*/
    u8 status; /*模块状态*/
    i32 args;  /*模块状态补充参数*/
} h1_get_module_status_resp_t;

typedef enum {
    H1_DEVICE_INFO_ID_NONE = 0,
} h1_device_info_id_t;

typedef struct {
    u8 id;
    u8 size;
    u8 data[];
} h1_set_device_info_req_t;

typedef struct {
    u8 result;
    u8 id;
} h1_set_device_info_resp_t;

typedef struct {
    u8 id;
} h1_get_device_info_req_t;

typedef struct {
    u8 result;
    u8 id;
    u8 size;
    u8 data[];
} h1_get_device_info_resp_t;

#pragma pack()
/* Public functions --------------------------------------------------------- */
etype_e h1_cmd_ping_req(void* param, void* data);
etype_e h1_cmd_ping_resp(void* param, void* data);

etype_e h1_cmd_set_module_status_req(void* param, void* data);
etype_e h1_cmd_set_module_status_resp(void* param, void* data);
etype_e h1_cmd_get_module_status_req(void* param, void* data);
etype_e h1_cmd_get_module_status_resp(void* param, void* data);

etype_e h1_cmd_set_device_info_req(void* param, void* data);
etype_e h1_cmd_set_device_info_resp(void* param, void* data);
etype_e h1_cmd_get_device_info_req(void* param, void* data);
etype_e h1_cmd_get_device_info_resp(void* param, void* data);

etype_e h1_set_module_status(u8 target_id, h1_set_module_status_req_t* req);
etype_e h1_get_module_status(u8 target_id, h1_get_module_status_req_t* req);

etype_e h1_set_device_info(u8 target_id, h1_set_device_info_req_t* req);
etype_e h1_get_device_info(u8 target_id, h1_get_device_info_req_t* req);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __H1_CMD_COMMON_H__ */

/* ----------------------------- end of file -------------------------------- */
