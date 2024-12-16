/**
 * \file            main.c
 * \brief           main
 * \date            2024-10-29
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

/* includes ----------------------------------------------------------------- */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "./m1_protocol/m1_protocol.h"

#define dbg_raw(...) printf(__VA_ARGS__)

#define LOG_HEX(name, width, buf, size)                                        \
    do {                                                                       \
        dbg_raw("%s:\n", name);                                                \
        for (size_t i = 0; i < size; i++) {                                    \
            dbg_raw("%02X ", ((unsigned char*)buf)[i]);                        \
            if ((i + 1) % width == 0) {                                        \
                dbg_raw("\n");                                                 \
            }                                                                  \
        }                                                                      \
        if (size % width != 0) {                                               \
            dbg_raw("\n");                                                     \
        }                                                                      \
    } while (0)

etype_e debug_send(u8* buf, size_t len);
etype_e debug_recv(u8* buf, size_t* len);
etype_e pc_send(u8* buf, size_t len);
etype_e pc_recv(u8* buf, size_t* len);
void rx_m1_cb(m1_rx_data_t* rx);

tx_async_t debug_tx = {
    .tx = debug_send,
    .get_state = NULL,
};

rx_async_t debug_rx = {
    .rx = debug_recv,
};

tx_async_t pc_tx = {
    .tx = pc_send,
    .get_state = NULL,
};

rx_async_t pc_rx = {
    .rx = pc_recv,
};

m1_route_item_t route_table[] = {
    {
        .link_name = "uart1",
        .link_type = M1_LINK_TYPE_UART,
        .target_id = 0x01,
        .host_name = "main",
        .tx = &debug_tx,
        .rx = &debug_rx,
        .read_freq = 1,
        .max_pkg_size = 256,
    },
    {
        .link_name = "uart2",
        .link_type = M1_LINK_TYPE_UART,
        .target_id = 0x02,
        .host_name = "pc",
        .tx = &pc_tx,
        .rx = &pc_rx,
        .read_freq = 1,
        .max_pkg_size = 128,
    },
};

m1_rx_parse_callback_item_t rx_callback_table[] = {
    {
        .data_type = M1_TRANSPORT_LAYER_PROTOCOL_TYPE,
        .cb = rx_m1_cb,
    },
};

void* thread_function(void* arg) {
    (void)arg;

    u32 run_cnt = 0;
    while (1) {
        m1_protocol_run(2);

        run_cnt++;
        usleep(1000 * 1000 * 1000 / 500);
    }
}

// 传输:55 00 01 F1 00 00 06 00 00 00 00 AC 01 02 03 04 05 06 5E 28
// 可靠传输:55 00 01 F1 01 00 06 00 00 00 00 91 01 02 03 04 05 06 B2 2A
u8 rx1_data[20] = {0x55, 0x00, 0x01, 0xF1, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
                   0x00, 0xAC, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x5E, 0x28};
u8 reliable_data[20] = {0x55, 0x00, 0x01, 0xF1, 0x01, 0x00, 0x06,
                        0x00, 0x00, 0x00, 0x00, 0x91, 0x01, 0x02,
                        0x03, 0x04, 0x05, 0x06, 0xB2, 0x2A};

u8 rx2_data[20] = {0x55, 0x00, 0x02, 0xF1, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
                   0x00, 0x59, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xBB, 0xD6};

/* public functions --------------------------------------------------------- */
int main(int argc, char* argv[]) {
    printf("Number of arguments: %d\n", argc);
    printf("Arguments:\n");

    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    pthread_t thread;

    // 创建线程
    if (pthread_create(&thread, NULL, thread_function, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return EXIT_FAILURE;
    }

    // 将线程设置为分离状态
    if (pthread_detach(thread)) {
        fprintf(stderr, "Error detaching thread\n");
        return EXIT_FAILURE;
    }

    u8 source_id[2] = {0xF1, 0xF2};
    m1_protocol_init("m1", 4096, route_table, 2, rx_callback_table, 1,
                     source_id, 2);
    m1_tx_data_t tx_data = {0};
    u8 data[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    tx_data.data = data;
    tx_data.data_len = 10;
    tx_data.data_type = M1_TRANSPORT_LAYER_PROTOCOL_TYPE;
    tx_data.target_id_len = 1;
    tx_data.version = 0;
    tx_data.reliable_tx = M1_RELIABLE_TX;

    u8 target_id = 0x01;
    tx_data.target_id = &target_id;
    m1_protocol_tx_data(&tx_data);

    target_id = 0x02;
    tx_data.target_id = &target_id;
    m1_protocol_tx_data(&tx_data);

    u32 run_cnt = 0;
    while (1) {
        // if (run_cnt % 10 == 0) {
        //     u8 target_id = 0x01;
        //     tx_data.target_id = &target_id;
        //     m1_protocol_tx_data(&tx_data);
        // } else if (run_cnt % 5 == 0) {
        //     u8 target_id = 0x02;
        //     tx_data.target_id = &target_id;
        //     m1_protocol_tx_data(&tx_data);
        // }
        sleep(1);
        run_cnt++;
    }
    return 0;
}

etype_e debug_send(u8* buf, size_t len) {
    LOG_HEX("debug_send", 10, buf, len);
    return E_STATE_OK;
}

etype_e debug_recv(u8* buf, size_t* len) {
    memcpy(buf, reliable_data, 20);
    *len = 20;
    return E_STATE_OK;
}

etype_e pc_send(u8* buf, size_t len) {
    LOG_HEX("pc_send", 10, buf, len);
    return E_STATE_OK;
}

etype_e pc_recv(u8* buf, size_t* len) {
    memcpy(buf, rx2_data, 20);
    *len = 20;
    return E_STATE_OK;
}

void rx_m1_cb(m1_rx_data_t* rx) {
    (void)rx;
    printf("source id: 0x%x target id: 0x%x\n", rx->source_id, rx->target_id);
    LOG_HEX("rx_m1_cb", 10, rx->data, rx->data_len);
}

/* ----------------------------- end of file -------------------------------- */
