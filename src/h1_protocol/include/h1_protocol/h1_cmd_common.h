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
#define H1_CMD_ID_PING (0x00)

/* Public typedefs ---------------------------------------------------------- */
#pragma pack(1)

typedef struct {
    uint8_t size;
    uint8_t data[];
} h1_cmd_ping_req_t;

typedef struct {
    uint8_t size;
    uint8_t data[];
} h1_cmd_ping_resp_t;

#pragma pack()
/* Public functions --------------------------------------------------------- */
etype_e h1_cmd_ping_req(void* data);
etype_e h1_cmd_ping_resp(void* data);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __H1_CMD_COMMON_H__ */

/* ----------------------------- end of file -------------------------------- */
