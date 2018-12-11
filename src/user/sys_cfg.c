/*******************************************************************************
 Copyright SAKC Corporation. 2016. All rights reserved.
--------------------------------------------------------------------------------
    File name    : sys_cfg.c
    Project name : ��������ֲģ�顣
    Module name  : 
    Date created : 2017��8��17��   10ʱ19��43��
    Author       : Ning.JianLi
    Description  : STM32ϵͳ���ýӿڡ�
*******************************************************************************/

#include "sys_cfg.h"

#ifdef __cplusplus
extern "c" {
#endif /* __cplusplus */

ETH_InitTypeDef g_stEthInitInfo ;
UINT            g_uiEthStatus ;
struct netif    g_stNetIf ;



/* �ú����Ķ���ʹ��printf�������Խ���Ϣ�����USART�˿��� */
INT fputc(INT iChar, FILE *pstFile)
{
    USART_SendData(USART1, (UCHAR)iChar);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    return iChar;
}

VOID Delay(UINT uiDelay)
{
    OS_ERR enErr ;
    
    OSTimeDly(uiDelay, OS_OPT_TIME_DLY, &enErr) ;
    return ;
}


VOID __SYS_SystemClockConfig(VOID)
{
    /* ��ʼ��ϵͳʱ�� */
    RCC_DeInit() ;
    RCC_HSEConfig(RCC_HSE_ON) ;
    if(ERROR == RCC_WaitForHSEStartUp())
    {
        return ;
    }

    /* ���໷����ϵ������ */
    RCC_PLLConfig(RCC_PLLSource_HSE, 
                  SYS_PLL_CONFIG_INFO_PLLM ,
                  SYS_PLL_CONFIG_INFO_PLLN ,
                  SYS_PLL_CONFIG_INFO_PLLP ,
                  SYS_PLL_CONFIG_INFO_PLLQ) ;

    /* AHBʱ�Ӻ�SYSCLKһ�� */
    RCC_HCLKConfig(RCC_SYSCLK_Div1) ;

    /* APB1����ʱ�ӹ�����AHB����ʱ�ӵ�1/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4) ;

    /* APB1����ʱ�ӹ�����AHB����ʱ�ӵ�1/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2) ;

    /* �������໷ */
    RCC_PLLCmd(ENABLE) ;

    /* �ȴ����໷���� */
    while(RESET == RCC_GetFlagStatus(RCC_FLAG_PLLRDY)) ;

    /* ����������168MHz��STM32F4��FLASH�ȴ�����Ӧ����Ϊ5 */
    FLASH_SetLatency(FLASH_Latency_5) ;

    /* ����Ԥȡ */
    FLASH_PrefetchBufferCmd(ENABLE) ;

    /* ����ָ��Cache */
    FLASH_InstructionCacheCmd(ENABLE) ;

    /* ��������Cache */
    FLASH_DataCacheCmd(ENABLE) ;

    /* ѡ��PLL�����Ϊϵͳʱ�� */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK) ;

    /* �ȴ�ϵͳʱ���л���� */
    while(0x08 != RCC_GetSYSCLKSource()) ;

    return ;
}

