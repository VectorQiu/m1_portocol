/**
 * \file            m1_format_frame.h
 * \brief           Header file for defining M1 protocol frame structures and
 *                  attributes.
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
#ifndef __M1_FORMAT_FRAME_H__
#define __M1_FORMAT_FRAME_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/m1_typedef.h" /*!< Defines the types used in the M1 protocol. */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        m1_format_frame_manager
 * \brief           M1 protocol frame formatting manager.
 * \{
 */

/**
 * \brief           Start-of-frame (SOF) byte for M1 frames.
 */
#define M1_FRAME_HEAD_SOF (0x55)

/**
 * \brief           Enumeration for M1 frame protocol versions.
 */
typedef enum {
    M1_FRAME_VERSION_0 = 0, /*!< Protocol version 0. */
    M1_FRAME_VERSION_1 = 1, /*!< Protocol version 1. */
} m1_frame_version_e;

/**
 * \brief           Enumeration for reliable transmission flags.
 */
typedef enum {
    M1_RELIABLE_NONE = 0,   /*!< No reliable transmission. */
    M1_RELIABLE_TX = 1,     /*!< Reliable transmission required. */
    A1_RELIABLE_TX_ACK = 2, /*!< Reliable transmission with acknowledgment. */
} m1_reliable_tx_e;

/**
 * \brief           Enumeration for data fragmentation flags.
 */
typedef enum {
    M1_FRAGMENT_NONE = 0,   /*!< No fragmentation. */
    M1_FRAGMENT_ENABLE = 1, /*!< Fragmentation enabled. */
} m1_fragment_e;

/**
 * \brief           Enumeration for encryption flags.
 */
typedef enum {
    M1_ENCRYPT_NONE = 0, /*!< No encryption. */
} m1_encrypt_e;

/**
 * \brief           Enumeration for data priority levels.
 */
typedef enum {
    M1_PRIORITY_NONE = 0, /*!< Default priority. */
} m1_priority_e;

/**
 * \brief           Enumeration for compression flags.
 */
typedef enum {
    M1_COMPRESS_NONE = 0, /*!< No compression. */
} m1_compress_e;

/**
 * \brief           Enumeration for data types in M1 protocol.
 */
typedef enum {
    M1_TRANSPORT_LAYER_PROTOCOL_TYPE = 0, /*!< Transport layer protocol type. */
    M1_DATA_TYPE_MAX,                     /*!< Maximum data type value. */
} m1_data_type_e;

#pragma pack(1) /*!< Ensure structures are packed without padding. */

/**
 * \brief           Structure for low significant byte (LSB) attributes of a
 *                  frame.
 */
typedef union {
    u8 byte; /*!< Raw byte representing all LSB attributes. */

    struct {
        u8 reliable : 2; /*!< Reliable transmission flag. */
        u8 fragment : 1; /*!< Fragmentation flag. */
        u8 encrypt  : 2; /*!< Encryption flag. */
        u8 priority : 3; /*!< Data priority level. */
    };
} m1_frame_attr_lsb_t;

/**
 * \brief           Structure for most significant byte (MSB) attributes of a
 *                  frame.
 */
typedef union {
    u8 byte; /*!< Raw byte representing all MSB attributes. */

    struct {
        u8 compress : 2; /*!< Compression flag. */
        u8 reserved : 6; /*!< Reserved bits for future use. */
    };
} m1_frame_attr_msb_t;

/**
 * \brief           Combined structure for frame attributes.
 */
typedef struct {
    m1_frame_attr_lsb_t lsb; /*!< Low significant byte attributes. */
    m1_frame_attr_msb_t msb; /*!< Most significant byte attributes. */
} m1_frame_attr_t;

/**
 * \brief           Structure representing the frame header of an M1 protocol
 * frame.
 */
typedef struct m1_frame_head {
    u8 sof;               /*!< Start-of-frame (SOF) byte. */
    u8 version   : 4;     /*!< Protocol version (4 bits). */
    u8 data_type : 4;     /*!< Data type (4 bits). */
    u8 source_id;         /*!< Source device ID. */
    u8 target_id;         /*!< Target device ID. */
    m1_frame_attr_t attr; /*!< Frame attributes (e.g., reliable, fragmented). */
    u8 data_len_lsb;      /*!< LSB of the data length. */
    u8 data_len_msb;      /*!< MSB of the data length. */
    u8 seq_num;           /*!< Sequence number for the frame. */
    u8 ack_num;           /*!< Acknowledgment number. */
    u8 reserved;          /*!< Reserved byte for future use. */
    u8 crc8;              /*!< CRC8 checksum for the header. */
} m1_frame_head_t;

/**
 * \brief           Structure representing the frame tail of an M1 protocol
 *                  frame.
 */
typedef struct m1_frame_tail {
    u8 crc16_lsb; /*!< LSB of the CRC16 checksum for the frame. */
    u8 crc16_msb; /*!< MSB of the CRC16 checksum for the frame. */
} m1_frame_tail_t;

#pragma pack() /*!< End of packed structure definition. */

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __M1_FORMAT_FRAME_H__ */

/* ----------------------------- end of file -------------------------------- */