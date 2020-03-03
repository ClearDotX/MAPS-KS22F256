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
#ifndef _FSL_ENET_H_
#define _FSL_ENET_H_

#include "fsl_common.h"
/*!
 * @addtogroup enet_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the driver version. */
#define FSL_ENET_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */

/*! @brief Defines the control and status region bit masks of the receive buffer descriptor. */
#define ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK 0x8000U       /*!< Empty bit mask. */
#define ENET_BUFFDESCRIPTOR_RX_SOFTOWNER1_MASK 0x4000U  /*!< Software owner one mask. */
#define ENET_BUFFDESCRIPTOR_RX_WRAP_MASK 0x2000U        /*!< Next buffer descriptor is the start address. */
#define ENET_BUFFDESCRIPTOR_RX_SOFTOWNER2_Mask 0x1000U  /*!< Software owner two mask. */
#define ENET_BUFFDESCRIPTOR_RX_LAST_MASK 0x0800U        /*!< Last BD of the frame mask. */
#define ENET_BUFFDESCRIPTOR_RX_MISS_MASK 0x0100U        /*!< Received because of the promiscuous mode. */
#define ENET_BUFFDESCRIPTOR_RX_BROADCAST_MASK 0x0080U   /*!< Broadcast packet mask. */
#define ENET_BUFFDESCRIPTOR_RX_MULTICAST_MASK 0x0040U   /*!< Multicast packet mask. */
#define ENET_BUFFDESCRIPTOR_RX_LENVLIOLATE_MASK 0x0020U /*!< Length violation mask. */
#define ENET_BUFFDESCRIPTOR_RX_NOOCTET_MASK 0x0010U     /*!< Non-octet aligned frame mask. */
#define ENET_BUFFDESCRIPTOR_RX_CRC_MASK 0x0004U         /*!< CRC error mask. */
#define ENET_BUFFDESCRIPTOR_RX_OVERRUN_MASK 0x0002U     /*!< FIFO overrun mask. */
#define ENET_BUFFDESCRIPTOR_RX_TRUNC_MASK 0x0001U       /*!< Frame is truncated mask. */

/*! @brief Defines the control and status bit masks of the transmit buffer descriptor. */
#define ENET_BUFFDESCRIPTOR_TX_READY_MASK 0x8000U       /*!< Ready bit mask. */
#define ENET_BUFFDESCRIPTOR_TX_SOFTOWENER1_MASK 0x4000U /*!< Software owner one mask. */
#define ENET_BUFFDESCRIPTOR_TX_WRAP_MASK 0x2000U        /*!< Wrap buffer descriptor mask. */
#define ENET_BUFFDESCRIPTOR_TX_SOFTOWENER2_MASK 0x1000U /*!< Software owner two mask. */
#define ENET_BUFFDESCRIPTOR_TX_LAST_MASK 0x0800U        /*!< Last BD of the frame mask. */
#define ENET_BUFFDESCRIPTOR_TX_TRANMITCRC_MASK 0x0400U  /*!< Transmit CRC mask. */

/*! @brief Defines the extended control regions for enhanced buffer descriptors. */
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
/*! @brief Defines the first extended control region bit masks of the receive buffer descriptor. */
#define ENET_BUFFDESCRIPTOR_RX_IPV4_MASK 0x0001U             /*!< Ipv4 frame mask. */
#define ENET_BUFFDESCRIPTOR_RX_IPV6_MASK 0x0002U             /*!< Ipv6 frame mask. */
#define ENET_BUFFDESCRIPTOR_RX_VLAN_MASK 0x0004U             /*!< VLAN frame mask. */
#define ENET_BUFFDESCRIPTOR_RX_PROTOCOLCHECKSUM_MASK 0x0010U /*!< Protocol checksum error mask. */
#define ENET_BUFFDESCRIPTOR_RX_IPHEADCHECKSUM_MASK 0x0020U   /*!< IP header checksum error mask. */

/*! @brief Defines the second extended control region bit masks of the receive buffer descriptor. */
#define ENET_BUFFDESCRIPTOR_RX_INTERRUPT_MASK 0x0080U /*!< BD interrupt mask. */
#define ENET_BUFFDESCRIPTOR_RX_UNICAST_MASK 0x0100U   /*!< Unicast frame mask. */
#define ENET_BUFFDESCRIPTOR_RX_COLLISION_MASK 0x0200U /*!< BD collision mask. */
#define ENET_BUFFDESCRIPTOR_RX_PHYERR_MASK 0x0400U    /*!< PHY error mask. */
#define ENET_BUFFDESCRIPTOR_RX_MACERR_MASK 0x8000U    /*!< Mac error mask. */

/*! @brief Defines the first extended control region bit masks of the transmit buffer descriptor. */
#define ENET_BUFFDESCRIPTOR_TX_ERR_MASK 0x8000U              /*!< Transmit error mask. */
#define ENET_BUFFDESCRIPTOR_TX_UNDERFLOWERR_MASK 0x2000U     /*!< Underflow error mask. */
#define ENET_BUFFDESCRIPTOR_TX_EXCCOLLISIONERR_MASK 0x1000U  /*!< Excess collision error mask. */
#define ENET_BUFFDESCRIPTOR_TX_FRAMEERR_MASK 0x0800U         /*!< Frame error mask. */
#define ENET_BUFFDESCRIPTOR_TX_LATECOLLISIONERR_MASK 0x0400U /*!< Late collision error mask. */
#define ENET_BUFFDESCRIPTOR_TX_OVERFLOWERR_MASK 0x0200U      /*!< Overflow error mask. */
#define ENET_BUFFDESCRIPTOR_TX_TIMESTAMPERR_MASK 0x0100U     /*!< Timestamp error mask. */

/*! @brief Defines the second extended control region bit masks of the transmit buffer descriptor. */
#define ENET_BUFFDESCRIPTOR_TX_INTERRUPT_MASK 0x4000U /*!< Interrupt mask. */
#define ENET_BUFFDESCRIPTOR_TX_TIMESTAMP_MASK 0x2000U /*!< Timestamp flag mask. */
#endif                                                /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */

/*! @brief Defines the receive error status flag mask. */
#define ENET_BUFFDESCRIPTOR_RX_ERR_MASK     \
    (ENET_BUFFDESCRIPTOR_RX_TRUNC_MASK | ENET_BUFFDESCRIPTOR_RX_OVERRUN_MASK \
| ENET_BUFFDESCRIPTOR_RX_LENVLIOLATE_MASK | \
     ENET_BUFFDESCRIPTOR_RX_NOOCTET_MASK | ENET_BUFFDESCRIPTOR_RX_CRC_MASK)
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
#define ENET_BUFFDESCRIPTOR_RX_EXT_ERR_MASK \
    (ENET_BUFFDESCRIPTOR_RX_MACERR_MASK | ENET_BUFFDESCRIPTOR_RX_PHYERR_MASK \
| ENET_BUFFDESCRIPTOR_RX_COLLISION_MASK)
#endif

