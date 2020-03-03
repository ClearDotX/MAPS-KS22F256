/*
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "fsl_enet.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the offset for IPv4 PTP 1588 UDP/IP multicast message. */
#define ENET_PTP1588_IPVERSION_OFFSET 0x0EU           /*!< IPv4 PTP message IP version offset. */
#define ENET_PTP1588_IPV4_UDP_PROTOCOL_OFFSET 0x17U   /*!< IPv4 PTP message UDP protocol offset. */
#define ENET_PTP1588_IPV4_UDP_PORT_OFFSET 0x24U       /*!< IPv4 PTP message UDP port offset. */
#define ENET_PTP1588_IPV4_UDP_MSGTYPE_OFFSET 0x2AU    /*!< IPv4 PTP message UDP message type offset. */
#define ENET_PTP1588_IPV4_UDP_VERSION_OFFSET 0x2BU    /*!< IPv4 PTP message UDP version offset. */
#define ENET_PTP1588_IPV4_UDP_CLKID_OFFSET 0x3EU      /*!< IPv4 PTP message UDP clock id offset. */
#define ENET_PTP1588_IPV4_UDP_SEQUENCEID_OFFSET 0x48U /*!< IPv4 PTP message UDP sequence id offset. */
#define ENET_PTP1588_IPV4_UDP_CTL_OFFSET 0x4AU        /*!< IPv4 PTP message UDP control offset. */

/*! @brief Defines the offset for IPv6 1588 PTP UDP/IP multicast message. */
#define ENET_PTP1588_IPV6_UDP_PROTOCOL_OFFSET 0x14U   /*!< IPv6 PTP message UDP protocol offset. */
#define ENET_PTP1588_IPV6_UDP_PORT_OFFSET 0x38U       /*!< IPv6 PTP message UDP port offset. */
#define ENET_PTP1588_IPV6_UDP_MSGTYPE_OFFSET 0x3EU    /*!< IPv6 PTP message UDP message type offset. */
#define ENET_PTP1588_IPV6_UDP_VERSION_OFFSET 0x3FU    /*!< IPv6 PTP message UDP version offset. */
#define ENET_PTP1588_IPV6_UDP_CLKID_OFFSET 0x52U      /*!< IPv6 PTP message UDP clock id offset. */
#define ENET_PTP1588_IPV6_UDP_SEQUENCEID_OFFSET 0x5CU /*!< IPv6 PTP message UDP sequence id offset. */
#define ENET_PTP1588_IPV6_UDP_CTL_OFFSET 0x5EU        /*!< IPv6 PTP message UDP control offset. */

/*! @brief Defines the offset for PTP 1588 Layer 2 Ethernet message. */
#define ENET_PTP1588_ETHL2_PACKETTYPE_OFFSET 0x0CU /*!< PTPv2 message Ethernet packet type offset. */
#define ENET_PTP1588_ETHL2_MSGTYPE_OFFSET 0x0EU    /*!< PTPv2 message Ethernet message type offset. */
#define ENET_PTP1588_ETHL2_VERSION_OFFSET 0X0FU    /*!< PTPv2 message Ethernet version type offset*/
#define ENET_PTP1588_ETHL2_CLOCKID_OFFSET 0x22     /*!< PTPv2 message Ethernet clock id offset*/
#define ENET_PTP1588_ETHL2_SEQUENCEID_OFFSET 0x2c  /*!< PTPv2 message Ethernet sequence id offset*/

/*! @brief Defines the macros for etherent driver use. */
#define ENET_ETHERNETL2 0x88F7U                /*!< Packet type Ethernet IEEE802.3 for PTPv2. */
#define ENET_IPV4 0x0800U                      /*!< Packet type IPv4. */
#define ENET_IPV6 0x86ddU                      /*!< Packet type IPv6. */
#define ENET_8021QVLAN 0x8100U                 /*!< Packet type VLAN. */
#define ENET_UDPVERSION 0x0011U                /*!< UDP protocol type. */
#define ENET_IPV4VERSION 0x0004U               /*!< Packet IP version IPv4. */
#define ENET_IPV6VERSION 0x0006U               /*!< Packet IP version IPv6. */
#define ENET_FRAME_MACLEN 6U                   /*!< Ethernet mac address length. */
#define ENET_FRAME_HEADERLEN 14U               /*!< Ethernet Frame header length. */
#define ENET_FRAME_VLAN_HEADERLEN 18U          /*!< Ethernet VLAN header length. */
#define ENET_MDC_FREQUENCY 2500000U            /*!< MDC frequency. */
#define ENET_NANOSECOND_ONE_SECOND 1000000000U /*!< NanoSecond in one second. */

/*! @brief Defines the macro for converting constants from host byte order to network byte order. */
#define ENET_HTONS(n) __REV16(n)
#define ENET_HTONL(n) __REV(n)
#define ENET_NTOHS(n) __REV16(n)
#define ENET_NTOHL(n) __REV(n)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static uint32_t getInstance(ENET_Type *base);
static void ENET_SetMacController(ENET_Type *base,
                                  const enet_config_t *eMacConfig,
                                  const enet_buffer_config_t *eBufferConfig,
                                  uint8_t *macAddr,
                                  uint32_t clockSrc);
static void ENET_SetTxBufferDescriptors(volatile enet_tx_bd_struct_t *txBdStartAlign,
                                        uint8_t *txBufferAlign,
                                        uint32_t txBuffSizeAlign,
                                        uint32_t txBdNumber);
static void ENET_SetRxBufferDescriptors(volatile enet_rx_bd_struct_t *rxBdStartAlign,
                                        uint8_t *rxBufferAlign,
                                        uint32_t rxBuffSizeAlign,
                                        uint32_t rxBdNumber,
                                        bool enableInterrupt);
static volatile enet_rx_bd_struct_t *ENET_IncrRxBuffDescripIndex(enet_handle_t *handle,
                                                                 volatile enet_rx_bd_struct_t *curBuffDescrip);
static volatile enet_tx_bd_struct_t *ENET_IncrTxBuffDescripIndex(enet_handle_t *handle,
                                                                 volatile enet_tx_bd_struct_t *curBuffDescrip);
static void ENET_UpdateReadBuffers(enet_handle_t *handle);
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
static bool ENET_Ptp1588ParseFrame(uint8_t *packet, enet_ptp_time_data_t *ptpTsData, bool isFastEnabled);
static status_t ENET_Ptp1588UpdateTimeRing(enet_ptp_time_data_ring_t *ptpTsDataRing,
                                                     enet_ptp_time_data_t *ptpTimeData);
static status_t ENET_Ptp1588SearchTimeRing(enet_ptp_time_data_ring_t *ptpTsDataRing,
                                                     enet_ptp_time_data_t *ptpTimedata);
static status_t ENET_StoreTxFrameTime(enet_handle_t *handle);
static status_t ENET_StoreRxFrameTime(enet_handle_t *handle, enet_ptp_time_data_t *ptpTimeData);
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */

/*******************************************************************************
 * Variables
 ******************************************************************************/

static enet_handle_t *g_ENETHandle[FSL_FEATURE_SOC_ENET_COUNT] = {NULL};
const clock_ip_name_t g_enetClock[FSL_FEATURE_SOC_ENET_COUNT] = ENET_CLOCKS;

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t getInstance(ENET_Type *base)
{
    uint8_t instance;

    switch ((uint32_t)base)
    {
        case (uint32_t)ENET:
            instance = 0;
            break;
        default:
            break;
    }

    return instance;
}

void ENET_GetDefaultConfig(enet_config_t *eMacConfig)
{
    /* Checks input parameter. */
    assert(eMacConfig);

    /* Initializes the MAC configure structure to zero. */
    memset(eMacConfig, 0, sizeof(enet_config_t));

    /* Sets MII mode, full duplex, 100Mbps for MAC and PHY data interface. */
    eMacConfig->miiMode = kENET_RmiiMode;
    eMacConfig->miiSpeed = kENET_MiiSpeed100M;
    eMacConfig->miiDuplex = kENET_MiiFullDuplex;

    /* Sets the maximum receive frame length. */
    eMacConfig->rxMaxFrameLen = ENET_FRAME_MAX_FRAMELEN;
}