VOID __SYS_SystemAllGPIOConfig(VOID)
{
    GPIO_InitTypeDef stGPIOInitInfo ;

    /* LED GPIO���� */
    /* LEDʹ��GPIOH���10��11��12���� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE) ;
    
    stGPIOInitInfo.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 ;
    stGPIOInitInfo.GPIO_Mode  = GPIO_Mode_OUT ;
    stGPIOInitInfo.GPIO_Speed = GPIO_Speed_50MHz ;
    stGPIOInitInfo.GPIO_OType = GPIO_OType_PP ;
    stGPIOInitInfo.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOH, &stGPIOInitInfo) ;

    /* Ϩ�����е�LED�� */
    GPIO_SetBits(GPIOH, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12) ;
    
    /* ϵͳ����CPIO���� */
    /* ϵͳ����ʹ��GPIOA���9��10���� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE) ;

    stGPIOInitInfo.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10 ;
    stGPIOInitInfo.GPIO_Mode  = GPIO_Mode_AF ;
    stGPIOInitInfo.GPIO_Speed = GPIO_Speed_50MHz ;
    stGPIOInitInfo.GPIO_OType = GPIO_OType_PP ;
    stGPIOInitInfo.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &stGPIOInitInfo) ;

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1) ;
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1) ;

    /* ��̫���˿�ʹ�õ�GPIO������ */
    /* -----------------------------------
     * | FUNC PIN NAME    | PIN LOCATION |
     * -----------------------------------
     * | ETH_MDIO         | PA2          |
     * | ETH_MDC          | PC1          |
     * | ETH_RMII_REF_CLK | PA1          |
     * | ETH_RMII_CRS_DV  | PA7          |
     * | ETH_RMII_RXD0    | PC4          |
     * | ETH_RMII_RXD1    | PC5          |
     * | ETH_RMII_TX_EN   | PB11         |
     * | ETH_RMII_TXD0    | PG13         |
     * | ETH_RMII_TXD1    | PG14         |
     * | ETH_NRST         | PI1          |
     * -----------------------------------
     */

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE) ;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE) ;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE) ;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE) ;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE) ;

    stGPIOInitInfo.GPIO_Mode  = GPIO_Mode_AF ;
    stGPIOInitInfo.GPIO_Speed = GPIO_Speed_100MHz ;
    stGPIOInitInfo.GPIO_OType = GPIO_OType_PP ;
    stGPIOInitInfo.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
    
    stGPIOInitInfo.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7 ;
    GPIO_Init(GPIOA, &stGPIOInitInfo) ;

    stGPIOInitInfo.GPIO_Pin   = GPIO_Pin_11 ;
    GPIO_Init(GPIOB, &stGPIOInitInfo) ;

    stGPIOInitInfo.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 ;
    GPIO_Init(GPIOC, &stGPIOInitInfo) ;

    stGPIOInitInfo.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 ;
    GPIO_Init(GPIOG, &stGPIOInitInfo) ;

    stGPIOInitInfo.GPIO_Pin   = GPIO_Pin_1 ;
    stGPIOInitInfo.GPIO_Mode  = GPIO_Mode_OUT ;
    GPIO_Init(GPIOI, &stGPIOInitInfo) ;

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH) ;
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH) ;
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH) ;

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_ETH) ;
    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH) ;
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH) ;
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH) ;

    GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH) ;
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH) ;

    //GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_ETH) ;

    return ;
}

VOID __SYS_SystemEthPortTypeConfig(VOID)
{
    /* ʹ��SYSCFG��Ԫ��ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE) ;

    /* ʹ��RMII�ӿ�ģʽ */
    SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII) ;

    return ;
}

VOID __SYS_SystemEthNVICConfig(VOID)
{
    NVIC_InitTypeDef   stNVICInitInfo ; 
    
    /* ʹ����̫���˿�ȫ���ж� */
    stNVICInitInfo.NVIC_IRQChannel                   = ETH_IRQn;
    stNVICInitInfo.NVIC_IRQChannelPreemptionPriority = 12 ;
    stNVICInitInfo.NVIC_IRQChannelSubPriority        = 0;
    stNVICInitInfo.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&stNVICInitInfo);
}

