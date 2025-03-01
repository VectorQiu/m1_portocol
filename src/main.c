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

#include "./h1_protocol/h1_protocol.h"
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

etype_e master_send(u8* buf, size_t len);
etype_e master_recv(u8* buf, size_t* len);
etype_e salve_send(u8* buf, size_t len);
etype_e salve_recv(u8* buf, size_t* len);
void m1_callback(m1_rx_data_t* rx);
void h1_protocol_callback(m1_rx_data_t* rx);

tx_async_t master_tx = {
    .tx = master_send,
    .get_state = NULL,
};

rx_async_t master_rx = {
    .rx = master_recv,
};

tx_async_t salve_tx = {
    .tx = salve_send,
    .get_state = NULL,
};

rx_async_t salve_rx = {
    .rx = salve_recv,
};

m1_route_item_t route_table[] = {
    {
        .link_name = "uart1",
        .link_type = M1_LINK_TYPE_UART,
        .target_id = HOST_ID_MASTER,
        .host_name = "master",
        .tx = &master_tx,
        .rx = &master_rx,
        .read_freq = 1,
        .max_pkg_size = 256,
    },
    {
        .link_name = "uart2",
        .link_type = M1_LINK_TYPE_UART,
        .target_id = HOST_ID_SLAVE,
        .host_name = "slave",
        .tx = &salve_tx,
        .rx = &salve_rx,
        .read_freq = 1,
        .max_pkg_size = 128,
    },
};

m1_rx_parse_callback_item_t rx_callback_table[] = {
    {
        .data_type = M1_TRANSPORT_LAYER_PROTOCOL_TYPE,
        .cb = m1_callback,
    },
    {
        .data_type = H1_PROTOCOL_TYPE,
        .cb = h1_protocol_callback,
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

// master(0x10) -> pc(0x12) M1:
// 55 00 10 12 00 00 06 00 00 00 00 04 01 02 03 04 05 06 18 F7
u8 master_data_m1[20] = {
    0x55, 0x00, 0x10, 0x12, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
    0x00, 0x04, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x18, 0xF7,
};

// master(0x10) -> pc(0x12) M1: reliable
// 55 00 10 12 01 00 06 00 00 00 00 39 01 02 03 04 05 06 F4 F5
u8 master_data_m1_reliable[20] = {
    0x55, 0x00, 0x10, 0x12, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00,
    0x00, 0x39, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xF4, 0xF5,
};

// master(0x10) -> pc(0x12) H1: ping req
// 55 10 10 12 00 00 0A 00 00 00 00 69 00 00 00 06 01 02 03 04 05 06 7C 88
u8 master_data_h1_ping_req[24] = {
    0x55, 0x10, 0x10, 0x12, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x69,
    0x00, 0x00, 0x00, 0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x7C, 0x88,
};

// master(0x10) -> pc(0x12) H1: ping resp
// 55 10 10 12 00 00 0A 00 00 00 00 69 01 00 00 06 01 02 03 04 05 06 2D 4D
u8 master_data_h1_ping_resp[24] = {
    0x55, 0x10, 0x10, 0x12, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x69,
    0x01, 0x00, 0x00, 0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x2D, 0x4D,
};

// salve(0x11) -> pc(0x12) M1:
// 55 00 11 12 00 00 06 00 00 00 00 A0 01 02 03 04 05 06 3C AD
u8 salve_data_m1[20] = {
    0x55, 0x00, 0x11, 0x12, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
    0x00, 0xA0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x3C, 0xAD,
};

// salve(0x11) -> pc(0x12) M1: reliable
// 55 00 11 12 01 00 06 00 00 00 00 9D 01 02 03 04 05 06 D0 AF
u8 salve_data_m1_reliable[20] = {
    0x55, 0x00, 0x11, 0x12, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00,
    0x00, 0x9D, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xD0, 0xAF,
};

// salve(0x11) -> pc(0x12) H1: ping req
// 55 10 11 12 00 00 0A 00 00 00 00 CD 00 00 00 06 01 02 03 04 05 06 56 6B
u8 salve_data_h1_ping_req[24] = {
    0x55, 0x10, 0x11, 0x12, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0xCD,
    0x00, 0x00, 0x00, 0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x56, 0x6B,
};

// salve(0x11) -> pc(0x12) H1: ping resp
// 55 10 11 12 00 00 0A 00 00 00 00 CD 01 00 00 06 01 02 03 04 05 06 07 AE
u8 salve_data_h1_ping_resp[24] = {
    0x55, 0x10, 0x11, 0x12, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0xCD,
    0x01, 0x00, 0x00, 0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xAE,
};

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

    u8 source_id[] = {
        HOST_ID_PC,
    };
    m1_protocol_init("m1", 4096, route_table, ARRAY_SIZE(route_table),
                     rx_callback_table, ARRAY_SIZE(rx_callback_table),
                     source_id, ARRAY_SIZE(source_id));
    m1_tx_data_t tx_data = {0};
    u8 data[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    tx_data.data = data;
    tx_data.data_len = 10;
    tx_data.data_type = M1_TRANSPORT_LAYER_PROTOCOL_TYPE;
    tx_data.target_id_len = 1;
    tx_data.version = 0;
    tx_data.reliable_tx = M1_RELIABLE_TX;

    u8 target_id = HOST_ID_MASTER;
    tx_data.target_id = &target_id;
    m1_protocol_tx_data(&tx_data);

    target_id = HOST_ID_SLAVE;
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

etype_e master_send(u8* buf, size_t len) {
    LOG_HEX("master_send", 10, buf, len);
    return E_STATE_OK;
}

etype_e master_recv(u8* buf, size_t* len) {
    size_t data_len = sizeof(master_data_h1_ping_req);
    memcpy(buf, master_data_h1_ping_req, data_len);
    *len = data_len;
    return E_STATE_OK;
}

etype_e salve_send(u8* buf, size_t len) {
    LOG_HEX("salve_send", 10, buf, len);
    return E_STATE_OK;
}

etype_e salve_recv(u8* buf, size_t* len) {
    size_t data_len = sizeof(salve_data_h1_ping_req);
    memcpy(buf, salve_data_h1_ping_req, data_len);
    *len = data_len;
    return E_STATE_OK;
}

void m1_callback(m1_rx_data_t* rx) {
    (void)rx;
    printf("source id: 0x%x target id: 0x%x\n", rx->source_id, rx->target_id);
    LOG_HEX("m1_callback", 10, rx->data, rx->data_len);
}

void h1_protocol_callback(m1_rx_data_t* rx) {
    printf("source id: 0x%x target id: 0x%x\n", rx->source_id, rx->target_id);
    LOG_HEX("h1_protocol_callback", 10, rx->data, rx->data_len);
    h1_cmd_callback_handle(NULL, rx);
}

/* ----------------------------- end of file -------------------------------- */
