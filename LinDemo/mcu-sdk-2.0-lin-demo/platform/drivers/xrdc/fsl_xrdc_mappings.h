/*
** ###################################################################
**     Version:             rev. 1.0, 2015-04-15
**     Build:               b151021
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
**     - rev. 1.0 (2015-04-15)
**         Initial Version.
**
** ###################################################################
*/

#ifndef _FSL_XRDC_MAPPINGS_H_
#define _FSL_XRDC_MAPPINGS_H_

/*!
 * @addtogroup xrdc_mapping
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Structure for the XRDC mapping
 *
 * Defines the structure for the XRDC resource collections.
 */

typedef enum _xrdc_master
{
#if defined(CPU_MKL28T512VDC7_CORE0) || defined(CPU_MKL28T512VDC7_CORE1) || defined(CPU_MKL28T512VLH7_CORE0) || defined(CPU_MKL28T512VLH7_CORE1) || \
    defined(CPU_MKL28T512VLL7_CORE0) || defined(CPU_MKL28T512VLL7_CORE1) || defined(CPU_MKL28T512VMP7_CORE0) || defined(CPU_MKL28T512VMP7_CORE1)
    kXRDC_MasterCpu0 = 0U, /*!< CPU0 */
    kXRDC_MasterDma0 = 1U, /*!< DMA0 */
    kXRDC_MasterUsb = 2U, /*!< USB */
    kXRDC_MasterCpu1 = 36U, /*!< CPU1 */
    kXRDC_MasterDma1 = 37U, /*!< DMA1 */
#else
    #error "No valid CPU defined!"
#endif
} xrdc_master_t;

typedef enum _xrdc_mem
{
#if defined(CPU_MKL28T512VDC7_CORE0) || defined(CPU_MKL28T512VDC7_CORE1) || defined(CPU_MKL28T512VLH7_CORE0) || defined(CPU_MKL28T512VLH7_CORE1) || \
    defined(CPU_MKL28T512VLL7_CORE0) || defined(CPU_MKL28T512VLL7_CORE1) || defined(CPU_MKL28T512VMP7_CORE0) || defined(CPU_MKL28T512VMP7_CORE1)
    kXRDC_MemMrc0_0 = 0U, /*!< MRC0 Memory 0 */
    kXRDC_MemMrc0_1 = 1U, /*!< MRC0 Memory 1 */
    kXRDC_MemMrc0_2 = 2U, /*!< MRC0 Memory 2 */
    kXRDC_MemMrc0_3 = 3U, /*!< MRC0 Memory 3 */
    kXRDC_MemMrc0_4 = 4U, /*!< MRC0 Memory 4 */
    kXRDC_MemMrc0_5 = 5U, /*!< MRC0 Memory 5 */
    kXRDC_MemMrc0_6 = 6U, /*!< MRC0 Memory 6 */
    kXRDC_MemMrc0_7 = 7U, /*!< MRC0 Memory 7 */
    kXRDC_MemMrc1_0 = 16U, /*!< MRC1 Memory 0 */
    kXRDC_MemMrc1_1 = 17U, /*!< MRC1 Memory 1 */
    kXRDC_MemMrc1_2 = 18U, /*!< MRC1 Memory 2 */
    kXRDC_MemMrc1_3 = 19U, /*!< MRC1 Memory 3 */
    kXRDC_MemMrc1_4 = 20U, /*!< MRC1 Memory 4 */
    kXRDC_MemMrc1_5 = 21U, /*!< MRC1 Memory 5 */
    kXRDC_MemMrc1_6 = 22U, /*!< MRC1 Memory 6 */
    kXRDC_MemMrc1_7 = 23U, /*!< MRC1 Memory 7 */
#else
    #error "No valid CPU defined!"
#endif
} xrdc_mem_t;