void ENET_Init(enet_handle_t *handle,
               const enet_config_t *eMacConfig,
               const enet_buffer_config_t *eBufferConfig,
               uint8_t *macAddr,
               uint32_t clockSrc)
{
    /* Checks input parameters. */
    assert(handle);
    assert(eMacConfig);
    assert(eBufferConfig);
    assert(eBufferConfig->rxBdStartAddrAlign);
    assert(eBufferConfig->txBdStartAddrAlign);
    assert(eBufferConfig->rxBufferAlign);
    assert(eBufferConfig->txBufferAlign);
    assert(macAddr);
    assert(eBufferConfig->rxBuffSizeAlign >= ENET_RX_MIN_BUFFERSIZE);

    ENET_Type *base = handle->base;

    /* Make sure the buffers should be have the capability of process at least one maximum frame. */
    if (eMacConfig->macSpecialConfig & kENET_ControlVLANTagEnable)
    {
        assert(eBufferConfig->txBuffSizeAlign * eBufferConfig->txBdNumber > ENET_FRAME_MAX_VALNFRAMELEN);
    }
    else
    {
        assert(eBufferConfig->txBuffSizeAlign * eBufferConfig->txBdNumber > ENET_FRAME_MAX_FRAMELEN);
        assert(eBufferConfig->rxBuffSizeAlign * eBufferConfig->rxBdNumber > eMacConfig->rxMaxFrameLen);
    }

    /* Ungate ENET clock. */
    CLOCK_EnableClock(g_enetClock[getInstance(base)]);

    /* Reset ENET module. */
    ENET_Reset(base);

    /* Initializes the ENET transmit buffer descriptors. */
    ENET_SetTxBufferDescriptors(eBufferConfig->txBdStartAddrAlign, eBufferConfig->txBufferAlign,
                                eBufferConfig->txBuffSizeAlign, eBufferConfig->txBdNumber);

    /* Initializes the ENET receive buffer descriptors. */
    ENET_SetRxBufferDescriptors(eBufferConfig->rxBdStartAddrAlign, eBufferConfig->rxBufferAlign,
                                eBufferConfig->rxBuffSizeAlign, eBufferConfig->rxBdNumber,
                                !!(eMacConfig->interrupt & (kENET_RxFrameInterrupt | kENET_RxByteInterrupt)));

    /* Initializes the ENET MAC controller. */
    ENET_SetMacController(base, eMacConfig, eBufferConfig, macAddr, clockSrc);

    /* Store transfer parameters in state pointer. */
    handle->rxBdBase = eBufferConfig->rxBdStartAddrAlign;
    handle->rxBdCurrent = eBufferConfig->rxBdStartAddrAlign;
    handle->rxBdDirty = eBufferConfig->rxBdStartAddrAlign;
    handle->txBdBase = eBufferConfig->txBdStartAddrAlign;
    handle->txBdCurrent = eBufferConfig->txBdStartAddrAlign;
    handle->rxBuffSizeAlign = eBufferConfig->rxBuffSizeAlign;
    handle->txBuffSizeAlign = eBufferConfig->txBuffSizeAlign;
}

void ENET_DeInit(ENET_Type *base)
{
    /* Disable interrupt. */
    base->EIMR = 0;

    /* Disable ENET. */
    base->ECR &= ~ENET_ECR_ETHEREN_MASK;

    /* Disables the clock source. */
    CLOCK_DisableClock(g_enetClock[getInstance(base)]);    
}

void ENET_CreateHandle(enet_handle_t *handle, ENET_Type *base)
{
    assert(handle);

    /* Initializes the handle. */
    memset((void *) handle, 0, sizeof(enet_handle_t));
    handle->base = base;
    g_ENETHandle[getInstance(base)] = handle;
}

/*!
 * @brief Configure MAC controller and the module will be enabled with the configuration.
 *
 * This API configure MAC controller with basic function. the txBdStartAddrAlign,
 * rxBdStartAddrAlign is recommended to evenly divisible by 16, the rxBuffSizeAlign
 * must be evenly divisible by 16.
 */
static void ENET_SetMacController(ENET_Type *base,
                                  const enet_config_t *eMacConfig,
                                  const enet_buffer_config_t *eBufferConfig,
                                  uint8_t *macAddr,
                                  uint32_t clockSrc)
{
    uint32_t rcr = 0;
    uint32_t tcr = 0;
    uint32_t ecr = 0;
    uint32_t macSpecialConfig = eMacConfig->macSpecialConfig;

    /* Configures MAC receive controller with user configure structure. */
    rcr = ENET_RCR_NLC(!!(macSpecialConfig & kENET_ControlRxPayloadCheckEnable)) |
           ENET_RCR_CFEN(!!(macSpecialConfig & kENET_ControlFlowControlEnable)) |
           ENET_RCR_FCE(!!(macSpecialConfig & kENET_ControlFlowControlEnable)) |
           ENET_RCR_PADEN(!!(macSpecialConfig & kENET_ControlRxPadRemoveEnable)) |
           ENET_RCR_BC_REJ(!!(macSpecialConfig & kENET_ControlRxBroadCastRejectEnable)) |
           ENET_RCR_PROM(!!(macSpecialConfig & kENET_ControlPromiscuousEnable)) | ENET_RCR_MII_MODE(1) |
           ENET_RCR_RMII_MODE(eMacConfig->miiMode) | ENET_RCR_RMII_10T(!eMacConfig->miiSpeed) |
           ENET_RCR_MAX_FL(eMacConfig->rxMaxFrameLen) |
           ENET_RCR_CRCFWD(1); 
    /* Receive setting for half duplex. */
    if (eMacConfig->miiDuplex == kENET_MiiHalfDuplex)
    {
        rcr |= ENET_RCR_DRT(1);
    }
    /* Sets internal loop only for MII mode. */
    if ((eMacConfig->macSpecialConfig & kENET_ControlMIILoopEnable) && (eMacConfig->miiMode == kENET_MiiMode))
    {
        rcr |= ENET_RCR_LOOP(1);
        rcr &= ~ENET_RCR_DRT_MASK;
    }
    base->RCR = rcr;

    /* Configures MAC transmit controller: duplex mode, mac address insertion. */
    tcr = base->TCR & ~(ENET_TCR_FDEN_MASK | ENET_TCR_ADDINS_MASK);
    tcr |= ENET_TCR_FDEN(eMacConfig->miiDuplex) | 
           ENET_TCR_ADDINS(!!(macSpecialConfig & kENET_ControlMacAddrInsert));
    base->TCR = tcr;

    /* Configures receive and transmit accelerator. */
    base->TACC = eMacConfig->txAccelerConfig;
    base->RACC = eMacConfig->rxAccelerConfig;

    /* Sets the pause duration and FIFO threshold for the flow control enabled case. */
    if (macSpecialConfig & kENET_ControlFlowControlEnable)
    {
        uint32_t reemReg;
        base->OPD = eMacConfig->pauseDuration;
        reemReg = ENET_RSEM_RX_SECTION_EMPTY(eMacConfig->rxFifoEmptyThreshold);
#if FSL_FEATURE_ENET_HAS_RECEIVE_STATUS_THRESHOLD
        reemReg |= ENET_RSEM_STAT_SECTION_EMPTY(eMacConfig->rxFifoStatEmptyThreshold);
#endif /* FSL_FEATURE_ENET_HAS_RECEIVE_STATUS_THRESHOLD */
        base->RSEM = reemReg;
    }

    /* FIFO threshold setting for store and forward enable/disable case. */
    if ( macSpecialConfig & kENET_ControlStoreAndFwdDisable)
    {
        /* Transmit fifo watermark settings. */
        base->TFWR = eMacConfig->txFifoWatermark & ENET_TFWR_TFWR_MASK;
        /* Receive fifo full threshold settings. */
        base->RSFL = eMacConfig->rxFifoFullThreshold & ENET_RSFL_RX_SECTION_FULL_MASK;
    }
    else
    {
        /* Transmit fifo watermark settings. */
        base->TFWR = ENET_TFWR_STRFWD_MASK;
        base->RSFL = 0;
    }

    /* Enable store and forward when accelerator is enabled */
    if (eMacConfig->txAccelerConfig & (kENET_TxAccelIpCheckEnabled | kENET_TxAccelProtoCheckEnabled))
    {   
        base->TFWR = ENET_TFWR_STRFWD_MASK;  
    }
    if (eMacConfig->rxAccelerConfig & (kENET_RxAccelIpCheckEnabled | kENET_RxAccelProtoCheckEnabled))
    {
        base->RSFL = 0;
    }

    /* Initializes transmit buffer descriptor rings start address, two start address should be aligned. */
    base->TDSR = (uint32_t)eBufferConfig->txBdStartAddrAlign;
    base->RDSR = (uint32_t)eBufferConfig->rxBdStartAddrAlign;
    /* Initializes the maximum buffer size, the buffer size should be aligned. */
    base->MRBR = eBufferConfig->rxBuffSizeAlign;

    /* Configures the Mac address. */
    ENET_SetMacAddr(base, macAddr);

    /* Initialize the SMI if uninitialized. */
    if (!ENET_GetSMI(base))
    {
        ENET_SetSMI(base, clockSrc, !!(eMacConfig->macSpecialConfig & kENET_ControlSMIPreambleDisable));
    }

    /* Enables Ethernet interrupt and NVIC. */
    ENET_EnableInterrupts(base, eMacConfig->interrupt);
    if (eMacConfig->interrupt & (kENET_RxByteInterrupt | kENET_RxFrameInterrupt))
    {
        EnableIRQ(ENET_Receive_IRQn);
    }
    else if (eMacConfig->interrupt & (kENET_TxByteInterrupt | kENET_TxFrameInterrupt))
    {
        EnableIRQ(ENET_Transmit_IRQn);
    }
    else if (eMacConfig->interrupt &
             (kENET_BabrInterrupt | kENET_BabtInterrupt | kENET_GraceStopInterrupt | kENET_MiiInterrupt |
              kENET_EBusERInterrupt | kENET_LateCollisionInterrupt | kENET_RetryLimitInterrupt |
              kENET_UnderrunInterrupt | kENET_PayloadRxInterrupt | kENET_WakeupInterrupt))
    {
        EnableIRQ(ENET_Error_IRQn);
    }

    /* ENET control register setting. */
    ecr = base->ECR;
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    /* Sets the 1588 enhanced feature. */
    ecr |= ENET_ECR_EN1588_MASK;
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
    /* Enables Ethernet module after all configuration except the buffer descriptor active. */
    ecr |= ENET_ECR_ETHEREN_MASK | ENET_ECR_DBSWP_MASK;
    base->ECR = ecr;
}