/*! @brief Defines the maximum ethernet frame size. */
#define ENET_FRAME_MAX_FRAMELEN 1518U     /*!< Maximum Ethernet frame size. */
#define ENET_FRAME_MAX_VALNFRAMELEN 1522U /*!< Maximum VLAN frame size. */

/*! @brief Defines the threshold for FIFO . */
#define ENET_FIFO_MIN_RX_FULL 5U /*!< ENET minimum receive FIFO full. */

/*! @brief Defines the minimum receive buffer size - will be defined in the feature file. */
#define ENET_RX_MIN_BUFFERSIZE 256U /*!< ENET minimum buffer size. */

/*! @brief Defines the alignment for buffer descriptor - will be defined in the feature file. */
#define ENET_BUFF_ALIGNMENT 16U /*!< Ethernet buffer alignment. */

/*! @brief Defines the PHY address scope for the ENET. */
#define ENET_PHY_MAXADDRESS (ENET_MMFR_PA_MASK >> ENET_MMFR_PA_SHIFT)

/*! @brief Defines the status return codes for transaction. */
enum _enet_status
{
    kStatus_ENET_Success = MAKE_STATUS(kStatusGroup_ENET, 0U),        /*!< ENET done successfully. */
    kStatus_ENET_RxFrameError = MAKE_STATUS(kStatusGroup_ENET, 1U),   /*!< Receive data error. */
    kStatus_ENET_RxFrameFail = MAKE_STATUS(kStatusGroup_ENET, 2U),    /*!< Receive frame fail. */
    kStatus_ENET_RxFrameEmpty = MAKE_STATUS(kStatusGroup_ENET, 3U),   /*!< Receive buffer empty no data. */
    kStatus_ENET_RxGetTimeFail = MAKE_STATUS(kStatusGroup_ENET, 4U),  /*!< Transmit get timestamp fail. */
    kStatus_ENET_TxGetTimeFail = MAKE_STATUS(kStatusGroup_ENET, 5U),  /*!< Receive buffer descriptors
                                                                        have no last flag. */
    kStatus_ENET_TxBdOnTransmit = MAKE_STATUS(kStatusGroup_ENET, 6U), /*!< Transmit buffer descriptors
                                                                        are under process. */
    kStatus_ENET_TxFrameFail = MAKE_STATUS(kStatusGroup_ENET, 7U)     /*!< Transmit frame fail. */
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    ,
    kStatus_ENET_PtpTsRingFull = MAKE_STATUS(kStatusGroup_ENET, 8U), /*!< Timestamp ring full. */
    kStatus_ENET_PtpTsRingEmpty = MAKE_STATUS(kStatusGroup_ENET, 9U) /*!< Timestamp ring empty. */
#endif                                                               /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
};

/*! @brief Defines the RMII or MII mode for data interface between the MAC and the PHY. */
typedef enum _enet_mii_mode
{
    kENET_MiiMode = 0U, /*!< MII mode for data interface. */
    kENET_RmiiMode      /*!< RMII mode for data interface. */
} enet_mii_mode_t;

/*! @brief Defines the 10 Mbps or 100 Mbps speed for the  MII data interface. */
typedef enum _enet_mii_speed
{
    kENET_MiiSpeed10M = 0U, /*!< Speed 10 Mbps. */
    kENET_MiiSpeed100M      /*!< Speed 100 Mbps. */
} enet_mii_speed_t;

/*! @brief Defines the half or full duplex for the MII data interface. */
typedef enum _enet_mii_duplex
{
    kENET_MiiHalfDuplex = 0U, /*!< Half duplex mode. */
    kENET_MiiFullDuplex       /*!< Full duplex mode. */
} enet_mii_duplex_t;

/*! @brief Defines the write operation for the MII management frame. */
typedef enum _enet_mii_write
{
    kENET_MiiWriteNoCompliant = 0U, /*!< Write frame operation, but not MII compliant. */
    kENET_MiiWriteValidFrame        /*!< Write frame operation for a valid MII management frame. */
} enet_mii_write_t;

/*! @brief Defines the read operation for the MII management frame. */
typedef enum _enet_mii_read
{
    kENET_MiiReadValidFrame = 2U, /*!< Read frame operation for a valid MII management frame. */
    kENET_MiiReadNoCompliant = 3U /*!< Read frame operation, but not MII compliant. */
} enet_mii_read_t;

/*! @brief Defines some special configure for ENET MAC controller.
 *
 * These control flag are provided for the special use requirement.
 * In normal, these control flags are usually unused for ENET initialize.
 * If you have any special requirement in these flags just set to
 * the macSpecialConfig in the enet_config_t.
 * The kENET_ControlStoreAndFwdDisable is to disable the FIFO store
 * and forward. FIFO store and forward is that the FIFO read/send will be start
 * when a complete frame is stored in tx/rx FIFO. If this flag set
 * please make sure to configure rxFifoFullThreshold and txFifoWatermark
 * in the enet_config_t.
 */
typedef enum _enet_special_control_flag
{
    kENET_ControlFlowControlEnable = 0x0001U,       /*!< Enable ENET flow control: pause frame. */
    kENET_ControlRxPayloadCheckEnable = 0x0002U,    /*!< Enable ENET receive payload length check. */
    kENET_ControlRxPadRemoveEnable = 0x0004U,       /*!< Padding is removed from received frames. */
    kENET_ControlRxBroadCastRejectEnable = 0x0008U, /*!< Enable broadcast frame reject. */
    kENET_ControlMacAddrInsert = 0x0010U,           /*!< Enable MAC address insert. */
    kENET_ControlStoreAndFwdDisable = 0x0020U,      /*!< Enable FIFO store and forward. */
    kENET_ControlSMIPreambleDisable = 0x0040U,      /*!< Enable SMI preamble. */
    kENET_ControlPromiscuousEnable = 0x0080U,       /*!< Enable promiscuous mode. */
    kENET_ControlMIILoopEnable = 0x0100U,           /*!< Enable ENET MII loop back. */
    kENET_ControlVLANTagEnable = 0x0200U            /*!< Enable VLAN tag frame. */
} enet_special_control_flag_t;

/*! @brief List of interrupts supported by the peripheral. This
 * enumeration uses one-bot encoding to allow a logical OR of multiple
 * members. Members usually map to interrupt enable bits in one or more
 * peripheral registers.
 */
