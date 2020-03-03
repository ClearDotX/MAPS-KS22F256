/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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
#if !defined(__FSL_LPI2C_HAL_H__)
#define __FSL_LPI2C_HAL_H__

#include <assert.h>
#include <stdbool.h>
#include "fsl_device_registers.h"

/*!
 * @addtogroup lpi2c_hal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief LPI2C status return codes.*/
typedef enum _lpi2c_status {
    kStatus_LPI2C_Success            = 0x0U, /*!< LPI2C operation has no error. */
    kStatus_LPI2C_Initialized        = 0x1U, /*!< Current LPI2C is already initialized by one task.*/
    kStatus_LPI2C_Fail               = 0x2U, /*!< LPI2C operation failed. */
    kStatus_LPI2C_Busy               = 0x3U, /*!< The master is already performing a transfer.*/
    kStatus_LPI2C_Timeout            = 0x4U, /*!< The transfer timed out.*/
    kStatus_LPI2C_ReceivedNak        = 0x5U, /*!< The slave device sent a NAK in response to a byte.*/
    kStatus_LPI2C_Error              = 0x6U, /*!< LPI2C bit error or FIFO error.*/
    kStatus_LPI2C_AribtrationLost    = 0x8U, /*!< LPI2C Arbitration Lost error.*/
    kStatus_LPI2C_StopSignalFail     = 0x9U, /*!< LPI2C STOP signal could not release bus. */
    kStatus_LPI2C_Idle               = 0xAU, /*!< LPI2C Slave Bus is Idle. */
    kStatus_LPI2C_NoReceiveInProgress= 0xBU, /*!< Attempt to abort a receiving when no transfer
                                                was in progress */
    kStatus_LPI2C_NoSendInProgress   = 0xCU, /*!< Attempt to abort a sending when no transfer
                                                was in progress */
    kStatus_LPI2C_DmaRequestFail     = 0xDU  /*!< DMA request fail */
} lpi2c_status_t;

/*! @brief LPI2C master status. */
typedef enum _lpi2c_master_status {
    kLpi2cMasterTxRequestFlag       = LPI2C_MSR_TDF_SHIFT,  /*!< Transmit data flag*/  
    kLpi2cMasterRxReadyFlag         = LPI2C_MSR_RDF_SHIFT,  /*!< Receive data flag*/   
    kLpi2cMasterEndOfPacketFlag     = LPI2C_MSR_EPF_SHIFT,  /*!< End Packet flag*/ 
    kLpi2cMasterStopDetectFlag      = LPI2C_MSR_SDF_SHIFT,  /*!< Stop detect flag*/ 
    kLpi2cMasterNackDetectFlag      = LPI2C_MSR_NDF_SHIFT,  /*!< NACK detect flag*/ 
    kLpi2cMasterArbitrationLostFlag = LPI2C_MSR_ALF_SHIFT,  /*!< Arbitration lost flag*/ 
    kLpi2cMasterFifoErrFlag         = LPI2C_MSR_FEF_SHIFT,  /*!< FIFO error flag*/ 
    kLpi2cMasterPinLowTimeoutFlag   = LPI2C_MSR_PLTF_SHIFT, /*!< Pin low timeout flag*/ 
    kLpi2cMasterDataMatchFlag       = LPI2C_MSR_DMF_SHIFT,  /*!< Data match flag*/ 
    kLpi2cMasterBusyFlag            = LPI2C_MSR_MBF_SHIFT,  /*!< Master busy flag. */
    kLpi2cMasterBusBusyFlag         = LPI2C_MSR_BBF_SHIFT   /*!< Bus busy flag. */
} lpi2c_master_status_t;