/*!
 * @brief Configure all transmit buffer descriptors of the ENET device.
 * the txBdStartAlign alignment is recommended to evenly divisible by 16,
 * txBufferAlign, txBuffSizeAlign must be evenly divisible by 16.
 */
static void ENET_SetTxBufferDescriptors(volatile enet_tx_bd_struct_t *txBdStartAlign,
                                        uint8_t *txBufferAlign,
                                        uint32_t txBuffSizeAlign,
                                        uint32_t txBdNumber)
{
    assert(txBdStartAlign);
    assert(txBufferAlign);

    uint32_t count;
    volatile enet_tx_bd_struct_t *curBuffDescrip = txBdStartAlign;

    for (count = 0; count < txBdNumber; count++)
    {
        /* Set data buffer address. */
        curBuffDescrip->buffer = (uint8_t *)((uint32_t)&txBufferAlign[count * txBuffSizeAlign]);
        /* Initializes data length. */
        curBuffDescrip->length = 0;
        /* Sets the crc. */
        curBuffDescrip->control = ENET_BUFFDESCRIPTOR_TX_TRANMITCRC_MASK;
        /* Sets the last buffer descriptor with the wrap flag. */
        if (count == txBdNumber - 1)
        {
            curBuffDescrip->control |= ENET_BUFFDESCRIPTOR_TX_WRAP_MASK;
        }

#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
        /* Enable transmit interrupt for store the transmit timestamp. */
        curBuffDescrip->controlExtend1 |= ENET_BUFFDESCRIPTOR_TX_INTERRUPT_MASK;
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
        /* Increase the index. */
        curBuffDescrip++;
    }
}

/*!
 * @brief Configure all receive buffer descriptors of the ENET device.
 * the interrupt is used in enhanced buffer descriptor. you can choose
 * if the receive interrupt is used for your use case.
 * the rxBdStartAlign alignment is recommended to evenly divisible by 16,
 * rxBufferAlign, rxBuffSizeAlign must be evenly divisible by 16.
 */
static void ENET_SetRxBufferDescriptors(volatile enet_rx_bd_struct_t *rxBdStartAlign,
                                        uint8_t *rxBufferAlign,
                                        uint32_t rxBuffSizeAlign,
                                        uint32_t rxBdNumber,
                                        bool enableInterrupt)
{
    assert(rxBdStartAlign);
    assert(rxBufferAlign);

    volatile enet_rx_bd_struct_t *curBuffDescrip = rxBdStartAlign;
    uint32_t count = 0;

    /* Initializes receive buffer descriptors. */
    for (count = 0; count < rxBdNumber; count++)
    {
        /* Set data buffer and the length. */
        curBuffDescrip->buffer = (uint8_t *)((uint32_t)&rxBufferAlign[count * rxBuffSizeAlign]);
        curBuffDescrip->length = 0;

        /* Initializes the buffer descriptors with empty bit. */
        curBuffDescrip->control = ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK;
        /* Sets the last buffer descriptor with the wrap flag. */
        if (count == rxBdNumber - 1)
        {
            curBuffDescrip->control |= ENET_BUFFDESCRIPTOR_RX_WRAP_MASK;
        }

#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
        if (enableInterrupt)
        {
            /* Enable receive interrupt. */
            curBuffDescrip->controlExtend1 |= ENET_BUFFDESCRIPTOR_RX_INTERRUPT_MASK;
        }
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
        /* Increase the index. */
        curBuffDescrip++;
    }
}

void ENET_SetMII(ENET_Type *base, enet_mii_speed_t speed, enet_mii_duplex_t duplex)
{
    uint32_t rcr;
    uint32_t tcr;

    rcr = base->RCR;
    tcr = base->TCR;

    /* Sets speed mode. */
    if (kENET_MiiSpeed10M == speed)
    {
        rcr |= ENET_RCR_RMII_10T_MASK;
    }
    else
    {
        rcr &= ~ENET_RCR_RMII_10T_MASK;
    }
    /* Set duplex mode. */
    if (duplex == kENET_MiiHalfDuplex)
    {
        rcr |= ENET_RCR_DRT_MASK;
        tcr &= ~ENET_TCR_FDEN_MASK;
    }
    else
    {
        rcr &= ~ENET_RCR_DRT_MASK;
        tcr |= ENET_TCR_FDEN_MASK;
    }

    base->RCR = rcr;
    base->TCR = tcr;
}

void ENET_SetMacAddr(ENET_Type *base, uint8_t *macAddr)
{
    uint32_t address;

    /* Set physical address lower register. */
    address = (uint32_t)(((uint32_t)macAddr[0] << 24U) | ((uint32_t)macAddr[1] << 16U) | ((uint32_t)macAddr[2] << 8U) |
                         (uint32_t)macAddr[3]);
    base->PALR = address;
    /* Set physical address high register. */
    address = (uint32_t)(((uint32_t)macAddr[4] << 8U) | ((uint32_t)macAddr[5]));
    base->PAUR = address << ENET_PAUR_PADDR2_SHIFT;
}

void ENET_GetMacAddr(ENET_Type *base, uint8_t *macAddr)
{
    assert(macAddr);

    uint32_t address;

    /* Get from physical address lower register. */
    address = base->PALR;
    macAddr[0] = 0xFFU & (address >> 24U);
    macAddr[1] = 0xFFU & (address >> 16U);
    macAddr[2] = 0xFFU & (address >> 8U);
    macAddr[3] = 0xFFU & address;

    /* Get from physical address high register. */
    address = (base->PAUR & ENET_PAUR_PADDR2_MASK) >> ENET_PAUR_PADDR2_SHIFT;
    macAddr[4] = 0xFFU & (address >> 8U);
    macAddr[5] = 0xFFU & address;
}

void ENET_SetSMI(ENET_Type *base, uint32_t sysClk, bool isPreambleDisabled)
{
    assert(sysClk);

    uint32_t clkCycle = 0;
    uint32_t speed = 0;
    uint32_t mscr = 0;

    /* Calculate the MII speed which controls the frequency of the MDC. */
    speed = sysClk / (2 * ENET_MDC_FREQUENCY);
    /* Calculate the hold time on the MDIO output. */
    clkCycle = (10 + ENET_NANOSECOND_ONE_SECOND / sysClk - 1) / (ENET_NANOSECOND_ONE_SECOND / sysClk) - 1;
    /* Build the configuration for MDC/MDIO control. */
    mscr = ENET_MSCR_MII_SPEED(speed) | ENET_MSCR_DIS_PRE(isPreambleDisabled) | ENET_MSCR_HOLDTIME(clkCycle);
    base->MSCR = mscr;
}

