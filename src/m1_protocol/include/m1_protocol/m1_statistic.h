/**
 * \file            m1_statistic.h
 * \brief           M1 protocol statistic management header file.
 * \details         This file contains macros, structures, and function
 *                  declarations for managing and tracking statistics related to
 *                  the M1 protocol.
 * \date            2024-12-13
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
#ifndef __M1_STATISTIC_H__
#define __M1_STATISTIC_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1_statistic_manager
 * \brief           M1 statistic management module.
 * \details         This module provides functionality for tracking various
 *                  statistics during the processing of M1 protocol frames.
 * \{
 */

/* public config ------------------------------------------------------------ */

/* public define ------------------------------------------------------------ */

/**
 * \brief           Increment total bytes received for a node.
 * \param[in]       node: Pointer to the node object.
 * \param[in]       len: Number of bytes to increment.
 */
#define M1_STATS_RX_NODE_TOTAL_BYTES(node, len)                                \
    ((node)->stats.total_bytes += (len))

/**
 * \brief           Increment count of bytes that are not part of a valid frame.
 * \param[in]       node: Pointer to the node object.
 */
#define M1_STATS_RX_NODE_NOT_FRAME_BYTES(node) ((node)->stats.not_frame_bytes++)

/**
 * \brief           Increment count of Start-of-Frame (SOF) successfully
 * detected.
 * \param[in]       node: Pointer to the node object.
 */
#define M1_STATS_RX_NODE_SOF_OK(node)          ((node)->stats.sof_ok_cnt++)

/**
 * \brief           Increment count of CRC8 checks successfully passed.
 * \param[in]       node: Pointer to the node object.
 */
#define M1_STATS_RX_NODE_CRC8_OK(node)         ((node)->stats.crc8_ok_cnt++)

/**
 * \brief           Increment count of CRC8 check failures.
 * \param[in]       node: Pointer to the node object.
 */
#define M1_STATS_RX_NODE_CRC8_ERR(node)        ((node)->stats.crc8_err_cnt++)

/**
 * \brief           Increment count of CRC16 checks successfully passed.
 * \param[in]       node: Pointer to the node object.
 */
#define M1_STATS_RX_NODE_CRC16_OK(node)        ((node)->stats.crc16_ok_cnt++)

/**
 * \brief           Increment count of CRC16 check failures.
 * \param[in]       node: Pointer to the node object.
 */
#define M1_STATS_RX_NODE_CRC16_ERR(node)       ((node)->stats.crc16_err_cnt++)

/**
 * \brief           Increment count of length overflows detected.
 * \param[in]       node: Pointer to the node object.
 */
#define M1_STATS_RX_NODE_LEN_OVERFLOW(node)    ((node)->stats.len_overflow_cnt++)

/* public typedef struct ---------------------------------------------------- */
/**
 * \brief           Structure to store statistics for parsing M1 protocol
 * frames.
 */
typedef struct m1_stats_rx_parse {
    u32 total_bytes;     /*!< Total bytes processed. */
    u32 not_frame_bytes; /*!< Bytes not part of valid frames. */
    u32 sof_ok_cnt; /*!< Count of successfully detected Start-of-Frame (SOF). */
    u32 crc8_ok_cnt;      /*!< Count of successfully passed CRC8 checks. */
    u32 crc8_err_cnt;     /*!< Count of CRC8 check failures. */
    u32 crc16_ok_cnt;     /*!< Count of successfully passed CRC16 checks. */
    u32 crc16_err_cnt;    /*!< Count of CRC16 check failures. */
    u32 len_overflow_cnt; /*!< Count of length overflows. */
} m1_stats_rx_parse_t;

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_STATISTIC_H__ */

/* ----------------------------- end of file -------------------------------- */