typedef enum _xrdc_periph
{
#if defined(CPU_MKL28T512VDC7_CORE0) || defined(CPU_MKL28T512VDC7_CORE1) || defined(CPU_MKL28T512VLH7_CORE0) || defined(CPU_MKL28T512VLH7_CORE1) || \
    defined(CPU_MKL28T512VLL7_CORE0) || defined(CPU_MKL28T512VLL7_CORE1) || defined(CPU_MKL28T512VMP7_CORE0) || defined(CPU_MKL28T512VMP7_CORE1)
    kXRDC_PeriphMscm = 1U, /*!< Miscellaneous System Control Module */
    kXRDC_PeriphDma0 = 8U, /*!< DMA0 Controller */
    kXRDC_PeriphDma0Tcd = 9U, /*!< DMA0 TCD */
    kXRDC_PeriphGpio = 15U, /*!< GPIO */
    kXRDC_PeriphXrdc0 = 20U, /*!< XRDC0 */
    kXRDC_PeriphXrdc1 = 21U, /*!< XRDC1 */
    kXRDC_PeriphXrdc2 = 22U, /*!< XRDC2 */
    kXRDC_PeriphXrdc3 = 23U, /*!< XRDC3 */
    kXRDC_PeriphSema420 = 27U, /*!< SEMA420 */
    kXRDC_PeriphFmu = 32U, /*!< Flash Memory Unit */
    kXRDC_PeriphDmamux0 = 33U, /*!< DMA0 Channel Multiplexer */
    kXRDC_PeriphMu0_A = 35U, /*!< Message Unit 0 (MU0_A) */
    kXRDC_PeriphIntmux0 = 36U, /*!< INTMUX0 */
    kXRDC_PeriphTrgmux0 = 39U, /*!< TRGMUX0 */
    kXRDC_PeriphTpm2 = 46U, /*!< TPM2 */
    kXRDC_PeriphLpit0 = 48U, /*!< LPIT0 */
    kXRDC_PeriphLptmr0 = 52U, /*!< LPTMR0 */
    kXRDC_PeriphRtc = 56U, /*!< RTC */
    kXRDC_PeriphLpspi2 = 62U, /*!< LPSPI2 */
    kXRDC_PeriphLpi2c2 = 66U, /*!< LPI2C2 */
    kXRDC_PeriphLpuart2 = 70U, /*!< LPUART2 */
    kXRDC_PeriphSai0 = 76U, /*!< LPSAI0 */
    kXRDC_PeriphEmvsim0 = 78U, /*!< EMVSIM0 */
    kXRDC_PeriphUsb0 = 85U, /*!< USB0 */
    kXRDC_PeriphPortA = 90U, /*!< PORTA Multiplex Control */
    kXRDC_PeriphPortB = 91U, /*!< PORTB Multiplex Control */
    kXRDC_PeriphPortC = 92U, /*!< PORTC Multiplex Control */
    kXRDC_PeriphPortD = 93U, /*!< PORTD Multiplex Control */
    kXRDC_PeriphPortE = 94U, /*!< PORTE Multiplex Control */
    kXRDC_PeriphLlwu0 = 97U, /*!< Low-leakage Wake-up Unit 0 (LLWU0) */
    kXRDC_PeriphTsi0 = 98U, /*!< TSI0 */
    kXRDC_PeriphAdc0 = 102U, /*!< ADC0 */
    kXRDC_PeriphDac0 = 106U, /*!< DAC0 */
    kXRDC_PeriphCmp0 = 110U, /*!< CMP0 */
    kXRDC_PeriphVref = 114U, /*!< VREF */
    kXRDC_PeriphSim = 116U, /*!< SIM */
    kXRDC_PeriphTstmr0 = 117U, /*!< TSTMR0 */
    kXRDC_PeriphWdog0 = 118U, /*!< WDOG0 */
    kXRDC_PeriphCrc = 120U, /*!< CRC */
    kXRDC_PeriphPcc0 = 122U, /*!< PCC0 */
    kXRDC_PeriphScg = 123U, /*!< SCG */
    kXRDC_PeriphSysReg = 123U, /*!< System Register File */
    kXRDC_PeriphPmc = 125U, /*!< PMC */
    kXRDC_PeriphSmc = 126U, /*!< SMC */
    kXRDC_PeriphRcm = 127U, /*!< RCM */
    kXRDC_PeriphDma1 = 136U, /*!< DMA1 Controller */
    kXRDC_PeriphDma1Tcd = 137U, /*!< DMA1 TCD */
    kXRDC_PeriphRpmGpio = 143U, /*!< RPM GPIO Controller */
    kXRDC_PeriphSema421 = 155U, /*!< SEMA42 1 */
    kXRDC_PeriphDmamux1 = 161U, /*!< DMA1 Channel Multiplexer */
    kXRDC_PeriphMu0_B = 163U, /*!< Memory Unit 0 (MU0_B) */
    kXRDC_PeriphIntmux1 = 164U, /*!< INTMUX1 */
    kXRDC_PeriphTrng = 165U, /*!< Random Number Generator */
    kXRDC_PeriphTrgmux1 = 167U, /*!< TRGMUX1 */
    kXRDC_PeriphTpm0 = 172U, /*!< TPM0 */
    kXRDC_PeriphTpm1 = 173U, /*!< TPM1 */
    kXRDC_PeriphLpit1 = 177U, /*!< LPIT1 */
    kXRDC_PeriphLptrm1 = 181U, /*!< Low-power Timer 1 */
    kXRDC_PeriphLpspi0 = 188U, /*!< LPSPI0 */
    kXRDC_PeriphLpspi1 = 189U, /*!< LPSPI1 */
    kXRDC_PeriphLpi2c0 = 192U, /*!< LPI2C0 */
    kXRDC_PeriphLpi2c1 = 193U, /*!< LPI2C1 */
    kXRDC_PeriphLpuart0 = 196U, /*!< LPUART0 */
    kXRDC_PeriphLpuart1 = 197U, /*!< LPUART1 */
    kXRDC_PeriphFlexio0 = 202U, /*!< FLEXIO0 */
    kXRDC_PeriphPortRpm = 224U, /*!< PORTRPM */
    kXRDC_PeriphLlwu1 = 225U, /*!< LLWU1 */
    kXRDC_PeriphCmp1 = 239U, /*!< CMP1 */
    kXRDC_PeriphTstmr1 = 245U, /*!< TSTMR1 */
    kXRDC_PeriphWdog1 = 246U, /*!< WDOG1 */
    kXRDC_PeriphPcc1 = 250U, /*!< PCC1 */
    kXRDC_PeriphUsbram = 256U, /*!< USB RAM */
#else
    #error "No valid CPU defined!"
#endif
} xrdc_periph_t;

/* @} */

#endif /* _FSL_XRDC_MAPPINGS_H_ */