void ENET_StartSMIWrite(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, enet_mii_write_t operation, uint32_t data)
{
    uint32_t mmfr = 0;

    /* Build MII write command. */
    mmfr = ENET_MMFR_ST(1) | ENET_MMFR_OP(operation) | ENET_MMFR_PA(phyAddr) | ENET_MMFR_RA(phyReg) | ENET_MMFR_TA(2) |
           (data & 0xFFFF);
    base->MMFR = mmfr;
}

void ENET_StartSMIRead(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, enet_mii_read_t operation)
{
    uint32_t mmfr = 0;

    /* Build MII read command. */
    mmfr = ENET_MMFR_ST(1) | ENET_MMFR_OP(operation) | ENET_MMFR_PA(phyAddr) | ENET_MMFR_RA(phyReg) | ENET_MMFR_TA(2);
    base->MMFR = mmfr;
}

void ENET_GetRxErrBeforeReadFrame(enet_handle_t *handle, enet_data_error_stats_t *eErrorStatic)
{
    assert(handle);
    assert(eErrorStatic);

    uint16_t control = 0;
    volatile enet_rx_bd_struct_t *curBuffDescrip = handle->rxBdCurrent;

    do
    {
        /* The last buffer descriptor of a frame. */
        if (curBuffDescrip->control & ENET_BUFFDESCRIPTOR_RX_LAST_MASK)
        {
            control = curBuffDescrip->control;
            if (control & ENET_BUFFDESCRIPTOR_RX_TRUNC_MASK)
            {
                /* The receive truncate error. */
                eErrorStatic->statsRxTruncateErr++;
            }
            if (control & ENET_BUFFDESCRIPTOR_RX_OVERRUN_MASK)
            {
                /* The receive over run error. */
                eErrorStatic->statsRxOverRunErr++;
            }
            if (control & ENET_BUFFDESCRIPTOR_RX_LENVLIOLATE_MASK)
            {
                /* The receive length violation error. */
                eErrorStatic->statsRxLenGreaterErr++;
            }
            if (control & ENET_BUFFDESCRIPTOR_RX_NOOCTET_MASK)
            {
                /* The receive alignment error. */
                eErrorStatic->statsRxAlignErr++;
            }
            if (control & ENET_BUFFDESCRIPTOR_RX_CRC_MASK)
            {
                /* The receive CRC error. */
                eErrorStatic->statsRxFcsErr++;
            }
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
            uint16_t controlExt = curBuffDescrip->controlExtend1;
            if (controlExt & ENET_BUFFDESCRIPTOR_RX_MACERR_MASK)
            {
                /* The MAC error. */
                eErrorStatic->statsRxMacErr++;
            }
            if (controlExt & ENET_BUFFDESCRIPTOR_RX_PHYERR_MASK)
            {
                /* The PHY error. */
                eErrorStatic->statsRxPhyErr++;
            }
            if (controlExt & ENET_BUFFDESCRIPTOR_RX_COLLISION_MASK)
            {
                /* The receive collision error. */
                eErrorStatic->statsRxCollisionErr++;
            }
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
        }
        /* Increase current buffer descriptor to the next one. */
        curBuffDescrip = ENET_IncrRxBuffDescripIndex(handle, curBuffDescrip);
    } while ((curBuffDescrip != NULL) && (curBuffDescrip != handle->rxBdCurrent));
}

status_t ENET_GetRxFrameSize(enet_handle_t *handle, uint32_t *length)
{
    assert(handle);
    assert(length);

    uint16_t validLastMask = ENET_BUFFDESCRIPTOR_RX_LAST_MASK | ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK;
    volatile enet_rx_bd_struct_t *curBuffDescrip = handle->rxBdCurrent;

    /* Check the current buffer descriptor's empty flag.  if empty means there is no frame received. */
    if (curBuffDescrip->control & ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK)
    {
        *length = 0;
        return kStatus_ENET_RxFrameEmpty;
    }

    do
    {
        /* Find the last buffer descriptor. */
        if ((curBuffDescrip->control & validLastMask) == ENET_BUFFDESCRIPTOR_RX_LAST_MASK)
        {
            /* The last buffer descriptor in the frame check the status of the received frame. */
            if ((curBuffDescrip->control & ENET_BUFFDESCRIPTOR_RX_ERR_MASK)
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
                || (curBuffDescrip->controlExtend1 & ENET_BUFFDESCRIPTOR_RX_EXT_ERR_MASK)
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
                )
            {
                *length = 0;
                return kStatus_ENET_RxFrameError;
            }
            /* FCS is removed by MAC. */
            *length = curBuffDescrip->length; 
            return kStatus_ENET_Success;
        }
        /* Increase the buffer descriptor to the next one. */
        curBuffDescrip = ENET_IncrRxBuffDescripIndex(handle, curBuffDescrip);
    } while ((curBuffDescrip != NULL) && (curBuffDescrip != handle->rxBdCurrent));

    /* The frame is on processing - set to empty status to make application to receive it next time. */
    return kStatus_ENET_RxFrameEmpty;
}

status_t ENET_ReadFrame(enet_handle_t *handle, uint8_t *data, uint32_t length)
{
    assert(handle);

    uint32_t len = 0;
    uint32_t offset = 0;
    bool isLastBuff = false;
    volatile enet_rx_bd_struct_t *curBuffDescrip;
    status_t result = kStatus_ENET_Success;
    ENET_Type *base = handle->base;

    /* For data-NULL input, only update the buffer descriptor. */
    if (!data)
    {
        do
        {
            /* Get the current buffer descriptor. */
            curBuffDescrip = handle->rxBdCurrent;
            /* Increase current buffer descriptor to the next one. */
            handle->rxBdCurrent = ENET_IncrRxBuffDescripIndex(handle, handle->rxBdCurrent);
            /* The last buffer descriptor of a frame. */
            if (curBuffDescrip->control & ENET_BUFFDESCRIPTOR_RX_LAST_MASK)
            {
                /* Find the last buffer descriptor for the frame*/
                break;
            }
        } while (handle->rxBdCurrent != handle->rxBdDirty);
        /* Update all receive buffer descriptors for the whole frame. */
        ENET_UpdateReadBuffers(handle);
        return result;
    }
    else
    {
        /* Frame read from the MAC to user buffer and update the buffer descriptors.
        Process the frame, a frame on several receive buffers are considered . */
        /* Get the current buffer descriptor. */
        curBuffDescrip = handle->rxBdCurrent;
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
        /* Parse the PTP message according to the header message. */
        enet_ptp_time_data_t ptpTimestamp;
        bool isPtpEventMessage = false;
        isPtpEventMessage = ENET_Ptp1588ParseFrame(curBuffDescrip->buffer, &ptpTimestamp, false);
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */

        while (!isLastBuff)
        {
            /* Increase current buffer descriptor to the next one. */
            handle->rxBdCurrent = ENET_IncrRxBuffDescripIndex(handle, handle->rxBdCurrent);
            /* The last buffer descriptor of a frame. */
            if (curBuffDescrip->control & ENET_BUFFDESCRIPTOR_RX_LAST_MASK)
            {
                /* This is a valid frame. */
                isLastBuff = true;
                if (length == curBuffDescrip->length)
                {
                    /* Copy the frame to user's buffer without FCS. */
                    len = curBuffDescrip->length - offset;
                    memcpy(data + offset, curBuffDescrip->buffer, len);
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
                    /* Store the PTP 1588 timestamp for received PTP event frame. */
                    if (isPtpEventMessage)
                    {
                        /* Set the timestamp to the timestamp ring. */
                        ptpTimestamp.timeStamp.nanosecond = curBuffDescrip->timestamp;
                        result = ENET_StoreRxFrameTime(handle, &ptpTimestamp);
                    }
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
                    ENET_UpdateReadBuffers(handle);
                    return result;
                }
            }
            else
            {
                /* Store the fragments of a frame on several buffer descriptors. */
                isLastBuff = false;
                memcpy(data + offset, curBuffDescrip->buffer, handle->rxBuffSizeAlign);
                offset += handle->rxBuffSizeAlign;
                if (offset >= length)
                {
                    break;
                }
            }

            /* Get the current buffer descriptor. */
            curBuffDescrip = handle->rxBdCurrent;
        }
        /* All ERR happen will break while loop and arrive here to update receive buffers*/
        ENET_UpdateReadBuffers(handle);
    }
    return kStatus_ENET_RxFrameFail;
}

/*!
 * @brief Updates the used receive buffer descriptors of the ENET device.
 *
 */