typedef enum _enet_interrupt_enable
{
    kENET_BabrInterrupt = ENET_EIR_BABR_MASK,        /*!< Babbling receive error interrupt source */
    kENET_BabtInterrupt = ENET_EIR_BABT_MASK,        /*!< Babbling transmit error interrupt source */
    kENET_GraceStopInterrupt = ENET_EIR_GRA_MASK,    /*!< Graceful stop complete interrupt source */
    kENET_TxFrameInterrupt = ENET_EIR_TXF_MASK,      /*!< TX FRAME interrupt source */
    kENET_TxByteInterrupt = ENET_EIR_TXB_MASK,       /*!< TX BYTE interrupt source */
    kENET_RxFrameInterrupt = ENET_EIR_RXF_MASK,      /*!< RX FRAME interrupt source */
    kENET_RxByteInterrupt = ENET_EIR_RXB_MASK,       /*!< RX BYTE interrupt source */
    kENET_MiiInterrupt = ENET_EIR_MII_MASK,          /*!< MII interrupt source */
    kENET_EBusERInterrupt = ENET_EIR_EBERR_MASK,     /*!< Ethernet bus error interrupt source */
    kENET_LateCollisionInterrupt = ENET_EIR_LC_MASK, /*!< Late collision interrupt source */
    kENET_RetryLimitInterrupt = ENET_EIR_RL_MASK,    /*!< Collision Retry Limit interrupt source */
    kENET_UnderrunInterrupt = ENET_EIR_UN_MASK,      /*!< Transmit FIFO underrun interrupt source */
    kENET_PayloadRxInterrupt = ENET_EIR_PLR_MASK,    /*!< Payload Receive interrupt source */
    kENET_WakeupInterrupt = ENET_EIR_WAKEUP_MASK     /*!< WAKEUP interrupt source */
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    ,
    kENET_TsAvailInterrupt = ENET_EIR_TS_AVAIL_MASK, /*!< TS AVAIL interrupt source for PTP */
    kENET_TsTimerInterrupt = ENET_EIR_TS_TIMER_MASK  /*!< TS WRAP interrupt source for PTP */
#endif                                               /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
} enet_interrupt_enable_t;

/*! @brief Defines the common interrupt event for callback use. */
typedef enum _enet_event
{
    kENET_RxEvent,     /*!< Receive event. */
    kENET_TxEvent,     /*!< Transmit event. */
    kENET_ErrEvent,    /*!< Error event: BABR/BABT/EBERR/LC/RL/UN/PLR . */
    kENET_WakeUpEvent, /*!< Wake up from sleep mode event. */
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    kENET_TimeStampEvent,     /*!< Time stamp event. */
    kENET_TimeStampAvailEvent /*!< Time stamp available event.*/
#endif                        /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
} enet_event_t;

/*! @brief Defines the transmit accelerator configuration. */
typedef enum _enet_tx_accelerator
{
    kENET_TxAccelIsShift16Enabled = ENET_TACC_SHIFT16_MASK, /*!< Transmit FIFO shift-16. */
    kENET_TxAccelIpCheckEnabled = ENET_TACC_IPCHK_MASK,     /*!< Insert IP header checksum. */
    kENET_TxAccelProtoCheckEnabled = ENET_TACC_PROCHK_MASK  /*!< Insert protocol checksum. */
} enet_tx_accelerator_t;

/*! @brief Defines the receive accelerator configuration. */
typedef enum _enet_rx_accelerator
{
    kENET_RxAccelPadRemoveEnabled = ENET_RACC_PADREM_MASK,  /*!< Padding removal for short IP frames. */
    kENET_RxAccelIpCheckEnabled = ENET_RACC_IPDIS_MASK,     /*!< Discard with wrong IP header checksum. */
    kENET_RxAccelProtoCheckEnabled = ENET_RACC_PRODIS_MASK, /*!< Discard with wrong protocol checksum. */
    kENET_RxAccelMacCheckEnabled = ENET_RACC_LINEDIS_MASK,  /*!< Discard with Mac layer errors. */
    kENET_RxAccelisShift16Enabled = ENET_RACC_SHIFT16_MASK  /*!< Receive FIFO shift-16. */
} enet_rx_accelerator_t;

#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
/*! @brief Defines the ENET PTP message related constant. */
typedef enum _enet_ptp_event_type
{
    kENET_PtpEventMsgType = 3U,  /*!< PTP event message type. */
    kENET_PtpSrcPortIdLen = 10U, /*!< PTP message sequence id length. */
    kENET_PtpEventPort = 319U,   /*!< PTP event port number. */
    kENET_PtpGnrlPort = 320U     /*!< PTP general port number. */
} enet_ptp_event_type_t;

/*! @brief Defines the IEEE 1588 PTP timer channel numbers. */
typedef enum _enet_ptp_timer_channel
{
    kENET_PtpTimerChannel1 = 0U, /*!< IEEE 1588 PTP timer Channel 1. */
    kENET_PtpTimerChannel2,      /*!< IEEE 1588 PTP timer Channel 2. */
    kENET_PtpTimerChannel3,      /*!< IEEE 1588 PTP timer Channel 3. */
    kENET_PtpTimerChannel4       /*!< IEEE 1588 PTP timer Channel 4. */
} enet_ptp_timer_channel_t;

/*! @brief Defines the capture or compare mode for IEEE 1588 PTP timer channels. */
typedef enum _enet_ptp_timer_channel_mode
{
    kENET_PtpChannelDisable = 0U,                  /*!< Disable timer channel. */
    kENET_PtpChannelRisingCapture = 1U,            /*!< Input capture on rising edge. */
    kENET_PtpChannelFallingCapture = 2U,           /*!< Input capture on falling edge. */
    kENET_PtpChannelBothCapture = 3U,              /*!< Input capture on both edges. */
    kENET_PtpChannelSoftCompare = 4U,              /*!< Output compare software only. */
    kENET_PtpChannelToggleCompare = 5U,            /*!< Toggle output on compare. */
    kENET_PtpChannelClearCompare = 6U,             /*!< Clear output on compare. */
    kENET_PtpChannelSetCompare = 7U,               /*!< Set output on compare. */
    kENET_PtpChannelClearCompareSetOverflow = 10U, /*!< Clear output on compare, set output on overflow. */
    kENET_PtpChannelSetCompareClearOverflow = 11U, /*!< Set output on compare, clear output on overflow. */
    kENET_PtpChannelPulseLowonCompare = 14U,       /*!< Pulse output low on compare for one IEEE 1588 clock cycle. */
    kENET_PtpChannelPulseHighonCompare = 15U       /*!< Pulse output high on compare for one IEEE 1588 clock cycle. */
} enet_ptp_timer_channel_mode_t;
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */

