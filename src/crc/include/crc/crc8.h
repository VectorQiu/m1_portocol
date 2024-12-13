/**
 * \file            crc8.h
 * \brief           description
 * \date            2024-12-10
 */

#ifndef __CRC8_H__
#define __CRC8_H__

/* includes ----------------------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        crc8 manager
 * \brief           crc8 manager
 * \{
 */
// 常用CRC8参数模型 计算结果和(http://www.ip33.com/crc.html)计算结果一致
// 参数模型: CRC8/MAXIM
/* public functions --------------------------------------------------------- */
uint8_t crc8_find_tab_cal(uint8_t* buf, uint32_t len);
void crc8_find_tab_pack(uint8_t* buf, uint32_t len);
bool crc8_find_tab_pack_check(uint8_t* buf, uint32_t len);
/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CRC8_H__ */

/* ----------------------------- end of file -------------------------------- */