VOID __SYS_SystemEthMACDMAConfig(VOID)
{
    /* ʹ����̫��MACʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | 
                           RCC_AHB1Periph_ETH_MAC_Tx | 
                           RCC_AHB1Periph_ETH_MAC_Rx, 
                           ENABLE);

    /* ��λ��̫��MAC */
    ETH_DeInit() ;

    /* ��λ��̫��DMA���� */
    ETH_SoftwareReset() ;
    while (ETH_GetSoftwareResetStatus() == SET);

    /* MAC��ʼ�� */
    ETH_StructInit(&g_stEthInitInfo) ;
    
    g_stEthInitInfo.ETH_AutoNegotiation             = ETH_AutoNegotiation_Enable ; 
                                                    
    g_stEthInitInfo.ETH_LoopbackMode                = ETH_LoopbackMode_Disable ;
    g_stEthInitInfo.ETH_RetryTransmission           = ETH_RetryTransmission_Disable ;
    g_stEthInitInfo.ETH_AutomaticPadCRCStrip        = ETH_AutomaticPadCRCStrip_Disable ;
    g_stEthInitInfo.ETH_ReceiveAll                  = ETH_ReceiveAll_Disable ;
    g_stEthInitInfo.ETH_BroadcastFramesReception    = ETH_BroadcastFramesReception_Enable ;
    g_stEthInitInfo.ETH_PromiscuousMode             = ETH_PromiscuousMode_Disable ;
    g_stEthInitInfo.ETH_MulticastFramesFilter       = ETH_MulticastFramesFilter_Perfect ;
    g_stEthInitInfo.ETH_UnicastFramesFilter         = ETH_UnicastFramesFilter_Perfect ;
                                                    
    g_stEthInitInfo.ETH_ChecksumOffload             = ETH_ChecksumOffload_Enable ;
                                                    
    g_stEthInitInfo.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable ;
    g_stEthInitInfo.ETH_ReceiveStoreForward         = ETH_ReceiveStoreForward_Enable ;
    g_stEthInitInfo.ETH_TransmitStoreForward        = ETH_TransmitStoreForward_Enable ;
                                                    
    g_stEthInitInfo.ETH_ForwardErrorFrames          = ETH_ForwardErrorFrames_Disable ;
    g_stEthInitInfo.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable ;
    g_stEthInitInfo.ETH_SecondFrameOperate          = ETH_SecondFrameOperate_Enable ;
    g_stEthInitInfo.ETH_AddressAlignedBeats         = ETH_AddressAlignedBeats_Enable ;
    g_stEthInitInfo.ETH_FixedBurst                  = ETH_FixedBurst_Enable ;
    g_stEthInitInfo.ETH_RxDMABurstLength            = ETH_RxDMABurstLength_32Beat ;
    g_stEthInitInfo.ETH_TxDMABurstLength            = ETH_TxDMABurstLength_32Beat ;
    g_stEthInitInfo.ETH_DMAArbitration              = ETH_DMAArbitration_RoundRobin_RxTx_1_1 ;
    
    /* ������̫�� */
    g_uiEthStatus = ETH_Init(&g_stEthInitInfo, (USHORT)SYS_ETH_PHY_ADDRESS) ;
    
    /* ��������ж� */
    //ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);

    return ;
}

VOID __SYS_SystemEthPhyReset(VOID)
{
    unsigned int uiDelay ;
    /* �������Ž���PHY��λ */
    GPIO_ResetBits(GPIOI, GPIO_Pin_1) ;
    uiDelay = 0xfffff ;
    while(uiDelay--) ;

    /* �������ų���PHY��λ */
    GPIO_SetBits(GPIOI, GPIO_Pin_1) ;
    uiDelay = 0xfffff ;
    while(uiDelay--) ;

    return ;
}

VOID __SYS_SystemEthConfig(VOID)
{
    /* ����ϵͳ��̫��MACʹ��RMII�ӿ� */
    __SYS_SystemEthPortTypeConfig() ;

    /* ��λPHY */
    __SYS_SystemEthPhyReset() ;
    
    /* ʹ����̫���ж� */
    //__SYS_SystemEthNVICConfig() ;
    //SYS_SystemInterruptEnable(SYS_INT_ID_ETH) ;

    /* ��̫��MAC��DMA�������� */
    __SYS_SystemEthMACDMAConfig() ;

    /* ��ȡ��ǰPHY������״̬(UP/DOWN) */
    if(PHY_Linked_Status == ETH_ReadPHYRegister(SYS_ETH_PHY_ADDRESS, PHY_BSR) & PHY_Linked_Status)
    {
        /* ��·OK */
        g_uiEthStatus |= SYS_ETH_LINK_FLAG ;
    }
    /* TODO: �˴������Link UP/DOWN ʱ���жϴ�����룬����Ҫ�ⲿPHY֧���жϡ�
     *       LAN8720��DP83848��֧�ָù��ܣ�����������Ӳ������֧�֡�
     */

    return ;
}

VOID __SYS_SystemSerialPortConfig(VOID)
{
    USART_InitTypeDef stUSARTInitInfo ;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE) ;

    /* 115200-8-1-N-N */
    stUSARTInitInfo.USART_BaudRate            = 115200 ;
    stUSARTInitInfo.USART_WordLength          = USART_WordLength_8b ;
    stUSARTInitInfo.USART_StopBits            = USART_StopBits_1 ;
    stUSARTInitInfo.USART_Parity              = USART_Parity_No ;
    stUSARTInitInfo.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx ;
    stUSARTInitInfo.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;

    USART_Init(USART1, &stUSARTInitInfo) ;
    USART_Cmd(USART1, ENABLE) ;
}


