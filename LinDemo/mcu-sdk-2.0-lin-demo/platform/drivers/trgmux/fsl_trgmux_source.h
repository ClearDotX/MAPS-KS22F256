/*
** ###################################################################
**     Version:             rev. 1.0, 2015-06-04
**     Build:               b151102
**
**     Abstract:
**         Enumeration value mapping header.
**
**     Copyright (c) 2015 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2015-06-04)
**         Initial Version.
**
** ###################################################################
*/

#ifndef _FSL_TRGMUX_SOURCE_H_
#define _FSL_TRGMUX_SOURCE_H_

/*!
 * @addtogroup trgmux_source
 * @{ */

/*******************************************************************************
 * Definitions
*******************************************************************************/

/*!
 * @brief Structure for the TRGMUX source
 *
 * Defines the structure for the TRGMUX source collections.
 */
typedef enum _trgmux_source
{
#if defined(CPU_MKL28T512VDC7_CORE0) || defined(CPU_MKL28T512VDC7_CORE1) || defined(CPU_MKL28T512VLH7_CORE0) || defined(CPU_MKL28T512VLH7_CORE1) || \
    defined(CPU_MKL28T512VLL7_CORE0) || defined(CPU_MKL28T512VLL7_CORE1) || defined(CPU_MKL28T512VMP7_CORE0) || defined(CPU_MKL28T512VMP7_CORE1) || \
    defined(CPU_MKL28Z512VDC7) || defined(CPU_MKL28Z512VLH7) || defined(CPU_MKL28Z512VLL7) || defined(CPU_MKL28Z512VMP7)
    kTRGMUX_SourceDisabled = 0U, /*!< Trigger function is disabled */
    kTRGMUX_SourcePortPin = 1U, /*!< Port pin trigger intput is selected */
    kTRGMUX_SourceFlexIOTimer0 = 2U, /*!< FlexIO Timer 0 input is selected */
    kTRGMUX_SourceFlexIOTimer1 = 3U, /*!< FlexIO Timer 1 input is selected */
    kTRGMUX_SourceFlexIOTimer2 = 4U, /*!< FlexIO Timer 2 input is selected */
    kTRGMUX_SourceFlexIOTimer3 = 5U, /*!< FlexIO Timer 3 input is selected */
    kTRGMUX_SourceFlexIOTimer4 = 6U, /*!< FlexIO Timer 4 input is selected */
    kTRGMUX_SourceFlexIOTimer5 = 7U, /*!< FlexIO Timer 5 input is selected */
    kTRGMUX_SourceFlexIOTimer6 = 8U, /*!< FlexIO Timer 6 input is selected */
    kTRGMUX_SourceFlexIOTimer7 = 9U, /*!< FlexIO Timer 7 input is selected */
    kTRGMUX_SourceTpm0Overflow = 10U, /*!< TPM0 Overflow is selected */
    kTRGMUX_SourceTpm0Ch0 = 11U, /*!< TPM0 Channel 0 is selected */
    kTRGMUX_SourceTpm0Ch1 = 12U, /*!< TPM0 Channel 1 is selected */
    kTRGMUX_SourceTpm1Overflow = 13U, /*!< TPM1 Overflow is selected */
    kTRGMUX_SourceTpm1Ch0 = 14U, /*!< TPM1 Channel 0 is selected */
    kTRGMUX_SourceTpm1Ch1 = 15U, /*!< TPM1 Channel 1 is selected */
    kTRGMUX_SourceLpit1Ch0 = 16U, /*!< LPIT1 Channel 0 is selected */
    kTRGMUX_SourceLpit1Ch1 = 17U, /*!< LPIT1 Channel 1 is selected */
    kTRGMUX_SourceLpit1Ch2 = 18U, /*!< LPIT1 Channel 2 is selected */
    kTRGMUX_SourceLpit1Ch3 = 19U, /*!< LPIT1 Channel 3 is selected */
    kTRGMUX_SourceLpuart0RxData = 20U, /*!< LPUART0 RX Data is selected */
    kTRGMUX_SourceLpuart0TxData = 21U, /*!< LPUART0 TX Data is selected */
    kTRGMUX_SourceLpuart0RxIdle = 22U, /*!< LPUART0 RX Idle is selected */
    kTRGMUX_SourceLpuart1RxData = 23U, /*!< LPUART1 RX Data is selected */
    kTRGMUX_SourceLpuart1TxData = 24U, /*!< LPUART1 TX Data is selected */
    kTRGMUX_SourceLpuart1RxIdle = 25U, /*!< LPUART1 RX Idle is selected */
    kTRGMUX_SourceLpi2c0MasterStop = 26U, /*!< LPI2C0 Master STOP is selected */
    kTRGMUX_SourceLpi2c0SlaveStop = 27U, /*!< LPI2C0 Slave STOP is selected */
    kTRGMUX_SourceLpi2c1MasterStop = 28U, /*!< LPI2C1 Master STOP is selected */
    kTRGMUX_SourceLpi2c1SlaveStop = 29U, /*!< LPI2C1 Slave STOP is selected */
    kTRGMUX_SourceLpspi0Frame = 30U, /*!< LPSPI0 Frame is selected */
    kTRGMUX_SourceLpspi0RxData = 31U, /*!< LPSPI0 RX Data is selected */
    kTRGMUX_SourceLpspi1Frame = 32U, /*!< LPSPI1 Frame is selected */
    kTRGMUX_SourceLpspi1RxData = 33U, /*!< LPSPI1 RX Data is selected */
    kTRGMUX_SourceRtcSecCount = 34U, /*!< RTC Seconds Counter is selected */
    kTRGMUX_SourceRtcAlarm = 35U, /*!< RTC Alarm is selected */
    kTRGMUX_SourceLptmr0Trg = 36U, /*!< LPTMR0 Trigger is selected */
    kTRGMUX_SourceLptmr1Trg = 37U, /*!< LPTMR1 Trigger is selected */
    kTRGMUX_SourceCmp0Output = 38U, /*!< CMP0 Output is selected */
    kTRGMUX_SourceCmp1Output = 39U, /*!< CMP1 Output is selected */
    kTRGMUX_SourceAdc0ConvAComplete = 40U, /*!< ADC0 Conversion A Complete is selected */
    kTRGMUX_SourceAdc0ConvBComplete = 41U, /*!< ADC0 Conversion B Complete is selected */
    kTRGMUX_SourcePortAPinTrg = 42U, /*!< Port A Pin Trigger is selected */
    kTRGMUX_SourcePortBPinTrg = 43U, /*!< Port B Pin Trigger is selected */
    kTRGMUX_SourcePortCPinTrg = 44U, /*!< Port C Pin Trigger is selected */
    kTRGMUX_SourcePortDPinTrg = 45U, /*!< Port D Pin Trigger is selected */
    kTRGMUX_SourcePortEPinTrg = 46U, /*!< Port E Pin Trigger is selected */
    kTRGMUX_SourceTpm2Overflow = 47U, /*!< TPM2 Overflow is selected */
    kTRGMUX_SourceTpm2Ch0 = 48U, /*!< TPM2 Channel 0 is selected */
    kTRGMUX_SourceTpm2Ch1 = 49U, /*!< TPM2 Channel 1 is selected */
    kTRGMUX_SourceLpit0Ch0 = 50U, /*!< LPIT0 Channel 0 is selected */
    kTRGMUX_SourceLpit0Ch1 = 51U, /*!< LPIT0 Channel 1 is selected */
    kTRGMUX_SourceLpit0Ch2 = 52U, /*!< LPIT0 Channel 2 is selected */
    kTRGMUX_SourceLpit0Ch3 = 53U, /*!< LPIT0 Channel 3 is selected */
    kTRGMUX_SourceUsbSof = 54U, /*!< USB Start-of-Frame is selected */
    kTRGMUX_SourceLpuart2RxData = 55U, /*!< LPUART2 RX Data is selected */
    kTRGMUX_SourceLpuart2TxData = 56U, /*!< LPUART2 TX Data is selected */
    kTRGMUX_SourceLpuart2RxIdle = 57U, /*!< LPUART2 RX Idle is selected */
    kTRGMUX_SourceLpi2c2MasterStop = 58U, /*!< LPI2C2 Master STOP is selected */
    kTRGMUX_SourceLpi2c2SlaveStop = 59U, /*!< LPI2C2 Slave STOP is selected */
    kTRGMUX_SourceLpspi2Frame = 60U, /*!< LPSPI2 Frame is selected */
    kTRGMUX_SourceLpspi2RxData = 61U, /*!< LPSPI2 RX Data is selected */
    kTRGMUX_SourceI2c0TxFrameSync = 62U, /*!< I2C0 TX Frame Sync is selected */
    kTRGMUX_SourceI2c0RxFrameSync = 63U, /*!< I2C0 RX Frame Sync is selected */
#elif defined(CPU_PKE18F512VLH15) || defined(CPU_PKE18F512VLL15)
    kTRGMUX_SourceDisabled = 0U, /*!< Trigger function is disabled */
    kTRGMUX_SourceDisabled = 1U, /*!< Trigger function is disabled */
    kTRGMUX_SourceTriggerMuxInput0 = 2U, /*!< Trigger Mux input 0 is selected */
    kTRGMUX_SourceTriggerMuxInput1 = 3U, /*!< Trigger Mux input 1 is selected */
    kTRGMUX_SourceTriggerMuxInput2 = 4U, /*!< Trigger Mux input 2 is selected */
    kTRGMUX_SourceTriggerMuxInput3 = 5U, /*!< Trigger Mux input 3 is selected */
    kTRGMUX_SourceTriggerMuxInput4 = 6U, /*!< Trigger Mux input 4 is selected */
    kTRGMUX_SourceTriggerMuxInput5 = 7U, /*!< Trigger Mux input 5 is selected */
    kTRGMUX_SourceTriggerMuxInput6 = 8U, /*!< Trigger Mux input 6 is selected */
    kTRGMUX_SourceTriggerMuxInput7 = 9U, /*!< Trigger Mux input 7 is selected */
    kTRGMUX_SourceTriggerMuxInput8 = 10U, /*!< Trigger Mux input 8 is selected */
    kTRGMUX_SourceTriggerMuxInput9 = 11U, /*!< Trigger Mux input 9 is selected */
    kTRGMUX_SourceTriggerMuxInput10 = 12U, /*!< Trigger Mux input 10 is selected */
    kTRGMUX_SourceTriggerMuxInput11 = 13U, /*!< Trigger Mux input 11 is selected */
    kTRGMUX_SourceCmp0Output = 14U, /*!< CMP0 is selected */
    kTRGMUX_SourceCmp1Output = 15U, /*!< CMP1 is selected */
    kTRGMUX_SourceCmp2Output = 16U, /*!< CMP2 is selected */
    kTRGMUX_SourceLpitCh0 = 17U, /*!< LPIT Channel 0 is selected */
    kTRGMUX_SourceLpitCh1 = 18U, /*!< LPIT Channel 1 is selected */
    kTRGMUX_SourceLpitCh2 = 19U, /*!< LPIT Channel 2 is selected */
    kTRGMUX_SourceLpitCh3 = 20U, /*!< LPIT Channel 3 is selected */
    kTRGMUX_SourceLptmr0 = 21U, /*!< LPTMR0 Trigger is selected */
    kTRGMUX_SourceFtm0Internal = 22U, /*!< FTM0 Internal Trigger is selected */
    kTRGMUX_SourceFtm0External = 23U, /*!< FTM0 External Trigger is selected */
    kTRGMUX_SourceFtm1Internal = 24U, /*!< FTM1 Internal Trigger is selected */
    kTRGMUX_SourceFtm1External = 25U, /*!< FTM1 External Trigger is selected */
    kTRGMUX_SourceFtm2Internal = 26U, /*!< FTM2 Internal Trigger is selected */
    kTRGMUX_SourceFtm2External = 27U, /*!< FTM2 External Trigger is selected */
    kTRGMUX_SourceFtm3Internal = 28U, /*!< FTM3 Internal Trigger is selected */
    kTRGMUX_SourceFtm3External = 29U, /*!< FTM3 External Trigger is selected */
    kTRGMUX_SourceAdc0CocoA = 30U, /*!< ADC0 COCOA is selected */
    kTRGMUX_SourceAdc0CocoB = 31U, /*!< ADC0 COCOB is selected */
    kTRGMUX_SourceAdc1CocoAOrAdc2CocoA = 32U, /*!< ADC1 or ADC2 COCOA is selected */
    kTRGMUX_SourceAdc1CocoBOrAdc2CocoB = 33U, /*!< ADC1 or ADC2 COCOB is selected */
    kTRGMUX_SourcePdb0Ch0 = 34U, /*!< PDB0 Channel 0 is selected */
    kTRGMUX_SourcePdb0Ch1 = 35U, /*!< PDB0 Channel 1 is selected */
    kTRGMUX_SourcePdb0Ch2 = 36U, /*!< PDB0 Channel 2 is selected */
    kTRGMUX_SourcePdb1Ch0 = 37U, /*!< PDB1 Channel 0 is selected */
    kTRGMUX_SourcePdb1Ch1 = 38U, /*!< PDB1 Channel 1 is selected */
    kTRGMUX_SourcePdb1Ch2 = 39U, /*!< PDB1 Channel 2 is selected */
    kTRGMUX_SourcePdb2Ch0 = 40U, /*!< PDB2 Channel 0 is selected */
    kTRGMUX_SourcePdb2Ch1 = 41U, /*!< PDB2 Channel 1 is selected */
    kTRGMUX_SourcePdb2Ch2 = 42U, /*!< PDB2 Channel 2 is selected */
    kTRGMUX_SourceRtcAlarm = 43U, /*!< RTC Alarm is selected */
    kTRGMUX_SourceRtcSecondsCounter = 44U, /*!< RTC Seconds Counter is selected */
    kTRGMUX_SourceFlexIOTrigger0 = 45U, /*!< FLEXIO Trigger 0 is selected */
    kTRGMUX_SourceFlexIOTrigger1 = 46U, /*!< FLEXIO Trigger 1 is selected */
    kTRGMUX_SourceFlexIOTrigger2 = 47U, /*!< FLEXIO Trigger 2 is selected */
    kTRGMUX_SourceFlexIOTrigger3 = 48U, /*!< FLEXIO Trigger 3 is selected */
    kTRGMUX_SourceLpuart0RxData = 49U, /*!< LPUART0 RX Data is selected */
    kTRGMUX_SourceLpuart0TxData = 50U, /*!< LPUART0 TX Data is selected */
    kTRGMUX_SourceLpuart0RxIdle = 51U, /*!< LPUART0 RX Idle is selected */
    kTRGMUX_SourceLpuart1RxData = 52U, /*!< LPUART1 RX Data is selected */
    kTRGMUX_SourceLpuart1TxData = 53U, /*!< LPUART1 TX Data is selected */
    kTRGMUX_SourceLpuart1RxIdle = 54U, /*!< LPUART1 RX Idle is selected */
    kTRGMUX_SourceLpi2c0MasterStop = 55U, /*!< LPI2C0 Master STOP is selected */
    kTRGMUX_SourceLpi2c0SlaveStop = 56U, /*!< LPI2C0 Slave STOP is selected */
    kTRGMUX_SourceLpi2c1MasterStop = 57U, /*!< LPI2C1 Master STOP is selected */
    kTRGMUX_SourceLpi2c1SlaveStop = 58U, /*!< LPI2C1 Slave STOP is selected */
    kTRGMUX_SourceLpspi0Frame = 59U, /*!< LPSPI0 Frame is selected */
    kTRGMUX_SourceLpspi0RxData = 60U, /*!< LPSPI0 RX Data is selected */
    kTRGMUX_SourceLpspi1Frame = 61U, /*!< LPSPI1 Frame is selected */
    kTRGMUX_SourceLpspi1RxData = 62U, /*!< LPSPI1 RX Data is selected */
    kTRGMUX_SimSoftwareTrigger = 63U, /*!< SIM Software Trigger is selected */
#else
    #error "No valid CPU defined!"
#endif
} trgmux_source_t;

