/**
 * \file            crc16.c
 * \brief           description
 * \date            2024-12-10
 */

/* includes ----------------------------------------------------------------- */
#include "./crc/crc16.h"

/* private variables -------------------------------------------------------- */
static const uint16_t crc16_modbus_tab[] = {
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
    0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400};

/* public functions --------------------------------------------------------- */
uint16_t crc16_find_tab_cal(uint8_t* buf, uint32_t len) {
    uint16_t crc = 0xFFFF;

    for (uint32_t i = 0; i < len; i++) {
        uint8_t data = *buf++;
        crc = crc16_modbus_tab[(data ^ crc) & 0x0F] ^ (crc >> 4);
        crc = crc16_modbus_tab[((data >> 4) ^ crc) & 0x0F] ^ (crc >> 4);
    }

    return crc;
}

void crc16_find_tab_pack(uint8_t* buf, uint32_t len) {
    uint16_t cksum = crc16_find_tab_cal(buf, len - 2);

    *(buf + len - 2) = cksum & 0xFF;
    *(buf + len - 1) = (cksum >> 8) & 0xFF;
}

bool crc16_find_tab_pack_check(uint8_t* buf, uint32_t len) {
    uint16_t crc_cur_val = (*(buf + len - 1) << 8) | (*(buf + len - 2));
    uint16_t crc_cal_val = crc16_find_tab_cal(buf, len - 2);
    if (crc_cur_val != crc_cal_val) {
        return false;
    }

    return true;
}

/* ----------------------------- end of file -------------------------------- */