/*! @brief LPI2C master interrupts. */
typedef enum _lpi2c_master_interrupt {
    kLpi2cMasterTxRequestInt       = LPI2C_MIER_TDIE_MASK,  /*!< Transmit data interrupt enable*/
    kLpi2cMasterRxReadyInt         = LPI2C_MIER_RDIE_MASK,  /*!< Receive data interrupt enable*/
    kLpi2cMasterEndOfPacketInt     = LPI2C_MIER_EPIE_MASK,  /*!< End packet interrupt enable*/
    kLpi2cMasterStopDetectInt      = LPI2C_MIER_SDIE_MASK,  /*!< Stop detect interrupt enable*/
    kLpi2cMasterNackDetectInt      = LPI2C_MIER_NDIE_MASK,  /*!< NACK detect interrupt enable*/
    kLpi2cMasterArbitrationLostInt = LPI2C_MIER_ALIE_MASK,  /*!< Arbitration lost interrupt enable*/
    kLpi2cMasterFifoErrInt         = LPI2C_MIER_FEIE_MASK,  /*!< FIFO error interrupt enable*/
    kLpi2cMasterPinLowTimeoutInt   = LPI2C_MIER_PLTIE_MASK, /*!< Pin low timeout interrupt enable*/
    kLpi2cMasterDataMatchInt       = LPI2C_MIER_DMIE_MASK   /*!< Data match interrupt enable*/
} lpi2c_master_interrupt_t;
/*! @brief LPI2C master modes. */
typedef enum _lpi2c_mode{
    kLpi2cMasterStandardMode      = 0x0U,  /*!< Standard-mode (Sm), with a bit rate up to 100 kbit/s*/
    kLpi2cMasterFastMode          = 0X1U,  /*!< Fast-mode (Fm), with a bit rate up to 400 kbit/s*/
    kLpi2cMasterFastPlusMode      = 0x2U,  /*!< Fast-mode Plus (Fm+), with a bit rate up to 1 Mbit/s*/ 
    kLpi2cMasterUltraFastMode     = 0x3U,  /*!< Ultra Fast-mode (UFm), with a bit rate up to 5 Mbit/s*/
    kLpi2cMasterHighSpeedMode     = 0x4U   /*!< High-speed mode (Hs-mode), with a bit rate up to 3.4 Mbit/s*/
} lpi2c_mode_t;
/*! @brief LPI2C master FIFO commands. */
typedef enum _lpi2c_master_fifo_cmd{
    kLpi2cMasterCmdTxData         = 0x0U,  /*!< Transmit DATA[7:0]*/
    kLpi2cMasterCmdRxData         = 0X1U,  /*!< Receive (DATA[7:0] + 1) bytes*/
    kLpi2cMasterCmdStop           = 0x2U,  /*!< Generate STOP condition*/
    kLpi2cMasterCmdRxDiscard      = 0x3U,  /*!< Receive and discard (DATA[7:0] + 1) bytes*/
    kLpi2cMasterCmdStart          = 0x4U,  /*!< Generate(repeated) START and transmit address in DATA[[7:0]*/
    kLpi2cMasterCmdStartNack      = 0x5U,  /*!< Generate(repeated) START and transmit address in DATA[[7:0], expect NACK to be returned*/
    kLpi2cMasterCmdHSStart        = 0x6U,  /*!< Generate(repeated) START and transmit address in DATA[[7:0] using high speed mode*/
    kLpi2cMasterCmdHSStartNack    = 0x7U   /*!< Generate(repeated) START and transmit address in DATA[[7:0] using high speed mode, expect NACK to be returned*/
} lpi2c_master_fifo_cmd_t;
typedef enum lpi2c_master_watermark {
    kLpi2cMasterWaterWord0        =0x0U,  /*!< Set Watermark 0 word*/
    kLpi2cMasterWaterWord1        =0x1U,  /*!< Set Watermark 1 word*/
    kLpi2cMasterWaterWord2        =0x2U,  /*!< Set Watermark 2 words*/ 
    kLpi2cMasterWaterWord3        =0x3U   /*!< Set Watermark 3 words*/ 
} lpi2c_master_watermark_t;
/*! @brief LPI2C slave status. */
typedef enum _lpi2c_slave_status {
    kLpi2cSlaveTxRequestFlag       = LPI2C_SSR_TDF_SHIFT,  /*!< Transmit data flag*/
    kLpi2cSlaveRxReadyFlag         = LPI2C_SSR_RDF_SHIFT,  /*!< Receive data flag*/
    kLpi2cSlaveAddressValidFlag    = LPI2C_SSR_AVF_SHIFT,  /*!< Address valid flag*/
    kLpi2cSlaveTransmitAckFlag     = LPI2C_SSR_TAF_SHIFT,  /*!< Transmit ACK flag*/
    kLpi2cSlaveRepeatStartFlag     = LPI2C_SSR_RSF_SHIFT,  /*!< Repeated start flag*/
    kLpi2cSlaveStopDetectFlag      = LPI2C_SSR_SDF_SHIFT,  /*!< Stop detect flag*/
    kLpi2cSlaveBitErrFlag          = LPI2C_SSR_BEF_SHIFT,  /*!< Bit error flag*/
    kLpi2cSlaveFifoErrFlag         = LPI2C_SSR_FEF_SHIFT,  /*!< FIFO error flag*/
    kLpi2cSlaveAddress0MatachFlag  = LPI2C_SSR_AM0F_SHIFT, /*!< Address match 0 flag*/
    kLpi2cSlaveAddress1MatachFlag  = LPI2C_SSR_AM1F_SHIFT, /*!< Address match 1 flag*/
    kLpi2cSlaveGeneralCallFlag     = LPI2C_SSR_GCF_SHIFT,  /*!< General call flag*/
    kLpi2cSlaveSMBusAlertFlag      = LPI2C_SSR_SARF_SHIFT, /*!< SMBus alert response flag*/
    kLpi2cSlaveBusyFlag            = LPI2C_SSR_SBF_SHIFT,  /*!< Slave busy flag*/
    kLpi2cSlaveBusBusyFlag         = LPI2C_SSR_BBF_SHIFT   /*!< Bus busy flag*/
} lpi2c_slave_status_t;