/*! @brief Defines the receive buffer descriptor structure for the little-endian system.*/
typedef struct _enet_rx_bd_struct
{
    uint16_t length;  /*!< Buffer descriptor data length. */
    uint16_t control; /*!< Buffer descriptor control and status. */
    uint8_t *buffer;  /*!< Data buffer pointer. */
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    uint16_t controlExtend0;  /*!< Extend buffer descriptor control0. */
    uint16_t controlExtend1;  /*!< Extend buffer descriptor control1. */
    uint16_t payloadCheckSum; /*!< Internal payload checksum. */
    uint8_t headerLength;     /*!< Header length. */
    uint8_t protocolTyte;     /*!< Protocol type. */
    uint16_t reserved0;
    uint16_t controlExtend2; /*!< Extend buffer descriptor control2. */
    uint32_t timestamp;      /*!< Timestamp. */
    uint16_t reserved1;
    uint16_t reserved2;
    uint16_t reserved3;
    uint16_t reserved4;
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
} enet_rx_bd_struct_t;

/*! @brief Defines the enhanced transmit buffer descriptor structure for the little-endian system. */
typedef struct _enet_tx_bd_struct
{
    uint16_t length;  /*!< Buffer descriptor data length. */
    uint16_t control; /*!< Buffer descriptor control and status. */
    uint8_t *buffer;  /*!< Data buffer pointer. */
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    uint16_t controlExtend0; /*!< Extend buffer descriptor control0. */
    uint16_t controlExtend1; /*!< Extend buffer descriptor control1. */
    uint16_t reserved0;
    uint16_t reserved1;
    uint16_t reserved2;
    uint16_t controlExtend2; /*!< Extend buffer descriptor control2. */
    uint32_t timestamp;      /*!< Timestamp. */
    uint16_t reserved3;
    uint16_t reserved4;
    uint16_t reserved5;
    uint16_t reserved6;
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
} enet_tx_bd_struct_t;

/*! @brief Defines the ENET data error statistic structure. */
typedef struct _enet_data_error_stats
{
    uint32_t statsRxLenGreaterErr; /*!< Receive length greater than RCR[MAX_FL]. */
    uint32_t statsRxAlignErr;      /*!< Receive non-octet alignment/ */
    uint32_t statsRxFcsErr;        /*!< Receive CRC error. */
    uint32_t statsRxOverRunErr;    /*!< Receive over run. */
    uint32_t statsRxTruncateErr;   /*!< Receive truncate. */
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    uint32_t statsRxProtocolChecksumErr; /*!< Receive protocol checksum error. */
    uint32_t statsRxIpHeadChecksumErr;   /*!< Receive IP header checksum error. */
    uint32_t statsRxMacErr;              /*!< Receive Mac error. */
    uint32_t statsRxPhyErr;              /*!< Receive PHY error. */
    uint32_t statsRxCollisionErr;        /*!< Receive collision. */
    uint32_t statsTxErr;                 /*!< The error happen when transmit the frame. */
    uint32_t statsTxFrameErr;            /*!< The transmit frame is error. */
    uint32_t statsTxOverFlowErr;         /*!< Transmit overflow. */
    uint32_t statsTxLateCollisionErr;    /*!< Transmit late collision. */
    uint32_t statsTxExcessCollisionErr;  /*!< Transmit excess collision.*/
    uint32_t statsTxUnderFlowErr;        /*!< Transmit under flow error. */
    uint32_t statsTxTsErr;               /*!< Transmit time stamp error. */
#endif                                   /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
} enet_data_error_stats_t;

/*! @brief Defines the receive buffer descriptor configure structure.
 *
 * Note: For the internal DMA requirement, the buffers have the alignment requirement:
 * 1. The aligned receive and transmit buffer size must be evenly divisible by 16.
 * 2. The aligned transmit and receive buffer descriptor start address must be at
 *    least 64 bit aligned, however it's recommended to be evenly divisible by 16.
 * 3. The aligned transmit and receive buffer start address must be evenly divisible by 16.
 *    receive buffers should be continuous with total size equal to "rxBdNumber * rxBuffSizeAlign".
 *    transmit buffers should be continuous with total size equal to "txBdNumber * txBuffSizeAlign".
 */
typedef struct _enet_buffer_config
{
    uint16_t rxBdNumber;                              /*!< Receive buffer descriptor number. */
    uint16_t txBdNumber;                              /*!< Transmit buffer descriptor number. */
    uint32_t rxBuffSizeAlign;                         /*!< Aligned receive data buffer size. */
    uint32_t txBuffSizeAlign;                         /*!< Aligned transmit data buffer size. */
    volatile enet_rx_bd_struct_t *rxBdStartAddrAlign; /*!< Aligned receive buffer descriptor start address. */
    volatile enet_tx_bd_struct_t *txBdStartAddrAlign; /*!< Aligned transmit buffer descriptor start address. */
    uint8_t *rxBufferAlign;                           /*!< Receive data buffer start address. */
    uint8_t *txBufferAlign;                           /*!< Transmit data buffer start address. */
} enet_buffer_config_t;

#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
/*! @brief Defines the ENET PTP time stamp structure. */
typedef struct _enet_ptp_time
{
    uint64_t second;     /*!< Second. */
    uint32_t nanosecond; /*!< Nanosecond. */
} enet_ptp_time_t;

/*! @brief Defines the structure for the ENET PTP message data and timestamp data.*/
typedef struct _enet_ptp_time_data
{
    uint8_t version;                             /*!< PTP version */
    uint8_t sourcePortId[kENET_PtpSrcPortIdLen]; /*!< PTP source port ID */
    uint16_t sequenceId;                         /*!< PTP sequence ID */
    uint8_t messageType;                         /*!< PTP message type */
    enet_ptp_time_t timeStamp;                   /*!< PTP timestamp */
} enet_ptp_time_data_t;

/*! @brief Defines the ENET PTP ring buffer structure for the PTP message timestamp store.*/
typedef struct _enet_ptp_time_data_ring
{
    uint32_t front;                  /*!< The first index of the ring */
    uint32_t end;                    /*!< The end index of the ring */
    uint32_t size;                   /*!< The size of the ring */
    enet_ptp_time_data_t *ptpTsData; /*!< PTP message data structure */
} enet_ptp_time_data_ring_t;

/*! @brief Defines the ENET PTP configure structure. */
typedef struct _enet_ptp_config
{
    uint8_t ptpTsRxBuffNum;            /*!< Receive 1588 timestamp buffer number*/
    uint8_t ptpTsTxBuffNum;            /*!< Transmit 1588 timestamp buffer number*/
    enet_ptp_time_data_t *rxPtpTsData; /*!< The start address of 1588 receive timestamp buffers */
    enet_ptp_time_data_t *txPtpTsData; /*!< The start address of 1588 transmit timestamp buffers */
    enet_ptp_timer_channel_t channel;  /*!< Used for ERRATA_2579: the PTP 1588 timer channel for time interrupt. */
    uint32_t ptpClockSrc;              /*!< The clock source of the PTP 1588 timer. */
} enet_ptp_config_t;
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */

