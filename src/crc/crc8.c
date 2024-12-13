/**
 * \file            crc8.c
 * \brief           description
 * \date            2024-12-10
 */

/* includes ----------------------------------------------------------------- */
#include "./crc/crc8.h"

/* private variables -------------------------------------------------------- */
static const uint8_t crc8_maxim_tab[] = {0x00, 0x9d, 0x23, 0xbe, 0x46, 0xdb,
                                         0x65, 0xf8, 0x8c, 0x11, 0xaf, 0x32,
                                         0xca, 0x57, 0xe9, 0x74};

/* public functions --------------------------------------------------------- */
uint8_t crc8_find_tab_cal(uint8_t* buf, uint32_t len) {
    uint8_t crc = 0x00;
    for (uint32_t i = 0; i < len; i++) {
        uint8_t data = *buf++;
        crc = crc8_maxim_tab[(data ^ crc) & 0x0F] ^ (crc >> 4);
        crc = crc8_maxim_tab[((data >> 4) ^ crc) & 0x0F] ^ (crc >> 4);
    }
    return crc;
}

void crc8_find_tab_pack(uint8_t* buf, uint32_t len) {
    uint8_t cksum = crc8_find_tab_cal(buf, len - 1);

    *(buf + len - 1) = cksum & 0xFF;
}

bool crc8_find_tab_pack_check(uint8_t* buf, uint32_t len) {
    uint8_t crc_cur_val = *(buf + len - 1);
    uint8_t crc_cal_val = crc8_find_tab_cal(buf, len - 1);
    if (crc_cur_val != crc_cal_val) {
        return false;
    }

    return true;
}

/* ----------------------------- end of file -------------------------------- */
