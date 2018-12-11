/*******************************************************************************
 Copyright SAKC Corporation. 2016. All rights reserved.
--------------------------------------------------------------------------------
    File name    : sys_cfg.h
    Project name : ��������ֲģ�顣
    Module name  : 
    Date created : 2017��8��17��   10ʱ21��23��
    Author       : Ning.JianLi
    Description  : STM32ϵͳ���ýӿ�
*******************************************************************************/

#ifndef __SYS_CFG_H__
#define __SYS_CFG_H__

#include <stdio.h>
#include "cpu.h"
#include "stm32f4xx.h"
#include "stm32f429_eth.h"
#include "lwip/ip_addr.h"
#include "netif.h"
#include "tcpip.h"
#include "ethernetif.h"
#include "pub_type.h"
#include "sys_interrupt.h"

#ifdef __cplusplus
extern "c" {
#endif /* __cplusplus */

/*  PLL_M = 25, PLL_N = 336, PLL_P = 2, PLL_Q = 7
 *  PLLCLK    = HSE * (PLLN / PLLM)      = 336MHz.
 *  SYSCLK    = PLLCLK / PLLP            = 168MHz.
 *  OTG_FSCLK = PLLCLK / PLLQ            =  48MHz.
 */

/* VCO��Ƶϵ�� */
#define SYS_PLL_CONFIG_INFO_PLLM              25

/* VCO��Ƶϵ�� */
#define SYS_PLL_CONFIG_INFO_PLLN              336

/* ����Ƶϵ��(ϵͳʱ��) */
#define SYS_PLL_CONFIG_INFO_PLLP              2

/* ����Ƶϵ��(����USB OTG FS��SDIO��RNG) */
#define SYS_PLL_CONFIG_INFO_PLLQ              7

#define SYS_REG_DEM_CR                        (*(CPU_REG32 *)0xE000EDFC)
#define SYS_REG_DWT_CR                        (*(CPU_REG32 *)0xE0001000)
#define SYS_REG_DWT_CYCCNT                    (*(CPU_REG32 *)0xE0001004)

#define SYS_BIT_DEM_CR_TRCENA                 DEF_BIT_24
#define SYS_BIT_DWT_CR_CYCCNTENA              DEF_BIT_00

/* ��̫����ض��� */
/* LAN8720 PHY��ַ���� */
#define SYS_ETH_PHY_ADDRESS                   0

/* ��̫����ʼ����� */
#define SYS_ETH_INIT_FLAG                     0x01

/* ��̫����·״̬��� */
#define SYS_ETH_LINK_FLAG                     0x10 

/* ���������ַ���� */
#define MAC_ADDR0                             2
#define MAC_ADDR1                             0
#define MAC_ADDR2                             0
#define MAC_ADDR3                             0
#define MAC_ADDR4                             0
#define MAC_ADDR5                             0


/* ����IP��ַ���� */
#define SYS_ETH_IP_ADDR0                      192
#define SYS_ETH_IP_ADDR1                      168
#define SYS_ETH_IP_ADDR2                      1
#define SYS_ETH_IP_ADDR3                      112

/* ������������������ */
#define SYS_ETH_NET_MASK0                     255
#define SYS_ETH_NET_MASK1                     255
#define SYS_ETH_NET_MASK2                     255
#define SYS_ETH_NET_MASK3                     0

/* �������������� */
#define SYS_ETH_GATE_WAY0                     192
#define SYS_ETH_GATE_WAY1                     168
#define SYS_ETH_GATE_WAY2                     1
#define SYS_ETH_GATE_WAY3                     1



INT fputc(INT iChar, FILE *pstFile) ;
VOID Delay(UINT uiDelay) ;
VOID __SYS_SystemClockConfig(VOID) ;
VOID __SYS_SystemAllGPIOConfig(VOID) ;
VOID __SYS_SystemSerialPortConfig(VOID) ;
VOID __SYS_SystemEthPortTypeConfig(VOID) ;
VOID __SYS_SystemEthNVICConfig(VOID) ;
VOID __SYS_SystemEthMACDMAConfig(VOID) ;
VOID SYS_SystemConfig(VOID) ;
VOID SYS_LWIPInit(VOID) ;
VOID SYS_SystemTickInit(VOID) ;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SYS_CFG_H__ */

/******* End of file sys_cfg.h. *******/  