/*! @brief Defines the basic configuration structure for the ENET device.
 *
 * Note:
 *  1. macSpecialConfig is for special control configure, A logical OR of
 *  "enet_special_control_flag_t". If you have any special configure for MAC,
 *  just set this parameter to 0.
 *  2. txWatermark is for cut-through operation, it is in steps of 64 bytes:
 *  0/1  - 64 bytes written to tx FIFO before transmission of a frame begins.
 *  2    - 128 bytes written to tx FIFO ....
 *  3    - 192 bytes written to tx FIFO ....
 *  the maximum of txWatermark is 0x2F   - 4032 bytes written to tx FIFO ....
 *  txWatermark allows you to minimize transmit latency to set txWatermark to 0 or 1.
 *  or for larger bus access latency 3 or larger due to contention for the system bus.
 *  3. rxFifoFullThreshold is similar to txWatermark for cut-through operation in rx.
 *  it is in 64-bit words. the minimum is ENET_FIFO_MIN_RX_FULL and the maximum is 0xFF.
 *  If the end of frame is stored in FIFO and the frame size if smaller than the
 *  txWatermark, the frame will still be transmit. The rule  is the
 *  same for rxFifoFullThreshold in receive direction.
 *  4. When "kENET_ControlFlowControlEnable" is set in macSpecialConfig, make sure
 *  the pauseDuration, rxFifoEmptyThreshold and rxFifoStatEmptyThreshold
 *  are set for flow control enabled case.
 *  5. When "kENET_ControlStoreAndFwdDisabled" is set in macSpecialConfig, make sure
 *  the rxFifoFullThreshold and txFifoWatermark are set for store and forward disable.
 *  6. The rxAccelerConfig and txAccelerConfig default setting with 0 - accelerator
 *  are disabled. The "enet_tx_accelerator_t" and "enet_rx_accelerator_t" are
 *  recommended to be used to enable the transmit and receive accelerator.
 *  Once the accelerators are enabled, the store and forward feature should be enabled.
 *  So kENET_ControlStoreAndFwdDisabled should not be set.
 */
typedef struct _enet_config
{
    uint32_t macSpecialConfig;    /*!< Mac special configure. A logical OR of "enet_special_control_flag_t". */
    uint32_t interrupt;           /*!< Mac interrupt source. A logical OR of "enet_interrupt_enable_t". */
    uint16_t rxMaxFrameLen;       /*!< Receive maximum frame length. */
    enet_mii_mode_t miiMode;      /*!< MII mode. */
    enet_mii_speed_t miiSpeed;    /*!< MII Speed. */
    enet_mii_duplex_t miiDuplex;  /*!< MII duplex. */
    uint8_t rxAccelerConfig;      /*!< Receive accelerator, A logical OR of "enet_rx_accelerator_t". */
    uint8_t txAccelerConfig;      /*!< Transmit accelerator, A logical OR of "enet_rx_accelerator_t". */
    uint16_t pauseDuration;       /*!< For flow control enabled case: Pause duration. */
    uint8_t rxFifoEmptyThreshold; /*!< For flow control enabled case:  when rx FIFO level reaches this value,
                                     it will make MAC generate XOFF pause frame. */
#if FSL_FEATURE_ENET_HAS_RECEIVE_STATUS_THRESHOLD
    uint8_t rxFifoStatEmptyThreshold; /*!< For flow control enabled case: number of frames in the receive FIFO,
                                    independent of size, that can be accept. If the limit is reached, reception
                                    will continue and a pause frame will be triggered. */
#endif                                /* FSL_FEATURE_ENET_HAS_RECEIVE_STATUS_THRESHOLD */
    uint8_t rxFifoFullThreshold;      /*!< For store and forward disable case, the data required in rx FIFO to notify
                                      the MAC receive ready status. */
    uint8_t txFifoWatermark;          /*!< For store and forward disable case, the data required in tx FIFO
                                      before a frame transmit start. */
} enet_config_t;

/*! @brief Defines the ENET handler structure. */
typedef struct _enet_handle
{
    ENET_Type *base;                           /*!< ENET peripheral base address. */
    volatile enet_rx_bd_struct_t *rxBdBase;    /*!< Receive buffer descriptor base address pointer. */
    volatile enet_rx_bd_struct_t *rxBdCurrent; /*!< The current available receive buffer descriptor pointer. */
    volatile enet_rx_bd_struct_t *rxBdDirty;   /*!< The dirty receive buffer descriptor needed to be updated from. */
    volatile enet_tx_bd_struct_t *txBdBase;    /*!< Transmit buffer descriptor base address pointer. */
    volatile enet_tx_bd_struct_t *txBdCurrent; /*!< The current available transmit buffer descriptor pointer. */
    uint32_t rxBuffSizeAlign;                  /*!< Receive buffer size alignment. */
    uint32_t txBuffSizeAlign;                  /*!< Transmit buffer size alignment. */
    void (*callback)(struct _enet_handle *handle, enet_event_t event, void *param);
    void *param; /*!< ENET callback function parameter.*/
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    volatile enet_tx_bd_struct_t *txBdDirtyStatic; /*!< The dirty transmit buffer descriptor for error static update. */
    volatile enet_tx_bd_struct_t *txBdDirtyTime;   /*!< The dirty transmit buffer descriptor for time stamp update. */
    uint64_t msTimerSecond;                        /*!< The second for Master PTP timer .*/
    enet_ptp_time_data_ring_t rxPtpTsDataRing;     /*!< Receive PTP 1588 time stamp data ring buffer. */
    enet_ptp_time_data_ring_t txPtpTsDataRing;     /*!< Transmit PTP 1588 time stamp data ring buffer. */
#if FSL_FEATURE_ENET_PTP_TIMER_CHANNEL_INTERRUPT_ERRATA_2579
    enet_ptp_timer_channel_t channel; /*!< Only for ERRATA_2579: Set the PTP 1588 timer channel for time interrupt. */
#endif                                /* FSL_FEATURE_ENET_PTP_TIMER_CHANNEL_INTERRUPT_ERRATA_2579*/
#endif                                /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
} enet_handle_t;

/*!
 * @brief ENET callback function type
 *
 * The callback function is defined by user if user need to be informed for the interrupt event and
 * it is refer to #ENET_InstallIRQCallback
 */