/*!
 * @brief Structure for the TRGMUX device
 *
 * Defines the structure for the TRGMUX device collections.
 */
typedef enum _trgmux_device
{
#if defined(CPU_MKL28T512VDC7_CORE0) || defined(CPU_MKL28T512VDC7_CORE1) || defined(CPU_MKL28T512VLH7_CORE0) || defined(CPU_MKL28T512VLH7_CORE1) || \
    defined(CPU_MKL28T512VLL7_CORE0) || defined(CPU_MKL28T512VLL7_CORE1) || defined(CPU_MKL28T512VMP7_CORE0) || defined(CPU_MKL28T512VMP7_CORE1) || \
    defined(CPU_MKL28Z512VDC7) || defined(CPU_MKL28Z512VLH7) || defined(CPU_MKL28Z512VLL7) || defined(CPU_MKL28Z512VMP7)
    kTRGMUX_Trgmux0Dmamux0 = 0U, /*!< DMAMUX0 device trigger input */
    kTRGMUX_Trgmux0Lpit0 = 1U,   /*!< LPIT0 device trigger input */
    kTRGMUX_Trgmux0Tpm2 = 2U,    /*!< TPM2 device trigger input */
    kTRGMUX_Trgmux0Adc0 = 4U,    /*!< ADC0 device trigger input */
    kTRGMUX_Trgmux0Lpuart2 = 5U, /*!< LPUART2 device trigger input */
    kTRGMUX_Trgmux0Lpi2c2 = 7U,  /*!< LPI2C2 device trigger input */
    kTRGMUX_Trgmux0Lpspi2 = 9U,  /*!< LPSPI2 device trigger input */
    kTRGMUX_Trgmux0Cmp0 = 11U,   /*!< CMP0 device trigger input */
    kTRGMUX_Trgmux0Cmp1 = 12U,   /*!< CMP1 device trigger input */
    kTRGMUX_Trgmux0Dac0 = 13U,   /*!< DAC0 device trigger input */
    kTRGMUX_Trgmux1Tpm0 = 2U,    /*!< TPM0 device trigger input */
    kTRGMUX_Trgmux1Tpm1 = 3U,    /*!< TPM1 device trigger input */
    kTRGMUX_Trgmux1Flexio = 4U,  /*!< FLEXIO device trigger input */
    kTRGMUX_Trgmux1Lpuart0 = 5U, /*!< LPUART0 device trigger input */
    kTRGMUX_Trgmux1Lpuart1 = 6U, /*!< LPUART1 device trigger input */
    kTRGMUX_Trgmux1Lpi2c0 = 7U,  /*!< LPI2C0 device trigger input */
    kTRGMUX_Trgmux1Lpi2c1 = 8U,  /*!< LPI2C1 device trigger input */
    kTRGMUX_Trgmux1Lpspi0 = 9U,  /*!< LPSPI0 device trigger input */
    kTRGMUX_Trgmux1Lpspi1 = 10U, /*!< LPSPI1 device trigger input */
#elif defined(CPU_PKE18F512VLH15) || defined(CPU_PKE18F512VLL15)
    kTRGMUX_Dmamux0 = 0U, /*!< DMAMUX0 device trigger input */
    kTRGMUX_XbarOut0_3 = 1U, /*!< XBAR device trigger input */
    kTRGMUX_XbarOut1_4 = 2U, /*!< XBAR device trigger input */
    kTRGMUX_Adc0 = 3U, /*!< ADC0 device trigger input */
    kTRGMUX_Adc1 = 4U, /*!< ADC1 device trigger input */
    kTRGMUX_Adc2 = 5U, /*!< ADC2 device trigger input */
    kTRGMUX_Dac = 6U, /*!< DAC device trigger input */
    kTRGMUX_Cmp0 = 7U, /*!< CMP0 device trigger input */
    kTRGMUX_Cmp1 = 8U, /*!< CMP1 device trigger input */
    kTRGMUX_Cmp2 = 9U, /*!< CMP2 device trigger input */
    kTRGMUX_Ftm0 = 10U, /*!< FTM0 device trigger input */
    kTRGMUX_Ftm1 = 11U, /*!< FTM1 device trigger input */
    kTRGMUX_Ftm2 = 12U, /*!< FTM2 device trigger input */
    kTRGMUX_Ftm3 = 13U, /*!< FTM3 device trigger input */
    kTRGMUX_Pdb0 = 14U, /*!< PDB0 device trigger input */
    kTRGMUX_Pdb1 = 15U, /*!< PDB1 device trigger input */
    kTRGMUX_Pdb2 = 16U, /*!< PDB2 device trigger input */
    kTRGMUX_Flexio = 17U, /*!< FLEXIO device trigger input */
    kTRGMUX_Lpit = 18U, /*!< LPIT device trigger input */
    kTRGMUX_Lpuart0 = 19U, /*!< LPUART0 device trigger input */
    kTRGMUX_Lpuart1 = 20U, /*!< LPUART1 device trigger input */
    kTRGMUX_Lpi2c0 = 21U, /*!< LPI2C0 device trigger input */
    kTRGMUX_Lpi2c1 = 22U, /*!< LPI2C1 device trigger input */
    kTRGMUX_Lpspi0 = 23U, /*!< LPSPI0 device trigger input */
    kTRGMUX_Lpspi1 = 24U, /*!< LPSPI1 device trigger input */
    kTRGMUX_Lptmr0 = 25U, /*!< LPTMR0 device trigger input */
    kTRGMUX_Tsi = 26U, /*!< TSI device trigger input */
#else
    #error "No valid CPU defined!"
#endif
} trgmux_device_t;

/* @} */

#endif /* _FSL_TRGMUX_SOURCE_H_ */
