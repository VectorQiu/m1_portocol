/**
 * \file            h1_protocol.h
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
#ifndef __H1_PROTOCOL_H__
#define __H1_PROTOCOL_H__

/* includes ----------------------------------------------------------------- */
#include "./h1_protocol/h1_cmd_common.h"
#include "./h1_protocol/h1_cmd_pillow.h"
#include "./h1_protocol/h1_protocol_def.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        h1 protocol manager
 * \brief           h1 protocol manager
 * \{
 */

/* Public functions --------------------------------------------------------- */
void h1_cmd_callback_handle(void* data);

etype_e h1_protocol_tx_data(u8 source_id, u8 target_id, bool reliable_tx,
                            u8* data, size_t data_len);
/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __H1_PROTOCOL_H__ */

/* ----------------------------- end of file -------------------------------- */