typedef void (*enet_callback_t)(enet_handle_t *handle, enet_event_t event, void *param);

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
  * @name ENET Initialization and De-initialization
  * @{
  */

/*!
 * @brief Gets the ENET MAC default configuration structure. The
 * purpose of this API is to get the default configure structure for ENET_Init().
 * User may use the initialized structure unchanged in ENET_Init(), or modify
 * some fields of the structure before calling ENET_Init().
 * Example:
   @code
   enet_config_t config;
   ENET_GetDefaultConfig(&config);
   @endcode
 * @param eMacConfig The ENET Mac configuration structure pointer.
 */
void ENET_GetDefaultConfig(enet_config_t *eMacConfig);

/*!
 * @brief Initializes the ENET module.
 * This function ungates the module clock, initialize with the MAC configuration.
 * IF you want to use enhanced PTP1588 mode or "ENET_ENHANCEDBUFFERDESCRIPTOR_MODE"is
 * defined, please make sure the PTP1588 configure APIs are used.
 * @param handle  ENET handler pointer.
 * @param eMacConfig  ENET mac configuration structure pointer.
 *        The "enet_config_t" type mac configuration return from ENET_InitMacConfig
 *        can be directly used. or you can verify the mac configuration as you wish.
 * @param eBufferConfig  ENET buffer configuration structure pointer.
 *        The buffer configuration should be prepared for ENET Initialization.
 * @param macAddr  ENET mac address of Ethernet device. This MAC address should be
 *        provided.
 * @param clockSrc The internal module clock source for MII clock.
 */
void ENET_Init(enet_handle_t *handle,
               const enet_config_t *eMacConfig,
               const enet_buffer_config_t *eBufferConfig,
               uint8_t *macAddr,
               uint32_t clockSrc);
/*!
 * @brief DeInitializes the ENET module, gates the module clock.
 *
 * @param base  ENET peripheral base address.
 */
void ENET_DeInit(ENET_Type *base);

/*!
 * @brief Creates the handler pointer for data transfer.
 * This should be called before calling ENET_Init.
 * @param handle ENET handler pointer. Should be provided by application.
 * @param base   ENET peripheral base address.
 */
void ENET_CreateHandle(enet_handle_t *handle, ENET_Type *base);

/*!
 * @brief Reset the ENET module. This function restores the ENET module
 * to reset state, notice that this function will set all the registers to
 * reset state so the ENET module can not work after calling this function.
  *
 * @param base  ENET peripheral base address.
 */
static inline void ENET_Reset(ENET_Type *base)
{
    base->ECR |= ENET_ECR_RESET_MASK;
}

/* @} */

/*!
 * @name ENET MII interface operation
 * @{
 */

/*!
 * @brief Sets the ENET MII speed and duplex.
 *
 * @param base  ENET peripheral base address.
 * @param speed The speed of the RMII mode.
 * @param duplex The duplex of the RMII mode.
 */
void ENET_SetMII(ENET_Type *base, enet_mii_speed_t speed, enet_mii_duplex_t duplex);

/*!
 * @brief Sets the ENET SMI(serial management interface)- MII management interface.
 *
 * This API is used to set the ENET SMI interface.
 *
 * @param base  ENET peripheral base address.
 * @param sysClk This is the internal module clock frequency for SMI.
 * @param isPreambleDisabled The preamble disable flag.
 *        - true   Enables the preamble.
 *        - false  Disables the preamble.
 */
void ENET_SetSMI(ENET_Type *base, uint32_t sysClk, bool isPreambleDisabled);

/*!
 * @brief Gets the ENET SMI- MII management interface configure.
 *
 * This API is used to get the SMI configuration to check if the MII management
 * interface has been set.
 *
 * @param base  ENET peripheral base address.
 * @return The SMI setup status.
 *        - true   SMI has been setup.
 *        - false  SMI has not been setup.
 */
static inline bool ENET_GetSMI(ENET_Type *base)
{
    return (0 != (base->MSCR & 0x7E));
}

/*!
 * @brief Reads data from PHY through SMI interface.
 *
 * @param base  ENET peripheral base address.
 * @return The data read from PHY
 */
static inline uint32_t ENET_ReadSMIData(ENET_Type *base)
{
    return (uint32_t)((base->MMFR & ENET_MMFR_DATA_MASK) >> ENET_MMFR_DATA_SHIFT);
}

/*!
 * @brief Starts a SMI(serial Management interface) read command.
 *
 * @param base  ENET peripheral base address.
 * @param phyAddr The PHY address.
 * @param phyReg The PHY register.
 * @param operation The read operation.
 */
void ENET_StartSMIRead(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, enet_mii_read_t operation);

/*!
 * @brief Starts a SMI(serial Management interface) write command.
 *
 * @param base  ENET peripheral base address.
 * @param phyAddr The PHY address.
 * @param phyReg The PHY register.
 * @param operation The write operation.
 * @param data The data written to PHY.
 */
void ENET_StartSMIWrite(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, enet_mii_write_t operation, uint32_t data);

/* @} */

/*!
 * @name ENET basic operation
 * @{
 */

/*!
 * @brief Actives the ENET read or receive.
 * @Note This should be called after the mac configuration and
 * state are prepared.
 * @param base  ENET peripheral base address.
 */
static inline void ENET_ActiveRead(ENET_Type *base)
{
    base->RDAR = ENET_RDAR_RDAR_MASK;
}

/*!
 * @brief Sets the ENET module Mac Address.
 *
 * @param base  ENET peripheral base address.
 * @param macAddr The six-byte Mac address pointer.
 *        The pointer is allocated by application and input into the API.
 */
void ENET_SetMacAddr(ENET_Type *base, uint8_t *macAddr);

/*!
 * @brief Gets the ENET module Mac Address.
 *
 * @param base  ENET peripheral base address.
 * @param macAddr The six-byte Mac address pointer.
 *        The pointer is allocated by application and input into the API.
 */
void ENET_GetMacAddr(ENET_Type *base, uint8_t *macAddr);

/*!
 * @brief Enables the ENET interrupt.
 *
 * @param base  ENET peripheral base address.
 * @param mask  ENET interrupts to enable. This is a logical OR of the
 *             enumeration :: enet_interrupt_enable_t.
 */
static inline void ENET_EnableInterrupts(ENET_Type *base, uint32_t mask)
{
    base->EIMR |= mask;
}

/*!
 * @brief Disables the ENET interrupt.
 *
 * @param base  ENET peripheral base address.
 * @param mask  ENET interrupts to disable. This is a logical OR of the
 *             enumeration :: enet_interrupt_enable_t.
 */
static inline void ENET_DisableInterrupts(ENET_Type *base, uint32_t mask)
{
    base->EIMR &= ~mask;
}

/*!
 * @brief Gets the ENET interrupt status flag.
 *
 * @param base  ENET peripheral base address.
 * @return The event status of the interrupt source. This is the logical OR of members
 *         of the enumeration :: enet_interrupt_enable_t.
 */
static inline uint32_t ENET_GetInterruptStatus(ENET_Type *base)
{
    return base->EIR;
}

/*!
 * @brief Clears ENET interrupt events status flag.
 *
 * @param base  ENET peripheral base address.
 * @param mask  ENET interrupt source to be cleared.
 * This is the logical OR of members of the enumeration :: enet_interrupt_enable_t.
 */
static inline void ENET_ClearInterruptStatus(ENET_Type *base, uint32_t mask)
{
    base->EIR = mask;
}

/*!
 * @brief Enables/disables the MAC enter sleep mode.
 * This function is used to set the MAC enter the sleep mode.
 * When enter sleep mode, the magic frame wakeup interrupt should be enabled
 * to wakeup the MAC from the sleep mode and reset the MAC into normal mode.
 *
 * @param base    ENET peripheral base address.
 * @param enable  True enable sleep mode, false disable sleep mode.
 */
static inline void ENET_EnableSleepMode(ENET_Type *base, bool enable)
{
    if (enable)
    {
        /* When this field is set, MAC enters sleep mode. */
        base->ECR |= ENET_ECR_SLEEP_MASK | ENET_ECR_MAGICEN_MASK;
    }
    else
    { /* Mac exit sleep mode. */
        base->ECR &= ~(ENET_ECR_SLEEP_MASK | ENET_ECR_MAGICEN_MASK);
    }
}

/*!
 * @brief Adds the ENET device to a multicast group.
 *
 * @param base    ENET peripheral base address.
 * @param address The six-byte multicast group address which is provided by application.
 */
void ENET_AddMulticastGroup(ENET_Type *base, uint8_t *address);

/*!
 * @brief Moves the ENET device from a multicast group.
 *
 * @param base  ENET peripheral base address.
 * @param address The six-byte multicast group address which is provided by application.
 */
void ENET_LeaveMulticastGroup(ENET_Type *base, uint8_t *address);

/*!
 * @brief Gets ENET transmit and receive accelerator functions from MAC controller.
 *
 * @param base  ENET peripheral base address.
 * @param txAccelOption The transmit accelerator option. The "enet_tx_accelerator_t" is
 *         recommended to be used to as the mask to get the exact the accelerator option.
 * @param rxAccelOption The receive accelerator option. The "enet_rx_accelerator_t" is
 *         recommended to be used to as the mask to get the exact the accelerator option.
 */
static inline void ENET_GetAccelFunction(ENET_Type *base, uint32_t *txAccelOption, uint32_t *rxAccelOption)
{
    assert(txAccelOption);
    assert(txAccelOption);

    *txAccelOption = base->TACC;
    *rxAccelOption = base->RACC;
}
/* @} */

/*!
 * @name ENET transactional operation
 * @{
 */

/*!
 * @brief Gets the ENET the error statistics of a received frame.
 * This interface gets the error statistics of a received error frame.
 * Because the error information should be called before the ENET_ReadFrame().
 * ENET_GetRxFrameSize is recommended to be called first. If ENET_GetRxFrameSize
 * returns kStatus_ENET_RxFrameError then ENET_GetRxErrBeforeReadFrame
 * can be used to get the exact error statistics.
 *
 * @param handle The ENET handler structure pointer. This is the same handler pointer used in the ENET_Init.
 * @param eErrorStatic The error statistics structure pointer.
 */
void ENET_GetRxErrBeforeReadFrame(enet_handle_t *handle, enet_data_error_stats_t *eErrorStatic);

#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
/*!
 * @brief Gets the ENET transmit frame statistics after the data send.
 * This interface gets the error statistics of the transmit frame.
 * Because the error information is report by the uDMA after the data delivery, this interface
 * should be called after the data transmit API. After calling the ENET_SendFrame, the
 * transmit interrupt will notify the transmit completion.
 *
 * @param handle The PTP handler pointer. This is the same handler pointer used in the ENET_Init.
 * @param eErrorStatic The error statistics structure pointer.
 * @param return The execute status.
 */
status_t ENET_GetTxErrAfterSendFrame(enet_handle_t *handle, enet_data_error_stats_t *eErrorStatic);
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
       /*!
       * @brief Gets the size of the read frame.
       * This function reads a received frame size from the ENET buffer descriptors.
       * @note The FCS of the frame is removed by MAC controller, so the size is the length without the FCS.
       *
       * @param handle The ENET handler structure. This is the same handler pointer used in the ENET_Init.
       * @param length The length of the valid frame received.
       * @return The execute status.
       *        "kStatus_ENET_RxFrameEmpty" - no frame received. Should not ENET_ReadFrame to read frame.
       *        "kStatus_ENET_RxFrameError" - error happens ENET_ReadFrame should be called with NULL data .
       *         and NULL length to update the buffers.
       *        "kStatus_ENET_Success", - a valid frame is received then the ENET_ReadFrame
       *         should be called with the right data buffer and the captured data length input.
       */
status_t ENET_GetRxFrameSize(enet_handle_t *handle, uint32_t *length);

/*!
 * @brief Reads a frame from the ENET device.
 * This function reads a frame (both the data and the length) from the ENET buffer descriptors.
 * The ENET_GetRxFrameSize should be used to get the size of the prepared data buffer.
 * @note The FCS of the frame is removed by MAC controller and will not be delivered to application.
 *
 * @param handle The ENET handler structure. This is the same handler pointer used in the ENET_Init.
 * @param data The data buffer provided by user to store the frame which memory size should be at least "length".
 * @param length The size of the data buffer which is still the length of the received frame.
 * @return The execute status.
 */
status_t ENET_ReadFrame(enet_handle_t *handle, uint8_t *data, uint32_t length);

/*!
 * @brief Transmits a ENET frame.
 * @note The crc will be automatically appended to the data, so just input the data
 * you want to send without crc.
 *
 * @param handle The ENET handler pointer. This is the same handler pointer used in the ENET_Init.
 * @param data The data buffer provided by user to be send.
 * @param length The length of the data to be send.
 * @return The execute status.
 */
status_t ENET_SendFrame(enet_handle_t *handle, uint8_t *data, uint32_t length);

/*!
 * @brief Install IRQ transfer callback
 *
 * This function install IRQ transfer callback.
 *
 * @param handle ENET handler structure.
 * @param callback ENET callback.
 * @param param callback function parameter.
 */
void ENET_SetIRQCallback(enet_handle_t *handle, enet_callback_t callback, void *param);

/*!
 * @brief The transmit IRQ handler.
 *
 * @param handle The ENET handler pointer.
 */
void ENET_TransmitIRQHandler(enet_handle_t *handle);

/*!
 * @brief The receive IRQ handler.
 *
 * @param handle The ENET handler pointer.
 */
void ENET_ReceiveIRQHandler(enet_handle_t *handle);

/*!
 * @brief The error IRQ handler.
 *
 * @param handle The ENET handler pointer.
 */
void ENET_ErrorIRQHandler(enet_handle_t *handle);

/* @} */

#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
/*!
 * @name ENET PTP 1588 function operation
 * @{
 */

/*!
 * @brief Configure the ENET PTP 1588 feature with the basic configuration.
 * The function set the clock for PTP 1588 timer and enables
 * time stamp interrupts and transmit interrupts for PTP 1588 features.
 * ENET_CreateHandle and ENET_Init should be called before calling this API.
 * @param handle  ENET handler pointer.
 * @param ptpConfig The ENET PTP1588 configuration.
 */
void ENET_Ptp1588Configure(enet_handle_t *handle, enet_ptp_config_t *ptpConfig);

/*!
 * @brief Starts the ENET PTP 1588 Timer.
 * This function is used to initialize the PTP timer. After the PTP starts,
 * the PTP timer starts running.
 *
 * @param base  ENET peripheral base address.
 * @param ptpClkSrc The clock source of the PTP timer.
 * @param channel The PTP 1588 channel only used for the ERRATA macro
 *        "FSL_FEATURE_ENET_PTP_TIMER_CHANNEL_INTERRUPT_ERRATA_2579" enabled case.
 */
void ENET_Ptp1588StartTimer(ENET_Type *base, uint32_t ptpClkSrc, enet_ptp_timer_channel_t channel);

/*!
 * @brief Stops the ENET PTP 1588 Timer.
 * This function is used to stops the ENET PTP timer.
 *
 * @param base  ENET peripheral base address.
 */
static inline void ENET_Ptp1588StopTimer(ENET_Type *base)
{
    /* Disable PTP timer and reset the timer. */
    base->ATCR &= ~ENET_ATCR_EN_MASK;
    base->ATCR |= ENET_ATCR_RESTART_MASK;
}

/*!
 * @brief Adjusts the ENET PTP 1588 timer.
 *
 * @param base  ENET peripheral base address.
 * @param corrIncrease The correction increment value. This value is added every time the correction
 *       timer expires. a value less than the PTP timer frequency(1/ptpClkSrc) slows down the timer,
 *        a value greater than the 1/ptpClkSrc speeds up the timer.
 * @param corrPeriod The PTP timer correction counter wrap-around value. This defines after how
 *       many timer clock the correction counter should be reset and trigger a correction
 *       increment on the timer. A value of 0 disables the correction counter and no correction occur.
 */
void ENET_Ptp1588AdjustTimer(ENET_Type *base, uint32_t corrIncrease, uint32_t corrPeriod);

/*!
 * @brief Sets ENET PTP 1588 timer channel mode.
 *
 * @param base  ENET peripheral base address.
 * @param channel The ENET PTP timer channel number.
 * @param mode The PTP timer channel mode, see "enet_ptp_timer_channel_mode_t".
 * @param intEnable Enables or disables the interrupt.
 */
static inline void ENET_Ptp1588SetChannelMode(ENET_Type *base,
                                              enet_ptp_timer_channel_t channel,
                                              enet_ptp_timer_channel_mode_t mode,
                                              bool intEnable)
{
    uint32_t tcrReg = 0;

    tcrReg = ENET_TCSR_TMODE(mode) | ENET_TCSR_TIE(intEnable);
    /* Disable channel mode first. */
    base->CHANNEL[channel].TCSR = 0;
    base->CHANNEL[channel].TCSR = tcrReg;
}

/*!
 * @brief Sets ENET PTP 1588 timer channel compare value.
 *
 * @param base  ENET peripheral base address.
 * @param channel The PTP timer channel, see "enet_ptp_timer_channel_t".
 * @param cmpValue The compare value for the compare setting.
 */
static inline void ENET_Ptp1588SetChannelCmpValue(ENET_Type *base, enet_ptp_timer_channel_t channel, uint32_t cmpValue)
{
    base->CHANNEL[channel].TCCR = cmpValue;
}

/*!
 * @brief Gets the ENET PTP 1588 timer channel status.
 *
 * @param base  ENET peripheral base address.
 * @param channel The IEEE 1588 timer channel number.
 * @return Compare or capture operation status
 *         - true if the compare or capture has occurred.
 *         - False if the compare or capture has not occurred.
 */
static inline bool ENET_Ptp1588GetChannelStatus(ENET_Type *base, enet_ptp_timer_channel_t channel)
{
    return (0 != (base->CHANNEL[channel].TCSR & ENET_TCSR_TF_MASK));
}

/*!
 * @brief Clears the ENET PTP 1588 timer channel status.
 *
 * @param base  ENET peripheral base address.
 * @param channel The IEEE 1588 timer channel number.
 */
static inline void ENET_Ptp1588ClearChannelStatus(ENET_Type *base, enet_ptp_timer_channel_t channel)
{
    base->CHANNEL[channel].TCSR |= ENET_TCSR_TF_MASK;
    base->TGSR |= (1U << channel);
}

/*!
 * @brief Gets the current ENET time from the PTP 1588 timer.
 *
 * @param handle The ENET state pointer. This is the same state pointer used in the ENET_Init.
 * @param ptpTime The PTP timer structure.
 */
void ENET_Ptp1588GetTimer(enet_handle_t *handle, enet_ptp_time_t *ptpTime);

/*!
 * @brief Sets the ENET PTP 1588 timer to the assigned time.
 *
 * @param handle The ENET state pointer. This is the same state pointer used in the ENET_Init.
 * @param ptpTime The timer to be set to the PTP timer.
 */
void ENET_Ptp1588SetTimer(enet_handle_t *handle, enet_ptp_time_t *ptpTime);

/*!
 * @brief The IEEE 1588 PTP time stamp interrupt handler.
 *
 * @param handle The ENET state pointer. This is the same state pointer used in the ENET_Init.
 */
void ENET_Ptp1588TimerIRQHandler(enet_handle_t *handle);

/*!
 * @brief Gets the time stamp of the received frame.
 *
 * @param handle The ENET handler pointer.This is the same state pointer used in
 *        ENET_Init.
 * @param ptpTimeData The special PTP timestamp data for search the receive timestamp.
 * @return The execution status.
 */
status_t ENET_GetRxFrameTime(enet_handle_t *handle, enet_ptp_time_data_t *ptpTimeData);

/*!
 * @brief Gets the time stamp of the transmit frame.This should be called after the
 * transmit completion. And after calling the ENET_SendFrame.
 *
 * @param handle The ENET handler pointer.This is the same state pointer used in
 *        ENET_Init.
 * @param ptpTimeData The special PTP timestamp data for search the receive timestamp.
 * @return The execution status.
 */
status_t ENET_GetTxFrameTime(enet_handle_t *handle, enet_ptp_time_data_t *ptpTimeData);
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_ENET_H_ */