static void ENET_UpdateReadBuffers(enet_handle_t *handle)
{
    assert(handle);

    ENET_Type *base = handle->base;

    do
    {
        /* Clears status. */
        handle->rxBdDirty->control &= ENET_BUFFDESCRIPTOR_RX_WRAP_MASK;
        /* Sets the receive buffer descriptor with the empty flag. */
        handle->rxBdDirty->control |= ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK;
        /* Increases the buffer descriptor to the next one. */
        handle->rxBdDirty = ENET_IncrRxBuffDescripIndex(handle, handle->rxBdDirty);

        /* Actives the receive buffer descriptor. */
        base->RDAR = ENET_RDAR_RDAR_MASK;
    } while (handle->rxBdDirty != handle->rxBdCurrent);
}

status_t ENET_SendFrame(enet_handle_t *handle, uint8_t *data, uint32_t length)
{
    assert(handle);
    assert(data);
    assert(length <= (ENET_FRAME_MAX_VALNFRAMELEN - 4));

    volatile enet_tx_bd_struct_t *curBuffDescrip = handle->txBdCurrent;
    uint32_t len = 0;
    uint32_t sizeleft = 0;
    ENET_Type *base = handle->base;

    /* Check if the transmit buffer is ready. */
    if (curBuffDescrip->control & ENET_BUFFDESCRIPTOR_TX_READY_MASK)
    {
        return kStatus_ENET_TxBdOnTransmit;
    }
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    bool isPtpEventMessage = false;
    /* Check PTP message with the PTP header. */
    isPtpEventMessage = ENET_Ptp1588ParseFrame(data, NULL, true);
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
    /* One transmit buffer is enough for one frame. */
    if (handle->txBuffSizeAlign >= length)
    {
        /* Copy data to the buffer for uDMA transfer. */
        memcpy(curBuffDescrip->buffer, data, length);
        /* Set data length. */
        curBuffDescrip->length = length;
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
        /* For enable the timestamp. */
        if (isPtpEventMessage)
        {
            curBuffDescrip->controlExtend1 |= ENET_BUFFDESCRIPTOR_TX_TIMESTAMP_MASK;
        }
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
        curBuffDescrip->control |= ENET_BUFFDESCRIPTOR_TX_READY_MASK | ENET_BUFFDESCRIPTOR_TX_LAST_MASK;

        /* Increase the buffer descriptor address. */
        handle->txBdCurrent = ENET_IncrTxBuffDescripIndex(handle, handle->txBdCurrent);
        /* Active the transmit buffer descriptor. */
        base->TDAR = ENET_TDAR_TDAR_MASK;
        return kStatus_ENET_Success;
    }
    else
    {
        /* One frame requires more than one transmit buffers. */
        while (curBuffDescrip)
        {
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
            /* For enable the timestamp. */
            if (isPtpEventMessage)
            {
                curBuffDescrip->controlExtend1 |= ENET_BUFFDESCRIPTOR_TX_TIMESTAMP_MASK;
            }
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */

            /* Increase the buffer descriptor address. */
            handle->txBdCurrent = ENET_IncrTxBuffDescripIndex(handle, handle->txBdCurrent);
            /* update the size left to be transmit. */
            sizeleft = length - len;
            if (sizeleft > handle->txBuffSizeAlign)
            { /* Data copy. */
                memcpy(curBuffDescrip->buffer, data + len, handle->txBuffSizeAlign);
                curBuffDescrip->length = handle->txBuffSizeAlign;
                len += handle->txBuffSizeAlign;
                /* Sets the control flag. */
                curBuffDescrip->control |= ENET_BUFFDESCRIPTOR_TX_READY_MASK;
                /* Active the transmit buffer descriptor*/
                base->TDAR = ENET_TDAR_TDAR_MASK;
            }
            else
            {
                memcpy(curBuffDescrip->buffer, data + len, sizeleft);
                curBuffDescrip->length = sizeleft;
                /* Set Last buffer wrap flag. */
                curBuffDescrip->control |= ENET_BUFFDESCRIPTOR_TX_READY_MASK | ENET_BUFFDESCRIPTOR_TX_LAST_MASK;
                /* Active the transmit buffer descriptor. */
                base->TDAR = ENET_TDAR_TDAR_MASK;
                return kStatus_ENET_Success;
            }

            /* Get the current buffer descriptor address. */
            curBuffDescrip = handle->txBdCurrent;
            if (curBuffDescrip->control & ENET_BUFFDESCRIPTOR_TX_READY_MASK)
            {
                return kStatus_ENET_TxBdOnTransmit;
            }
        }
        return kStatus_ENET_TxFrameFail;
    }
}

/*!
 * @brief Increases the receive buffer descriptor to the next one.
 *
 */
static volatile enet_rx_bd_struct_t *ENET_IncrRxBuffDescripIndex(enet_handle_t *handle,
                                                                 volatile enet_rx_bd_struct_t *curBuffDescrip)
{
    assert(handle);
    assert(curBuffDescrip);

    /* Increase the buffer descriptor, if it is the last one, increase to first one of the ring buffer. */
    if (curBuffDescrip->control & ENET_BUFFDESCRIPTOR_RX_WRAP_MASK)
    {
        curBuffDescrip = handle->rxBdBase;
    }
    else
    {
        curBuffDescrip++;
    }

    return curBuffDescrip;
}

/*!
 * @brief Increases the transmit buffer descriptor to the next one.
 *
 */
static volatile enet_tx_bd_struct_t *ENET_IncrTxBuffDescripIndex(enet_handle_t *handle,
                                                                 volatile enet_tx_bd_struct_t *curBuffDescrip)
{
    assert(handle);
    assert(curBuffDescrip);

    /* Increase the buffer descriptor. */
    if (curBuffDescrip->control & ENET_BUFFDESCRIPTOR_TX_WRAP_MASK)
    {
        curBuffDescrip = handle->txBdBase;
    }
    else
    {
        curBuffDescrip++;
    }
    return curBuffDescrip;
}

void ENET_AddMulticastGroup(ENET_Type *base, uint8_t *address)
{
    assert(address);

    uint32_t crc = 0xFFFFFFFFU;
    uint32_t count1 = 0;
    uint32_t count2 = 0;

    /* Calculates the CRC-32 polynomial on the multicast group address. */
    for (count1 = 0; count1 < ENET_FRAME_MACLEN; count1++)
    {
        uint8_t c = address[count1];
        for (count2 = 0; count2 < 0x08U; count2++)
        {
            if ((c ^ crc) & 1U)
            {
                crc >>= 1U;
                c >>= 1U;
                crc ^= 0xEDB88320U;
            }
            else
            {
                crc >>= 1U;
                c >>= 1U;
            }
        }
    }

    /* Enable a multicast group address. */
    if (!((crc >> 0x1FU) & 1U))
    {
        base->GALR = 1U << ((crc >> 0x1AU) & 0x1FU);
    }
    else
    {
        base->GAUR = 1U << ((crc >> 0x1AU) & 0x1FU);
    }
}

void ENET_LeaveMulticastGroup(ENET_Type *base, uint8_t *address)
{
    assert(address);

    uint32_t crc = 0xFFFFFFFFU;
    uint32_t count1 = 0;
    uint32_t count2 = 0;

    /* Calculates the CRC-32 polynomial on the multicast group address. */
    for (count1 = 0; count1 < ENET_FRAME_MACLEN; count1++)
    {
        uint8_t c = address[count1];
        for (count2 = 0; count2 < 0x08U; count2++)
        {
            if ((c ^ crc) & 1U)
            {
                crc >>= 1U;
                c >>= 1U;
                crc ^= 0xEDB88320U;
            }
            else
            {
                crc >>= 1U;
                c >>= 1U;
            }
        }
    }

    /* Set the hash table. */
    if (!((crc >> 0x1FU) & 1U))
    {
        base->GALR &= ~(1U << ((crc >> 0x1AU) & 0x1FU));
    }
    else
    {
        base->GAUR &= ~(1U << ((crc >> 0x1AU) & 0x1FU));
    }
}