VOID SYS_SystemConfig(VOID)
{
    /* ϵͳʱ������ */
    __SYS_SystemClockConfig() ;

    /* GPIO���� */
    __SYS_SystemAllGPIOConfig() ;

    /* ϵͳ�������� */ 
    __SYS_SystemSerialPortConfig() ;
    
    /* ��̫���ӿ����� */
    __SYS_SystemEthConfig() ;

    /* �жϴ��������ʼ�� */
    __SYS_SystemInterruptInit() ;

    return ;
}

VOID __SYS_SystemEthLinkCallback(struct netif *pstNetIF)
{
    UINT   uiTimeOut = 0 ;
    UINT   uiTmpReg ;
    UINT   uiRegVal ;
    struct ip_addr stIPAddr ;
    struct ip_addr stNetMask ;
    struct ip_addr stGateWay ;

    if(netif_is_link_up(pstNetIF))
    {
        /* ���½����Զ�Э�� */
        if(g_stEthInitInfo.ETH_AutoNegotiation != ETH_AutoNegotiation_Disable)
        {
            /* ���ó�ʱ������ */
            uiTimeOut = 0;

            /* �����Զ�Э�� */
            ETH_WritePHYRegister(0, PHY_BCR, PHY_AutoNegotiation);

            /* �ȴ���ֱ���Զ�Э����� */
            do
            {
                uiTimeOut++;
            } while (!(ETH_ReadPHYRegister(0, PHY_BSR) & PHY_AutoNego_Complete) && (uiTimeOut < (UINT)PHY_READ_TO));

            /* ���ó�ʱ��ʱ�� */
            uiTimeOut = 0;

            /* ��ȡ�Զ�Э�̽�� */
            uiRegVal = ETH_ReadPHYRegister(0, PHY_SR);
    
            /* ����Э�̽��������MAC��˫��ģʽ */
            if((uiRegVal & PHY_DUPLEX_STATUS) != (UINT)RESET)
            {
                /* ȫ˫��ģʽ */
                g_stEthInitInfo.ETH_Mode = ETH_Mode_FullDuplex;  
            }
            else
            {
                /* ��˫��ģʽ */
                g_stEthInitInfo.ETH_Mode = ETH_Mode_HalfDuplex;
            }
            
            /* ����Э�̽����������·�ٶ� */
            if(uiRegVal & PHY_SPEED_STATUS)
            {
                /* 10Mbps */
                g_stEthInitInfo.ETH_Speed = ETH_Speed_10M; 
            }
            else
            {
                /* 100Mbps */
                g_stEthInitInfo.ETH_Speed = ETH_Speed_100M;      
            }

            /* ��̫��MAC CR�Ĵ����������� */
            /* ��ȡ��ǰCRֵ */  
            uiTmpReg = ETH->MACCR;

            /* �����ٶ�����FESλ */ 
            /* ����ģʽ����DMλ */ 
            uiTmpReg |= (UINT)(g_stEthInitInfo.ETH_Speed | g_stEthInitInfo.ETH_Mode);

            /* д����MAC CR�Ĵ��� */
            ETH->MACCR = (UINT)uiTmpReg;

            _eth_delay_(ETH_REG_WRITE_DELAY);
            uiTmpReg = ETH->MACCR;
            ETH->MACCR = uiTmpReg;
        }

        /* ����MAC�ӿ� */
        ETH_Start();

        /* ������������ */
        IP4_ADDR(&stIPAddr, 
                  SYS_ETH_IP_ADDR0, 
                  SYS_ETH_IP_ADDR1, 
                  SYS_ETH_IP_ADDR2, 
                  SYS_ETH_IP_ADDR3) ;
    
        IP4_ADDR(&stNetMask, 
                  SYS_ETH_NET_MASK0, 
                  SYS_ETH_NET_MASK1, 
                  SYS_ETH_NET_MASK2, 
                  SYS_ETH_NET_MASK3) ;
    
        IP4_ADDR(&stGateWay, 
                  SYS_ETH_GATE_WAY0, 
                  SYS_ETH_GATE_WAY1, 
                  SYS_ETH_GATE_WAY2, 
                  SYS_ETH_GATE_WAY3) ;


        netif_set_addr(&g_stNetIf, 
                       &stIPAddr, 
                       &stNetMask, 
                       &stGateWay) ;
    
        /* ʹ�ýӿ�UP */
        netif_set_up(&g_stNetIf);    
    }
    else
    {
        ETH_Stop();

        /* �Ͽ���·�����ýӿ�DOWN */
        netif_set_down(&g_stNetIf);
    }
}