/*! @brief LPI2C slave interrupts. */
typedef enum _lpi2c_slave_interrupt {
    kLpi2cSlaveTxRequestInt       = LPI2C_SIER_TDIE_MASK,  /*!< Transmit data interrupt enable*/ 
    kLpi2cSlaveRxReadyInt         = LPI2C_SIER_RDIE_MASK,  /*!< Receive data interrupt enable*/ 
    kLpi2cSlaveAddressValidInt    = LPI2C_SIER_AVIE_MASK,  /*!< Address valid interrupt enable*/ 
    kLpi2cSlaveTransmitAckInt     = LPI2C_SIER_TAIE_MASK,  /*!< Transmit ACK interrupt enable*/ 
    kLpi2cSlaveRepeatStartInt     = LPI2C_SIER_RSIE_MASK,  /*!< Repeated Start interrupt enable*/ 
    kLpi2cSlaveStopDetectInt      = LPI2C_SIER_SDIE_MASK,  /*!< Stop detect interrupt enable*/ 
    kLpi2cSlaveBitErrInt          = LPI2C_SIER_BEIE_MASK,  /*!< Bit error interrupt enable*/ 
    kLpi2cSlaveFifoErrInt         = LPI2C_SIER_FEIE_MASK,  /*!< FIFO error interrupt enable*/ 
    kLpi2cSlaveAddress0MatachInt  = LPI2C_SIER_AM0IE_MASK, /*!< Address match 0 interrupt enable*/ 
    kLpi2cSlaveAddress1MatachInt  = LPI2C_SIER_AM1F_MASK,  /*!< Address match 1 interrupt enable*/ 
    kLpi2cSlaveGeneralCallInt     = LPI2C_SIER_GCIE_MASK,  /*!< General call interrupt enable*/ 
    kLpi2cSlaveSMBusAlertInt      = LPI2C_SIER_SARIE_MASK  /*!< SMBus alert response interrupt enable*/ 
} lpi2c_slave_interrupt_t;
/*! @brief Direction of master and slave transfers.*/
typedef enum _lpi2c_direction {
    kLPI2CWrite = 0U,  /*!< Master transmit.*/
    kLPI2CRead  = 1U   /*!< Master receive.*/
} lpi2c_direction_t;
/*!@brief LPI2C master match configuration.*/
typedef enum _lpi2c_master_match_config {
    kLpi2cMatchDisabled                   = 0x0U,  /*!<LPI2C Match Disabled*/
    kLpi2c1stWrdEqualsM0orM1              = 0x2U,  /*!<LPI2C Match Enabled and 1st data word equals MATCH0 OR MATCH1*/
    kLpi2cAnyWrdEqualsM0orM1              = 0x3U,  /*!<LPI2C Match Enabled and any data word equals MATCH0 OR MATCH1*/
    kLpi2c1stWrdEqualsM0and2ndWrdEqualsM1 = 0x4U,  /*!<LPI2C Match Enabled and 1st data word equals MATCH0, 2nd data equals MATCH1*/
    kLpi2cAnyWrdEqualsM0andNxtWrdEqualsM1 = 0x5U,  /*!<LPI2C Match Enabled and any data word equals MATCH0, next data equals MATCH1*/
    kLpi2c1stWrdAndM1EqualsM0andM1        = 0x6U,  /*!<LPI2C Match Enabled and 1st data word and MATCH0 equals MATCH0 and MATCH1*/
    kLpi2cAnyWrdAndM1EqualsM0andM1        = 0x7U   /*!<LPI2C Match Enabled and any data word and MATCH0 equals MATCH0 and MATCH1*/
}lpi2c_master_match_config_t;
/*!@brief LPI2C pin configuration.*/
typedef enum _lpi2c_master_pin_config {
    kLpi2c2PinOpenDrain                   = 0x0U,  /*!<LPI2C Configured for 2-pin open drain mode*/
    kLpi2c2PinOutputOnly                  = 0x1U,  /*!<LPI2C Configured for 2-pin output only mode(ultra-fast mode)*/
    kLpi2c2PinPushPull                    = 0x2U,  /*!<LPI2C Configured for 2-pin push-pull mode*/
    kLpi2c4PinPushPull                    = 0x3U,  /*!<LPI2C Configured for 4-pin push-pull mode*/
    kLpi2c2PinOpenDrainwithSeparateSlave  = 0x4U,  /*!<LPI2C Configured for 2-pin open drain mode with separate LPI2C slave*/
    kLpi2c2PinOutputOnlywithSeparateSlave = 0x5U,  /*!<LPI2C Configured for 2-pin output only mode(ultra-fast mode) with separate LPI2C slave*/
    kLpi2c2PinPushPullwithSeparateSlave   = 0x6U,  /*!<LPI2C Configured for 2-pin push-pull mode with separate LPI2C slave*/
    kLpi2c4PinPushPullwithInvertedOutput  = 0x7U   /*!<LPI2C Configured for 4-pin push-pull mode(inverted outputs)*/
}lpi2c_master_pin_config_t;
/*!@brief LPI2C master host request selection.*/
typedef enum _lpi2c_master_hostreq_sel {
    kLpi2cHostReqExtPin                   = 0x0U,   /*!<Select the LPI2C_HREQ pin as the host request input*/
    kLpi2cHostReqInputTrigger             = 0x1U,   /*!<Select the input trigger as the host request input*/
}lpi2c_master_hostreq_sel_t;
/*!@brief LPI2C master host request pin polarity configuration.*/
typedef enum _lpi2c_master_hostreq_config {
    kLpi2cHostReqPinActiveLow             = 0x0U,   /*!<Configure the LPI2C_HREQ pin active low*/
    kLpi2cHostReqPinActiveHigh            = 0x1U    /*!<Configure the LPI2C_HREQ pin active high*/
}lpi2c_master_hostreq_config_t;
/*!@brief LPI2C slave transmit flag assert type.*/
typedef enum _lpi2c_slave_txflag_assert {
    kLpi2cSlaveTDFAssertTransmit          = 0x0U,  /*!<TDF assert during slave transmit when STDR is empty*/
    kLpi2cSlaveTDFAssertEmpty             = 0x1U   /*!<TDF assert whenever STDR is empty*/
}lpi2c_slave_txflag_assert_t;
/*!@brief LPI2C slave clock stretching type.*/
typedef enum _lpi2c_slave_clkstall_type {
    kLpi2cAddressSclStall                 = LPI2C_SCFGR1_ADRSTALL_MASK,  /*!<Address SCL Stall*/
    kLpi2cReceiveSclStall                 = LPI2C_SCFGR1_RXSTALL_MASK,   /*!<receive SCL Stall*/
    kLpi2cTransmitSclStall                = LPI2C_SCFGR1_TXDSTALL_MASK,  /*!<transmit Data SCL Stall*/
    kLpi2cAckSclStall                     = LPI2C_SCFGR1_ACKSTALL_MASK   /*!<ACK SCL Stall*/
}lpi2c_slave_clkstall_type_t;
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*Define const value arrays for the baud rate pre-scaler values*/
static const uint32_t lpi2c_prescaler[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Module controls
 * @{
 */

/*!
 * @brief Restores the LPI2C master to reset state.
 *
 * @param base The LPI2C peripheral base pointer
 */
void LPI2C_HAL_MasterInit(LPI2C_Type * base);
/*!
 * @brief Restores the LPI2C slave to reset state.
 *
 * @param base The LPI2C peripheral base pointer
 */
void LPI2C_HAL_SlaveInit(LPI2C_Type * base);
/*!
 * @brief Enables/disables the LPI2C module as a master.
 *
 * @param base The LPI2C peripheral base pointer
 * @param enable Enable(true) or disable(false) current LPI2C as master.
 */
static inline void LPI2C_HAL_SetMasterCmd(LPI2C_Type * base, bool enable)
{
    LPI2C_BWR_MCR_MEN(base, enable);
}

/*!
 * @brief Enables/disables the LPI2C module as a slave.
 *
 * @param base The LPI2C peripheral base pointer
 * @param enable Enable(true) or disable(false) current LPI2C as slave.
 */
static inline void LPI2C_HAL_SetSlaveCmd(LPI2C_Type * base, bool enable)
{
    LPI2C_BWR_SCR_SEN(base, enable);
}
/*!
 * @brief Configures the LPI2C Master Host Request input.
 * 
 * @param base The LPI2C peripheral base pointer
 * @param hostReqInput Host request input source of type lpi2c_master_hostreq_sel_t
 * @param hostReqPol Host request polarity of type lpi2c_master_hostreq_config_t
 * @param enable Enable or disable the Host request feature
 */
void LPI2C_HAL_SetMasterHostRequestMode(LPI2C_Type *base,
                                        lpi2c_master_hostreq_sel_t hostReqInput,
                                        lpi2c_master_hostreq_config_t hostReqPol,
                                        bool enable);
/*!
 * @brief Configures the LPI2C Master Pin Mode.
 * 
 * @param base The LPI2C peripheral base pointer
 * @param pinConfig Pin mode of type lpi2c_master_pin_config_t
 * This function should be called when master is disabled.
*/
static inline void LPI2C_HAL_SetMasterPinConfigMode(LPI2C_Type *base,
                                       lpi2c_master_pin_config_t pinConfig)
{
    LPI2C_BWR_MCFGR1_PINCFG(base, pinConfig);
}
/*!
 * @brief Configures the LPI2C master to ignore the NACK which is assumed to be ACK.
 * 
 * @param base The LPI2C peripheral base pointer
 * @param enable Pass true to enable, false to disable.
 * This function should be called when master is disabled.
*/
static inline void LPI2C_HAL_SetMasterIgnoreNack(LPI2C_Type *base, bool enable)
{
    LPI2C_BWR_MCFGR1_IGNACK(base, enable ? 1U : 0U);
}
/*!
 * @brief Enables/disables auto stop generation when the transmit FIFO is empty.
 * 
 * @param base The LPI2C peripheral base pointer
 * @param enable Pass true to enable, false to disable.
 * This function should be called when master is disabled.
*/
static inline void LPI2C_HAL_SetMasterAutoStop(LPI2C_Type *base, bool enable)
{
    LPI2C_BWR_MCFGR1_AUTOSTOP(base, enable ? 1U : 0U);
}
/*!
 * @brief Configures the LPI2C master data match configuration mode.
 * 
 * @param base The LPI2C peripheral base pointer
 * @param matchMode Select condition of type lpi2c_master_match_config_t for the data 
 *        match
 * @param rxDataMatchOnly When enabled, all receive data that does not cause DMF to set 
 *        is discarded
 * @param match0 Setting for Match0 value
 * @param match1 Setting for Match1 value
 */
void LPI2C_HAL_SetMasterMatchConfigMode(LPI2C_Type *base,
                                        lpi2c_master_match_config_t matchMode,
                                        bool rxDataMatchOnly,
                                        uint32_t match0,
                                        uint32_t match1);

/*@}*/

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Gets the LPI2C status flag state.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param statusFlag The status flag, defined in type lpi2c_status_flag_t.
 * @return State of the status flag: asserted (true) or not-asserted (false).
 *         - true: related status flag is being set.
 *         - false: related status flag is not set.
 */
static inline bool LPI2C_HAL_MasterGetStatus(LPI2C_Type * base, lpi2c_master_status_t statusFlag)
{
    return (bool)((LPI2C_RD_MSR(base) >> statusFlag) & 0x1U);
}

/*!
 * @brief Clears the LPI2C status flag state.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param statusFlag The status flag, defined in type lpi2c_status_flag_t.
 */
static inline void LPI2C_HAL_MasterClearStatus(LPI2C_Type * base, lpi2c_master_status_t statusFlag)
{
    /* Flags could be cleared by write 1 except TDF/RDF/MBF/BBF. Write 0 will
     * have no effect. */
    LPI2C_WR_MSR(base, 0x1U << statusFlag);
}

/*!
 * @brief Gets the LPI2C status flag state.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param statusFlag The status flag, defined in type lpi2c_status_flag_t.
 * @return State of the status flag: asserted (true) or not-asserted (false).
 *         - true: related status flag is being set.
 *         - false: related status flag is not set.
 */
static inline bool LPI2C_HAL_SlaveGetStatus(LPI2C_Type * base, lpi2c_slave_status_t statusFlag)
{
    return (bool)((LPI2C_RD_SSR(base) >> statusFlag) & 0x1U);
}

/*!
 * @brief Clears the LPI2C status flag state.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param statusFlag The status flag, defined in type lpi2c_status_flag_t.
 */
static inline void LPI2C_HAL_SlaveClearStatus(LPI2C_Type * base, lpi2c_slave_status_t statusFlag)
{
    /* Flags could be cleared by write 1 except BBF/SBF/SARF/CGF/AM1F/AM0F/AVF/RDF/TDF. Write 0 will
     * have no effect. */
    LPI2C_WR_SSR(base, 0x1U << statusFlag);
}

/*@}*/

/*!
 * @name Interrupt
 * @{
 */

/*!
 * @brief Enables or disables specific LPI2C master interrupt.
 *
 * This function only enable/disable one specific interrupt. If you need to
 * enable/disable multiple interrupt in one function call, it's better to use
 * the entire register write operation. 
 *
 * @param base The LPI2C peripheral base pointer
 * @param interrupt Interrupt name. 
 * @param enable   Pass true to enable interrupt, false to disable.
 */
void LPI2C_HAL_MasterSetIntCmd(LPI2C_Type * base, uint32_t interrupt, bool enable);

/*!
 * @brief Returns whether the LPI2C master interrupt is enabled.
 *
 * @param base The LPI2C peripheral base pointer
 * @return Whether LPI2C master interrupts are enabled(true) or not(false).
 */
static inline bool LPI2C_HAL_MasterGetIntCmd(LPI2C_Type * base, lpi2c_master_interrupt_t interrupt)
{
    return (bool)(LPI2C_RD_MIER(base) & (uint8_t)(interrupt));
}

/*!
 * @brief Enables or disables a specific LPI2C slave interrupt.
 *
 * This function only enables/disables one specific interrupt. To
 * enable/disable multiple interrupts in one function call,  use
 * the entire register write operation. 
 *
 * @param base The LPI2C peripheral base pointer
 * @param interrupt Interrupt name. 
 * @param enable   Pass true to enable interrupt, false to disable.
 */
void LPI2C_HAL_SlaveSetIntCmd(LPI2C_Type * base, uint32_t interrupt, bool enable);

/*!
 * @brief Returns whether the LPI2C slave interrupt is enabled.
 *
 * @param base The LPI2C peripheral base pointer
 * @return Whether LPI2C slave interrupts are enabled.
 */
static inline bool LPI2C_HAL_SlaveGetIntCmd(LPI2C_Type * base, lpi2c_slave_interrupt_t interrupt)
{
    return (bool)(LPI2C_RD_SIER(base) & (uint8_t)(interrupt));
}
/*@}*/

/*!
 * @name DMA control
 * @{
 */
/*!
 * @brief Enables or disables the LPI2C master DMA transmit.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param enable   Pass true to enable interrupt, false to disable.
 */
static inline void LPI2C_HAL_MasterSetTxDmaCmd(LPI2C_Type * base, bool enable)
{
    LPI2C_BWR_MDER_TDDE(base, enable ? 1U : 0U);
}
/*!
 * @brief Enables or disables the LPI2C master DMA receive.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param enable   Pass true to enable interrupt, false to disable.
 */
static inline void LPI2C_HAL_MasterSetRxDmaCmd(LPI2C_Type * base, bool enable)
{
    LPI2C_BWR_MDER_RDDE(base, enable ? 1U : 0U);
}
/*!
 * @brief Gets the LPI2C master transmit data register address for the DMA transfer.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param return   The LPI2C Master Transmit Data Register address.
 */
static inline uint32_t LPI2C_HAL_MasterGetTxDataRegAddr(LPI2C_Type * base)
{
    return (uint32_t)(&LPI2C_MTDR_REG(base));
}
/*!
 * @brief Gets the LPI2C master receive data register address for the DMA transfer.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param return   The LPI2C Master Receive Data Register address.
 */
static inline uint32_t LPI2C_HAL_MasterGetRxDataRegAddr(LPI2C_Type * base)
{
    return (uint32_t)(&LPI2C_MRDR_REG(base));
}
/*!
 * @brief Enables or disables the LPI2C slave DMA transmit.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param enable   Pass true to enable interrupt, false to disable.
 */
static inline void LPI2C_HAL_SlaveSetTxDmaCmd(LPI2C_Type * base, bool enable)
{
    LPI2C_BWR_SDER_TDDE(base, enable ? 1U : 0U);
}
/*!
 * @brief Enables or disables the LPI2C slave DMA receive.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param enable   Pass true to enable interrupt, false to disable.
 */
static inline void LPI2C_HAL_SlaveSetRxDmaCmd(LPI2C_Type * base, bool enable)
{
    LPI2C_BWR_SDER_RDDE(base, enable ? 1U : 0U);
}
/*!
 * @brief Gets the LPI2C slave transmit data register address for the DMA transfer.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param return   The LPI2C Slave Transmit Data Register address.
 */
static inline uint32_t LPI2C_HAL_SlaveGetTxDataRegAddr(LPI2C_Type * base)
{
    return (uint32_t)(&LPI2C_STDR_REG(base));
}
/*!
 * @brief Gets the LPI2C slave receive data register address for the DMA transfer.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param return   The LPI2C Slave Receive Data Register address.
 */
static inline uint32_t LPI2C_HAL_SlaveGetRxDataRegAddr(LPI2C_Type * base)
{
    return (uint32_t)(&LPI2C_SRDR_REG(base));
}
/*@}*/

/*!
 * @name Baud rate
 * @{
 */

/*!
 * @brief Sets the LPI2C bus frequency for master transactions.
 *
 * @param base The LPI2C peripheral base pointer
 * @param sourceClockInHz LPI2C source input clock in Hertz
 * @param kbps Requested bus frequency in kilohertz. 
 * @param mode LPI2C operation mode.
 */
void LPI2C_HAL_SetBaudRate(LPI2C_Type * base,
                           uint32_t sourceClockInHz,
                           uint32_t kbps,
                           lpi2c_mode_t mode);
/*!
 * @brief Sets the glitch filter for slave transactions.
 *
 * This function should be called before the slave is enabled.
 * @param base The LPI2C peripheral base pointer
 * @param sourceClockInHz LPI2C source input clock in Hertz
 * @param glitchWidthInNs Width of glitches to filter in nanoseconds. 
 */
void LPI2C_HAL_SetGlitch(LPI2C_Type * base, 
                         uint32_t sourceClockInHz, 
                         uint32_t glitchWidthInNs);
/*@}*/

/*!
 * @name Bus operations
 * @{
 */

/*!
 * @brief Sends a start or a repeated start signal on the LPI2C bus.
 *
 * This function initiates a new master mode transfer by sending the start signal. It
 * is also sends a repeated start signal when a transfer is already in progress.
 *
 * @param base The LPI2C peripheral base pointer
 * @param address 7-bit slave device address.
 * @param dir Master transfer directions(transmit/receive).
 */
void LPI2C_HAL_SendStart(LPI2C_Type * base, uint8_t address, lpi2c_direction_t dir);

/*!
 * @brief Sends a stop signal on the LPI2C bus.
 *
 * @param base The LPI2C peripheral base pointer
 * @return Whether the sending of STOP single is success or not.
 */
lpi2c_status_t LPI2C_HAL_SendStop(LPI2C_Type * base);

/*!
 * @brief Causes an ACK to be sent on the bus.
 *
 * This function specifies that an ACK signal is sent in response to the next received byte.
 *
 * Note that the behaviour of this function is changed when the LPI2C peripheral is placed in
 * the fast ACK mode. In this case, this function causes an ACK signal to be sent in
 * response to the current byte, rather than the next received byte.
 *
 * @param base The LPI2C peripheral base pointer
 */
static inline void LPI2C_HAL_SlaveSendAck(LPI2C_Type * base)
{
    LPI2C_BWR_STAR_TXNACK(base, 0U);
}

/*!
 * @brief Causes a NAK to be sent on the bus.
 *
 * This function specifies that a NAK signal is sent in response to the next received byte.
 *
 * Note that the behaviour of this function is changed when the LPI2C peripheral is placed in the
 * fast ACK mode. In this case, this function causes an NAK signal to be sent in
 * response to the current byte, rather than the next received byte.
 *
 * @param base The LPI2C peripheral base pointer
 */
static inline void LPI2C_HAL_SlaveSendNak(LPI2C_Type * base)
{
    LPI2C_BWR_STAR_TXNACK(base, 1U);
}
/*!
 * @brief Enables/disables the SCL stretch.
 *
 * This function should be called before the slave is enabled.
 * @param base The LPI2C peripheral base pointer.
 * @param stallType situation of SCL stretch.
 * @param enable Enable/Disable SCL stretch.
 *               - true to enable SCL stretch
 *               - false to disable SCL stretch
 */
void LPI2C_HAL_SlaveSetSclStretch(LPI2C_Type * base, uint32_t stallType, bool enable);
/*!
 * @brief Sets the slave transmit flag configuration.
 *
 * This function should be called before the slave is enabled.
 * @param base The LPI2C peripheral base pointer.
 * @param txConfig configure when to assert transmit flag.
 */
static inline void LPI2C_HAL_SlaveSetTxConfig(LPI2C_Type * base,lpi2c_slave_txflag_assert_t txConfig)
{
    LPI2C_BWR_SCFGR1_TXCFG(base, txConfig);
}
/*!
 * @brief Selects the receive mode.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param rxSize Initialize receiving "rxSize" of bytes.
 */
static inline void LPI2C_HAL_MasterSetRxMode(LPI2C_Type * base, uint8_t rxSize)
{
    LPI2C_WR_MTDR(base, (LPI2C_MTDR_CMD(kLpi2cMasterCmdRxData) | LPI2C_MRDR_DATA(rxSize-1)));
}
/*@}*/

/*!
 * @name Master FIFO
 * @{
 */
/*!
 * @brief Sets the watermark for the transmit FIFO.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param value watermark of words for transmit FIFO.
 */
static inline void LPI2C_HAL_MasterSetTxWatermark(LPI2C_Type * base, lpi2c_master_watermark_t value)
{
    LPI2C_BWR_MFCR_TXWATER(base, value);
}
/*!
 * @brief Sets the watermark for the receive FIFO.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param value watermark of words for receive FIFO.
 */
static inline void LPI2C_HAL_MasterSetRxWatermark(LPI2C_Type * base, lpi2c_master_watermark_t value)
{
    LPI2C_BWR_MFCR_RXWATER(base, value);
}
/*!
 * @brief Gets the number of words in the transmit FIFO.
 *
 * @param base The LPI2C peripheral base pointer.
 * @return txcount number of words in transmit FIFO.
 */
static inline uint32_t LPI2C_HAL_MasterGetTxFifoCount(LPI2C_Type * base)
{
    return LPI2C_BRD_MFSR_TXCOUNT(base);
}
/*!
 * @brief Gets the number of words in the receive FIFO.
 *
 * @param base The LPI2C peripheral base pointer.
 * @return rxcount number of words in receive FIFO.
 */
static inline uint32_t LPI2C_HAL_MasterGetRxFifoCount(LPI2C_Type * base)
{
    return LPI2C_BRD_MFSR_RXCOUNT(base);
}
/*!
 * @brief Gets the number of words in the receive FIFO.
 *
 * @param base The LPI2C peripheral base pointer.
 * @return Whether receive FIFO is empty.
 *         - true: receive FIFO is empty.
 *         - false: receive FIFO is is not empty.
 */
static inline bool LPI2C_HAL_MasterCheckRxFifoEmpty(LPI2C_Type * base)
{
    return LPI2C_BRD_MRDR_RXEMPTY(base);
}
/*@}*/

/*!
 * @name Slave address
 * @{
 */

/*!
 * @brief Sets the primary 7-bit slave address.
 *
 * @param base The LPI2C peripheral base pointer
 * @param address The slave address in the upper 7 bits.
 */
void LPI2C_HAL_SetAddress7bit(LPI2C_Type * base, uint8_t address);

/*!
 * @brief Sets the primary slave address and enables 10-bit address mode.
 *
 * @param base The LPI2C peripheral base pointer
 * @param address The 10-bit slave address, in bits [9:0] of the value.
 */
void LPI2C_HAL_SetAddress10bit(LPI2C_Type * base, uint16_t address);

/*@}*/

/*!
 * @name Data transfer
 * @{
 */

/*!
 * @brief Returns the last byte of data read from the bus and initiate another read.
 *
 * @param base The LPI2C peripheral base pointer
 * @return This function returns the last byte received while the LPI2C module is configured in master
 *     receive or slave receive mode.
 */
static inline uint8_t LPI2C_HAL_MasterReadByte(LPI2C_Type * base)
{
    return LPI2C_RD_MRDR(base);
}

/*!
 * @brief Writes one byte of data to the LPI2C bus.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param byte The byte of data to transmit.
 */
static inline void LPI2C_HAL_MasterWriteByte(LPI2C_Type * base, uint8_t byte)
{
    LPI2C_WR_MTDR(base, byte);
}

/*!
 * @brief Returns the last byte of data read from the slave.
 *
 * @param base The LPI2C peripheral base pointer
 * @return This function returns the last byte received while the LPI2C module is configured in slave
 *     receive mode.
 */
static inline uint8_t LPI2C_HAL_SlaveReadByte(LPI2C_Type * base)
{
    return LPI2C_RD_SRDR(base);
}

/*!
 * @brief Writes one byte of data to the LPI2C slave transmit register.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param byte The byte of data to transmit.
 */
static inline void LPI2C_HAL_SlaveWriteByte(LPI2C_Type * base, uint8_t byte)
{
    LPI2C_WR_STDR(base, byte);
}
/*!
 * @brief Waits for the LPI2C master transfer to end.
 */
void LPI2C_HAL_MasterWaitTransfer(LPI2C_Type * base);
/*!
 * @brief Waits until there is a receive ready flag and reads one byte from the receive reg.
 *
 * @param base The LPI2C peripheral base pointer.
 * @return Returns the last byte received. 
 */
uint8_t LPI2C_HAL_SlaveReadByteBlocking(LPI2C_Type * base);
/*!
 * @brief Writes one byte of data to the LPI2C bus and waits till that byte is
 * transferred successfully.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param byte The byte of data to transmit.
 * @return Whether error happens (FALSE) or not(TRUE).
 */
bool LPI2C_HAL_SlaveWriteByteBlocking(LPI2C_Type * base, uint8_t byte);

/*!
 * @brief Waits until there is a receive ready flag and reads one byte from the receive FIFO.
 *
 * @param base The LPI2C peripheral base pointer.
 * @return Returns the last byte received. 
 */
uint8_t LPI2C_HAL_MasterReadByteBlocking(LPI2C_Type * base);

/*!
 * @brief Waits till a transmit request flag is set and writes one byte of data 
 * to the transmit FIFO, then waits till that byte is transferred successfully.
 *
 * @param base The LPI2C peripheral base pointer.
 * @param byte The byte of data to transmit.
 * @return Whether error happens (FALSE) or not(TRUE).
 */
bool LPI2C_HAL_MasterWriteByteBlocking(LPI2C_Type * base, uint8_t byte);

/*!
 * @brief Performs a polling receive transaction on the LPI2C bus.
 *
 * @param base  The LPI2C peripheral base pointer.
 * @param salveAddr The slave address to communicate.
 * @param cmdBuff   The pointer to the commands to be transferred.
 * @param cmdSize   The length in bytes of the commands to be transferred.
 * @param rxBuff    The pointer to the data to be transferred.
 * @param rxSize    The length in bytes of the data to be transferred.
 * @return Error or success status returned by API.
 */
lpi2c_status_t LPI2C_HAL_MasterReceiveDataPolling(LPI2C_Type * base,
                                                  uint16_t slaveAddr,
                                                  const uint8_t * cmdBuff,
                                                  uint32_t cmdSize,
                                                  uint8_t * rxBuff,
                                                  uint32_t rxSize);

/*!
 * @brief Performs a polling send transaction on the LPI2C bus.
 *
 * @param base  The LPI2C peripheral base pointer.
 * @param salveAddr The slave address to communicate.
 * @param cmdBuff   The pointer to the commands to be transferred.
 * @param cmdSize   The length in bytes of the commands to be transferred.
 * @param txBuff    The pointer to the data to be transferred.
 * @param txSize    The length in bytes of the data to be transferred.
 * @return Error or success status returned by API.
 */
lpi2c_status_t LPI2C_HAL_MasterSendDataPolling(LPI2C_Type * base,
                                               uint16_t slaveAddr,
                                               const uint8_t * cmdBuff,
                                               uint32_t cmdSize,
                                               const uint8_t * txBuff,
                                               uint32_t txSize);

/*!
* @brief Sends out multiple bytes of data using the polling method.
*
* @param  base LPI2C module base pointer.
* @param  txBuff The buffer pointer which saves the data to be sent.
* @param  txSize Size of data to be sent in unit of byte.
* @return Error or success status returned by API.
*/
lpi2c_status_t LPI2C_HAL_SlaveSendDataPolling(LPI2C_Type * base,
                                              const uint8_t* txBuff,
                                              uint32_t txSize);

/*!
* @brief Receives multiple bytes of data using the polling method.
*
* @param  base LPI2C module base pointer.
* @param  rxBuff The buffer pointer which saves the data to be received.
* @param  rxSize Size of data need to be received in unit of byte.
* @return Error or success status returned by API.
*/
lpi2c_status_t LPI2C_HAL_SlaveReceiveDataPolling(LPI2C_Type * base,
                                                 uint8_t *rxBuff,
                                                 uint32_t rxSize);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* __FSL_LPI2C_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/