#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
status_t ENET_GetTxErrAfterSendFrame(enet_handle_t *handle, enet_data_error_stats_t *eErrorStatic)
{
    assert(handle);
    assert(eErrorStatic);

    uint16_t control = 0;
    uint16_t controlExt = 0;

    do
    {
        /* Get the current dirty transmit buffer descriptor. */
        control = handle->txBdDirtyStatic->control;
        controlExt = handle->txBdDirtyStatic->controlExtend0;
        /* Get the control status data, If the buffer descriptor has not been processed break out. */
        if (control & ENET_BUFFDESCRIPTOR_TX_READY_MASK)
        {
            return kStatus_ENET_TxBdOnTransmit;
        }
        /* Increase the transmit dirty static pointer. */
        handle->txBdDirtyStatic = ENET_IncrTxBuffDescripIndex(handle, handle->txBdDirtyStatic);

        /* If the transmit buffer descriptor is ready and the last buffer descriptor, store packet statistic. */
        if (control & ENET_BUFFDESCRIPTOR_TX_LAST_MASK)
        {
            if (controlExt & ENET_BUFFDESCRIPTOR_TX_ERR_MASK)
            {
                /* Transmit error. */
                eErrorStatic->statsTxErr++;
            }
            if (controlExt & ENET_BUFFDESCRIPTOR_TX_EXCCOLLISIONERR_MASK)
            {
                /* Transmit excess collision error. */
                eErrorStatic->statsTxExcessCollisionErr++;
            }
            if (controlExt & ENET_BUFFDESCRIPTOR_TX_LATECOLLISIONERR_MASK)
            {
                /* Transmit late collision error. */
                eErrorStatic->statsTxLateCollisionErr++;
            }
            if (controlExt & ENET_BUFFDESCRIPTOR_TX_UNDERFLOWERR_MASK)
            {
                /* Transmit under flow error. */
                eErrorStatic->statsTxUnderFlowErr++;
            }
            if (controlExt & ENET_BUFFDESCRIPTOR_TX_OVERFLOWERR_MASK)
            {
                /* Transmit over flow error. */
                eErrorStatic->statsTxOverFlowErr++;
            }
            return kStatus_ENET_Success;
        }

    } while (handle->txBdDirtyStatic != handle->txBdCurrent);

    return kStatus_ENET_TxFrameFail;
}

/*!
 * @brief Parses the ENET packet and checks if it is a PTP event message.
 *
 */
static bool ENET_Ptp1588ParseFrame(uint8_t *packet, enet_ptp_time_data_t *ptpTsData, bool isFastEnabled)
{
    assert(packet);
    if (!isFastEnabled)
    {
        assert(ptpTsData);
    }

    bool isPtpMsg = false;
    uint8_t *buffer = packet;
    uint16_t ptpType;

    /* Check for VLAN frame. */
    if (*(uint16_t *)(buffer + ENET_PTP1588_ETHL2_PACKETTYPE_OFFSET) == ENET_HTONS(ENET_8021QVLAN))
    {
        buffer += (ENET_FRAME_VLAN_HEADERLEN - ENET_FRAME_HEADERLEN);
    }

    ptpType = *(uint16_t *)(buffer + ENET_PTP1588_ETHL2_PACKETTYPE_OFFSET);
    switch (ENET_HTONS(ptpType))
    { /* Ethernet layer 2. */
        case ENET_ETHERNETL2:
            if (*(uint8_t *)(buffer + ENET_PTP1588_ETHL2_MSGTYPE_OFFSET) <= kENET_PtpEventMsgType)
            {
                isPtpMsg = true;
                if (!isFastEnabled)
                {
                    /* It's a ptpv2 message and store the ptp header information. */
                    ptpTsData->version = (*(uint8_t *)(buffer + ENET_PTP1588_ETHL2_VERSION_OFFSET)) & 0x0F;
                    ptpTsData->messageType = (*(uint8_t *)(buffer + ENET_PTP1588_ETHL2_MSGTYPE_OFFSET)) & 0x0F;
                    ptpTsData->sequenceId = ENET_HTONS(*(uint16_t *)(buffer + ENET_PTP1588_ETHL2_SEQUENCEID_OFFSET));
                    memcpy((void *)&ptpTsData->sourcePortId[0], (void *)(buffer + ENET_PTP1588_ETHL2_CLOCKID_OFFSET),
                           kENET_PtpSrcPortIdLen);
                }
            }
            break;
        /* IPV4. */
        case ENET_IPV4:
            if ((*(uint8_t *)(buffer + ENET_PTP1588_IPVERSION_OFFSET) >> 4) == ENET_IPV4VERSION)
            {
                if (((*(uint16_t *)(buffer + ENET_PTP1588_IPV4_UDP_PORT_OFFSET)) == ENET_HTONS(kENET_PtpEventPort)) &&
                    (*(uint8_t *)(buffer + ENET_PTP1588_IPV4_UDP_PROTOCOL_OFFSET) == ENET_UDPVERSION))
                {
                    /* Set the PTP message flag. */
                    isPtpMsg = true;
                    if (!isFastEnabled)
                    {
                        /* It's a IPV4 ptp message and store the ptp header information. */
                        ptpTsData->version = (*(uint8_t *)(buffer + ENET_PTP1588_IPV4_UDP_VERSION_OFFSET)) & 0x0F;
                        ptpTsData->messageType = (*(uint8_t *)(buffer + ENET_PTP1588_IPV4_UDP_MSGTYPE_OFFSET)) & 0x0F;
                        ptpTsData->sequenceId =
                            ENET_HTONS(*(uint16_t *)(buffer + ENET_PTP1588_IPV4_UDP_SEQUENCEID_OFFSET));
                        memcpy((void *)&ptpTsData->sourcePortId[0],
                               (void *)(buffer + ENET_PTP1588_IPV4_UDP_CLKID_OFFSET), kENET_PtpSrcPortIdLen);
                    }
                }
            }
            break;
        /* IPV6. */
        case ENET_IPV6:
            if ((*(uint8_t *)(buffer + ENET_PTP1588_IPVERSION_OFFSET) >> 4) == ENET_IPV6VERSION)
            {
                if (((*(uint16_t *)(buffer + ENET_PTP1588_IPV6_UDP_PORT_OFFSET)) == ENET_HTONS(kENET_PtpEventPort)) &&
                    (*(uint8_t *)(buffer + ENET_PTP1588_IPV6_UDP_PROTOCOL_OFFSET) == ENET_UDPVERSION))
                {
                    /* Set the PTP message flag. */
                    isPtpMsg = true;
                    if (!isFastEnabled)
                    {
                        /* It's a IPV6 ptp message and store the ptp header information. */
                        ptpTsData->version = (*(uint8_t *)(buffer + ENET_PTP1588_IPV6_UDP_VERSION_OFFSET)) & 0x0F;
                        ptpTsData->messageType = (*(uint8_t *)(buffer + ENET_PTP1588_IPV6_UDP_MSGTYPE_OFFSET)) & 0x0F;
                        ptpTsData->sequenceId =
                            ENET_HTONS(*(uint16_t *)(buffer + ENET_PTP1588_IPV6_UDP_SEQUENCEID_OFFSET));
                        memcpy((void *)&ptpTsData->sourcePortId[0],
                               (void *)(buffer + ENET_PTP1588_IPV6_UDP_CLKID_OFFSET), kENET_PtpSrcPortIdLen);
                    }
                }
            }
            break;
        default:
            break;
    }
    return isPtpMsg;
}

void ENET_Ptp1588Configure(enet_handle_t *handle, enet_ptp_config_t *ptpConfig)
{
    assert(handle);
    assert(ptpConfig);

    ENET_Type *base = handle->base;

    /* Start the 1588 timer. */
    ENET_Ptp1588StartTimer(base, ptpConfig->ptpClockSrc, ptpConfig->channel);

    /* Enables the time stamp interrupt for the master clock on a device. */
    ENET_EnableInterrupts(base, kENET_TsTimerInterrupt);
    EnableIRQ(ENET_1588_Timer_IRQn);

    /* Enables the transmit interrupt to store the transmit frame timestamp. */
    ENET_EnableInterrupts(base, kENET_TxFrameInterrupt);
    EnableIRQ(ENET_Transmit_IRQn);

    /* Setting the receive and transmit state for transaction. */
    handle->rxPtpTsDataRing.ptpTsData = ptpConfig->rxPtpTsData;
    handle->rxPtpTsDataRing.size = ptpConfig->ptpTsRxBuffNum;
    handle->rxPtpTsDataRing.front = 0;
    handle->rxPtpTsDataRing.end = 0;
    handle->txPtpTsDataRing.ptpTsData = ptpConfig->txPtpTsData;
    handle->txPtpTsDataRing.size = ptpConfig->ptpTsTxBuffNum;
    handle->txPtpTsDataRing.front = 0;
    handle->txPtpTsDataRing.end = 0;
    handle->msTimerSecond = 0;
    handle->txBdDirtyTime = handle->txBdBase;
    handle->txBdDirtyStatic = handle->txBdBase;
#if FSL_FEATURE_ENET_PTP_TIMER_CHANNEL_INTERRUPT_ERRATA_2579
    /* Sets the channel used for timer stamp interrupt. */
    handle->channel = ptpConfig->channel;
#endif /* FSL_FEATURE_ENET_PTP_TIMER_CHANNEL_INTERRUPT_ERRATA_2579 */
}