VOID SYS_LWIPInit(VOID)
{
    struct ip_addr stIPAddr ;
    struct ip_addr stNetMask ;
    struct ip_addr stGateWay ;

    /* ����TCP/IPЭ��ջ�߳� */
    tcpip_init( NULL, NULL );	

    /* ������������ */
    IP4_ADDR(&stIPAddr, 
              SYS_ETH_IP_ADDR0, 
              SYS_ETH_IP_ADDR1, 
              SYS_ETH_IP_ADDR2, 
              SYS_ETH_IP_ADDR3) ;
    
    IP4_ADDR(&stNetMask, 
              SYS_ETH_NET_MASK0, 
              SYS_ETH_NET_MASK1, 
              SYS_ETH_NET_MASK2, 
              SYS_ETH_NET_MASK3) ;
    
    IP4_ADDR(&stGateWay, 
              SYS_ETH_GATE_WAY0, 
              SYS_ETH_GATE_WAY1, 
              SYS_ETH_GATE_WAY2, 
              SYS_ETH_GATE_WAY3) ;

    /* ������� */
    netif_add(&g_stNetIf, 
              &stIPAddr, 
              &stNetMask, 
              &stGateWay, 
              NULL, 
              &ethernetif_init, 
              &tcpip_input);

    /* ע���ʼ���� */
    netif_set_default(&g_stNetIf);

    //if (g_uiEthStatus == (SYS_ETH_INIT_FLAG | SYS_ETH_LINK_FLAG))
    //{ 
        /* ������·UP��� */
        g_stNetIf.flags |= NETIF_FLAG_LINK_UP;

        /* ������ɺ������ñ����� */
        netif_set_up(&g_stNetIf);
    //}
    //else
    //{
        /* �����·DOWN�����ýӿ�DOWN */
        //netif_set_down(&g_stNetIf);
    //}

    /* ��·״̬�����仯ʱ�Ļص����� */
    //netif_set_link_callback(&g_stNetIf, __SYS_SystemEthLinkCallback);
}

VOID SYS_SystemTickInit(VOID)
{
    RCC_ClocksTypeDef stRCCClockFreqInfo ;
    UINT              uiCnts ;

    /* ��ȡϵͳʱ��Ƶ�� */
    RCC_GetClocksFreq(&stRCCClockFreqInfo) ;

    uiCnts = stRCCClockFreqInfo.HCLK_Frequency / OSCfg_TickRate_Hz ;

    /* �Լ������systick������װ��ֵ����ʼ��systick */
    OS_CPU_SysTickInit(uiCnts) ;

    return ;
}

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
VOID  CPU_TS_TmrInit(VOID)
{
    RCC_ClocksTypeDef stRCCClockFreqInfo ;
    
    /* ��ȡϵͳʱ��Ƶ�� */
    RCC_GetClocksFreq(&stRCCClockFreqInfo) ;

    SYS_REG_DEM_CR     |= (CPU_INT32U)SYS_BIT_DEM_CR_TRCENA;    /* Enable Cortex-M4's DWT CYCCNT reg.                   */
    SYS_REG_DWT_CYCCNT  = (CPU_INT32U)0u;
    SYS_REG_DWT_CR     |= (CPU_INT32U)SYS_BIT_DWT_CR_CYCCNTENA;
    
    CPU_TS_TmrFreqSet(stRCCClockFreqInfo.HCLK_Frequency) ;

    return ;
}

CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;

    ts_tmr_cnts = (CPU_TS_TMR)SYS_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}

CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
    RCC_ClocksTypeDef stRCCClockFreqInfo ;

    RCC_GetClocksFreq(&stRCCClockFreqInfo) ;

    return(ts_cnts / (stRCCClockFreqInfo.HCLK_Frequency / DEF_TIME_NBR_uS_PER_SEC)) ;
}

CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
    RCC_ClocksTypeDef stRCCClockFreqInfo ;

    RCC_GetClocksFreq(&stRCCClockFreqInfo) ;

    return(ts_cnts / (stRCCClockFreqInfo.HCLK_Frequency / DEF_TIME_NBR_uS_PER_SEC)) ;
}

#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */
/******* End of file sys_cfg.c. *******/  