void ENET_Ptp1588StartTimer(ENET_Type *base, uint32_t ptpClkSrc, enet_ptp_timer_channel_t channel)
{
    /* Restart PTP 1588 timer, master clock. */
    base->ATCR = ENET_ATCR_RESTART_MASK;

    /* Initializes PTP 1588 timer. */
    base->ATINC = ENET_ATINC_INC(ENET_NANOSECOND_ONE_SECOND / ptpClkSrc);
    base->ATPER = ENET_NANOSECOND_ONE_SECOND;
#if FSL_FEATURE_ENET_PTP_TIMER_CHANNEL_INTERRUPT_ERRATA_2579
    /* Set the first compare value before set the timer channel. */
    ENET_Ptp1588SetChannelCmpValue(base, channel,
                                   (ENET_NANOSECOND_ONE_SECOND - ENET_NANOSECOND_ONE_SECOND / ptpClkSrc));
    ENET_Ptp1588SetChannelMode(base, channel, kENET_PtpChannelToggleCompare, true);
    /* Set the second compare value after set the timer channel. */
    ENET_Ptp1588SetChannelCmpValue(base, channel,
                                   (ENET_NANOSECOND_ONE_SECOND - ENET_NANOSECOND_ONE_SECOND / ptpClkSrc));
#endif /* FSL_FEATURE_ENET_PTP_TIMER_CHANNEL_INTERRUPT_ERRATA_2579 */
    /* Sets periodical event and the event signal output assertion and Actives PTP 1588 timer.  */
    base->ATCR = ENET_ATCR_PEREN_MASK | ENET_ATCR_PINPER_MASK | ENET_ATCR_EN_MASK;
}

void ENET_Ptp1588GetTimer(enet_handle_t *handle, enet_ptp_time_t *ptpTime)
{
    assert(handle);
    assert(ptpTime);

    ENET_Type *base = handle->base;

    /* Get the nanosecond from the master timer. */
    base->ATCR |= ENET_ATCR_CAPTURE_MASK;
    /* Disable the interrupt. */
    __disable_irq();
    /* Get the current PTP time. */
    ptpTime->second = handle->msTimerSecond;
    ptpTime->nanosecond = base->ATVR;
    /* Enable the interrupt. */
    __enable_irq();
}

void ENET_Ptp1588SetTimer(enet_handle_t *handle, enet_ptp_time_t *ptpTime)
{
    assert(handle);
    assert(ptpTime);

    ENET_Type *base = handle->base;

    /* Disables the interrupt. */
    __disable_irq();
    /* Sets PTP timer. */
    handle->msTimerSecond = ptpTime->second;
    base->ATVR = ptpTime->nanosecond;
    /* Enables the interrupt. */
    __enable_irq();
}

void ENET_Ptp1588AdjustTimer(ENET_Type *base, uint32_t corrIncrease, uint32_t corrPeriod)
{
    /* Set correction for PTP timer increment. */
    base->ATINC = (base->ATINC & ~ENET_ATINC_INC_CORR_MASK) | ((uint32_t)corrIncrease << ENET_ATINC_INC_CORR_SHIFT);
    /* Set correction for PTP timer period. */
    base->ATCOR = (base->ATCOR & ~ENET_ATCOR_COR_MASK) | (corrPeriod << ENET_ATCOR_COR_SHIFT);
}

/*!
 * @brief Updates the new PTP 1588 timestamp to the timestamp buffer ring.
 *
 */
static status_t ENET_Ptp1588UpdateTimeRing(enet_ptp_time_data_ring_t *ptpTsDataRing,
                                                     enet_ptp_time_data_t *ptpTimeData)
{
    assert(ptpTsDataRing);
    assert(ptpTimeData);

    uint16_t usedBuffer = 0;

    /* Check if the buffers ring is full. */
    if (ptpTsDataRing->end > ptpTsDataRing->front)
    {
        usedBuffer = ptpTsDataRing->end - ptpTsDataRing->front;
    }
    else if (ptpTsDataRing->end < ptpTsDataRing->front)
    {
        usedBuffer = ptpTsDataRing->size - (ptpTsDataRing->front - ptpTsDataRing->end);
    }

    if (usedBuffer == ptpTsDataRing->size)
    {
        return kStatus_ENET_PtpTsRingFull;
    }

    /* Copy the new data into the buffer. */
    memcpy((ptpTsDataRing->ptpTsData + ptpTsDataRing->end), ptpTimeData, sizeof(enet_ptp_time_data_t));

    /* Increase the buffer pointer to the next empty one. */
    ptpTsDataRing->end = (ptpTsDataRing->end + 1) % ptpTsDataRing->size;

    return kStatus_ENET_Success;
}

/*!
 * @brief Store the receive timestamp in the timestamp buffer ring.
 * @note Store the timestamp for event PTP frame only.
 */
static status_t ENET_StoreRxFrameTime(enet_handle_t *handle, enet_ptp_time_data_t *ptpTimeData)
{
    assert(handle);
    assert(ptpTimeData);

    ENET_Type *base = handle->base;
    bool ptpTimerWrap = false;
    enet_ptp_time_t ptpTimer;

    /* Disable the interrupt. */
    __disable_irq();

    /* Get current PTP timer nanosecond value. */
    ENET_Ptp1588GetTimer(handle, &ptpTimer);

    /* Get PTP timer wrap event. */
    ptpTimerWrap = base->EIR & kENET_TsTimerInterrupt;

    /* Get transmit time stamp second. */
    if ((ptpTimer.nanosecond > ptpTimeData->timeStamp.nanosecond) ||
        ((ptpTimer.nanosecond < ptpTimeData->timeStamp.nanosecond) && ptpTimerWrap))
    {
        ptpTimeData->timeStamp.second = handle->msTimerSecond;
    }
    else
    {
        ptpTimeData->timeStamp.second = handle->msTimerSecond - 1;
    }
    /* Enable the interrupt. */
    __enable_irq();

    /* Store the timestamp to the receive time stamp ring. */
    /* Check if the buffers ring is full. */
    return ENET_Ptp1588UpdateTimeRing(&handle->rxPtpTsDataRing, ptpTimeData);
}

/*!
 * @brief Store the transmit timestamp in the timestamp buffer ring.
 * @note Store the timestamp for event PTP frame only.
 */
static status_t ENET_StoreTxFrameTime(enet_handle_t *handle)
{
    assert(handle);

    ENET_Type *base = handle->base;
    bool ptpTimerWrap;
    bool isPtpEventMessage = false;
    enet_ptp_time_data_t ptpTimeData;
    volatile enet_tx_bd_struct_t *curBuffDescrip = handle->txBdDirtyTime;

    /* Get the control status data, If the buffer descriptor has not been processed break out. */
    if (curBuffDescrip->control & ENET_BUFFDESCRIPTOR_TX_READY_MASK)
    {
        return kStatus_ENET_TxBdOnTransmit;
    }

    /* Parse the PTP message. */
    isPtpEventMessage = ENET_Ptp1588ParseFrame(curBuffDescrip->buffer, &ptpTimeData, false);
    if (isPtpEventMessage)
    {
        do
        {
            /* Increase current buffer descriptor to the next one. */
            handle->txBdDirtyTime = ENET_IncrTxBuffDescripIndex(handle, handle->txBdDirtyTime);

            /* Do time stamp check on the last buffer descriptor of the frame. */
            if (curBuffDescrip->control & ENET_BUFFDESCRIPTOR_TX_LAST_MASK)
            {
                /* Disable the interrupt. */
                __disable_irq();
                /* Get current PTP timer nanosecond value. */
                ENET_Ptp1588GetTimer(handle, &ptpTimeData.timeStamp);
                /* Get PTP timer wrap event. */
                ptpTimerWrap = base->EIR & kENET_TsTimerInterrupt;

                /* Get transmit time stamp second. */
                if ((ptpTimeData.timeStamp.nanosecond > curBuffDescrip->timestamp) ||
                    ((ptpTimeData.timeStamp.nanosecond < curBuffDescrip->timestamp) && ptpTimerWrap))
                {
                    ptpTimeData.timeStamp.second = handle->msTimerSecond;
                }
                else
                {
                    ptpTimeData.timeStamp.second = handle->msTimerSecond - 1;
                }

                /* Enable the interrupt. */
                __enable_irq();

                /* Store the timestamp to the transmit timestamp ring. */
                return ENET_Ptp1588UpdateTimeRing(&handle->txPtpTsDataRing, &ptpTimeData);
            }

            /* Get the current transmit buffer descriptor. */
            curBuffDescrip = handle->txBdDirtyTime;

            /* Get the control status data, If the buffer descriptor has not been processed break out. */
            if (curBuffDescrip->control & ENET_BUFFDESCRIPTOR_TX_READY_MASK)
            {
                return kStatus_ENET_TxBdOnTransmit;
            }
        } while (handle->txBdDirtyTime != handle->txBdCurrent);
        return kStatus_ENET_TxGetTimeFail;
    }
    return kStatus_ENET_Success;
}

/*!
 * @brief Search the PTP 1588 timestamp from the timestamp buffer ring.
 *
 */
static status_t ENET_Ptp1588SearchTimeRing(enet_ptp_time_data_ring_t *ptpTsDataRing,
                                                     enet_ptp_time_data_t *ptpTimedata)
{
    assert(ptpTsDataRing);
    assert(ptpTimedata);

    uint32_t index;
    uint32_t size;
    uint16_t usedBuffer = 0;

    /* Check the PTP 1588 timestamp ring. */
    if (ptpTsDataRing->front == ptpTsDataRing->end)
    {
        return kStatus_ENET_PtpTsRingEmpty;
    }

    /* Search the element in the ring buffer */
    index = ptpTsDataRing->front;
    size = ptpTsDataRing->size;
    while (index != ptpTsDataRing->end)
    {
        if (((ptpTsDataRing->ptpTsData + index)->sequenceId == ptpTimedata->sequenceId) &&
            (!memcmp(((void *)&(ptpTsDataRing->ptpTsData + index)->sourcePortId[0]),
                     (void *)&ptpTimedata->sourcePortId[0], kENET_PtpSrcPortIdLen)) &&
            ((ptpTsDataRing->ptpTsData + index)->version == ptpTimedata->version) &&
            ((ptpTsDataRing->ptpTsData + index)->messageType == ptpTimedata->messageType))
        {
            break;
        }

        /* Increase the ptp ring index. */
        index = (index + 1) % size;
    }

    if (index == ptpTsDataRing->end)
    {
        /* Check if buffers is full. */
        if (ptpTsDataRing->end > ptpTsDataRing->front)
        {
            usedBuffer = ptpTsDataRing->end - ptpTsDataRing->front;
        }
        else if (ptpTsDataRing->end < ptpTsDataRing->front)
        {
            usedBuffer = ptpTsDataRing->size - (ptpTsDataRing->front - ptpTsDataRing->end);
        }

        if (usedBuffer == ptpTsDataRing->size)
        { /* Drop one in the front. */
            ptpTsDataRing->front = (ptpTsDataRing->front + 1) % size;
        }
        return kStatus_ENET_PtpTsRingFull;
    }

    /* Get the right timestamp of the required ptp messag. */
    ptpTimedata->timeStamp.second = (ptpTsDataRing->ptpTsData + index)->timeStamp.second;
    ptpTimedata->timeStamp.nanosecond = (ptpTsDataRing->ptpTsData + index)->timeStamp.nanosecond;

    /* Increase the index. */
    ptpTsDataRing->front = (ptpTsDataRing->front + 1) % size;

    return kStatus_ENET_Success;
}

status_t ENET_GetTxFrameTime(enet_handle_t *handle, enet_ptp_time_data_t *ptpTimeData)
{
    assert(handle);
    assert(ptpTimeData);

    return ENET_Ptp1588SearchTimeRing(&handle->txPtpTsDataRing, ptpTimeData);
}

status_t ENET_GetRxFrameTime(enet_handle_t *handle, enet_ptp_time_data_t *ptpTimeData)
{
    assert(handle);
    assert(ptpTimeData);

    return ENET_Ptp1588SearchTimeRing(&handle->rxPtpTsDataRing, ptpTimeData);
}

#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */

void ENET_SetIRQCallback(enet_handle_t *handle, enet_callback_t callback, void *param)
{
    assert(handle != NULL);

    handle->callback = callback;
    handle->param = param;
}

void ENET_TransmitIRQHandler(enet_handle_t *handle)
{
    assert(handle);

    ENET_Type *base = handle->base;

    /* Check if the transmit interrupt happen. */
    if ((kENET_TxByteInterrupt | kENET_TxFrameInterrupt) & base->EIR)
    {
        /* Clear the transmit interrupt event. */
        base->EIR = kENET_TxFrameInterrupt | kENET_TxByteInterrupt;
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
        /* Store the transmit timestamp from the buffer descriptor should be done here. */
        ENET_StoreTxFrameTime(handle);
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
        /* Callback function. */
        if (handle->callback)
        {
            handle->callback(handle, kENET_TxEvent, handle->param);
        }
    }
}

void ENET_ReceiveIRQHandler(enet_handle_t *handle)
{
    assert(handle);

    ENET_Type *base = handle->base;

    /* Check if the receive interrupt happen. */
    if ((kENET_RxByteInterrupt | kENET_RxFrameInterrupt) & base->EIR)
    {
        /* Clear the transmit interrupt event. */
        base->EIR = kENET_RxFrameInterrupt | kENET_RxByteInterrupt;

        /* Callback function. */
        if (handle->callback)
        {
            handle->callback(handle, kENET_RxEvent, handle->param);
        }
    }
}

void ENET_ErrorIRQHandler(enet_handle_t *handle)
{
    assert(handle);

    ENET_Type *base = handle->base;
    uint32_t errMask = kENET_BabrInterrupt | kENET_BabtInterrupt | kENET_EBusERInterrupt | kENET_PayloadRxInterrupt |
                       kENET_LateCollisionInterrupt | kENET_RetryLimitInterrupt | kENET_UnderrunInterrupt;

    /* Check if the PTP time stamp interrupt happen. */
    if (kENET_WakeupInterrupt & base->EIR)
    {
        /* Clear the wakeup interrupt. */
        base->EIR = kENET_WakeupInterrupt;
        /* wake up and enter the normal mode. */
        ENET_EnableSleepMode(base, false);
        /* Callback function. */
        if (handle->callback)
        {
            handle->callback(handle, kENET_WakeUpEvent, handle->param);
        }
    }
    else
    {
        /* Clear the time stamp interrupt. */
        errMask &= base->EIR;
        base->EIR = errMask;
        /* Callback function. */
        if (handle->callback)
        {
            handle->callback(handle, kENET_ErrEvent, handle->param);
        }
    }
}
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
void ENET_Ptp1588TimerIRQHandler(enet_handle_t *handle)
{
    assert(handle);

    ENET_Type *base = handle->base;

    /* Check if the PTP time stamp interrupt happen. */
    if (kENET_TsTimerInterrupt & base->EIR)
    {
        /* Clear the time stamp interrupt. */
        base->EIR = kENET_TsTimerInterrupt;
/* Increase timer second counter. */
#if !FSL_FEATURE_ENET_PTP_TIMER_CHANNEL_INTERRUPT_ERRATA_2579
        handle->msTimerSecond++;
#else
        /* Load the compare value. */
        ENET_Ptp1588SetChannelCmpValue(base, handle->channel,
                                       (ENET_NANOSECOND_ONE_SECOND - ENET_NANOSECOND_ONE_SECOND / ptpClkSrc));
        ENET_Ptp1588ClearChannelStatus(base, handle->channel);
#endif /* !FSL_FEATURE_ENET_PTP_TIMER_CHANNEL_INTERRUPT_ERRATA_2579 */
        /* Callback function. */
        if (handle->callback)
        {
            handle->callback(handle, kENET_TimeStampEvent, handle->param);
        }
    }
    else if (kENET_TsAvailInterrupt & base->EIR)
    {
        /* Clear the time stamp interrupt. */
        base->EIR = kENET_TsAvailInterrupt;
        /* Callback function. */
        if (handle->callback)
        {
            handle->callback(handle, kENET_TimeStampAvailEvent, handle->param);
        }
    }
}
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */

void ENET_Transmit_IRQHandler(void)
{
    ENET_TransmitIRQHandler(g_ENETHandle[0]);
}

void ENET_Receive_IRQHandler(void)
{
    ENET_ReceiveIRQHandler(g_ENETHandle[0]);
}

void ENET_Error_IRQHandler(void)
{
    ENET_ErrorIRQHandler(g_ENETHandle[0]);
}

void ENET_1588_Timer_IRQHandler(void)
{
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    ENET_Ptp1588TimerIRQHandler(g_ENETHandle[0]);
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
}
