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

#ifndef _FSL_CLOCK_MKW40Z4_H_
#define _FSL_CLOCK_MKW40Z4_H_

#include <stdint.h>
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Clock ip name array for DMAMUX. */
#define DMAMUX_CLOCKS {kCLOCK_Dmamux0}

/*! @brief Clock ip name array for RTC. */
#define RTC_CLOCKS {kCLOCK_Rtc0}

/*! @brief Clock ip name array for PIT. */
#define PIT_CLOCKS {kCLOCK_Pit0}

/*! @brief Clock ip name array for PORT. */
#define PORT_CLOCKS {kCLOCK_PortA, kCLOCK_PortB, kCLOCK_PortC}

/*! @brief Clock ip name array for TSI. */
#define TSI_CLOCKS {kCLOCK_Tsi0}

/*! @brief Clock ip name array for DSPI. */
#define DSPI_CLOCKS {kCLOCK_Spi0, kCLOCK_Spi1}

/*! @brief Clock ip name array for LPUART. */
#define LPUART_CLOCKS {kCLOCK_Lpuart0}

/*! @brief Clock ip name array for DAC. */
#define DAC_CLOCKS {kCLOCK_Dac0}

/*! @brief Clock ip name array for LPTMR. */
#define LPTMR_CLOCKS {kCLOCK_Lptmr0}

/*! @brief Clock ip name array for ADC16. */
#define ADC16_CLOCKS {kCLOCK_Adc0}

/*! @brief Clock ip name array for TRNG. */
#define TRNG_CLOCKS {kCLOCK_Trng0}

/*! @brief Clock ip name array for DMA. */
#define DMA_CLOCKS {kCLOCK_Dma0}

/*! @brief Clock ip name array for CMT. */
#define CMT_CLOCKS {kCLOCK_Cmt0}

/*! @brief Clock ip name array for TPM. */
#define TPM_CLOCKS {kCLOCK_Tpm0, kCLOCK_Tpm1, kCLOCK_Tpm2}

/*! @brief Clock ip name array for LTC. */
#define LTC_CLOCKS {kCLOCK_Ltc0}

/*! @brief Clock ip name array for I2C. */
#define I2C_CLOCKS {kCLOCK_I2c0, kCLOCK_I2c1}

/*! @brief Clock ip name array for CMP. */
#define CMP_CLOCKS {kCLOCK_Cmp0, kCLOCK_Cmp1, kCLOCK_Cmp2}

/*!
 * @brief LPO clock frequency.
 */
#define LPO_CLK_FREQ 1000U

/*! @brief Prepherials clock source definition. */
#define SYS_CLK kCoreSysClk
#define BUS_CLK kBusClk

#define I2C0_CLK_SRC   BUS_CLK
#define I2C1_CLK_SRC   SYS_CLK
#define DSPI0_CLK_SRC  BUS_CLK
#define DSPI1_CLK_SRC  BUS_CLK

/*! @brief Clock name used to get clock frequency. */
typedef enum _clock_name
{
    /* ----------------------------- System layer clock -------------------------------*/
    kCLOCK_CoreSysClk,        /*!< Core/system clock                                         */
    kCLOCK_PlatClk,           /*!< Platform clock                                            */
    kCLOCK_BusClk,            /*!< Bus clock                                                 */
    kCLOCK_FlashClk,          /*!< Flash clock                                               */

    /* ---------------------------------- OSC clock -----------------------------------*/
    kCLOCK_Er32kClk,          /*!< External reference 32K clock (ERCLK32K)                   */
    kCLOCK_Osc0ErClk,         /*!< OSC0 external reference clock (OSC0ERCLK)                 */

    /* ----------------------------- MCG and MCG-Lite clock ---------------------------*/
    kCLOCK_McgFixedFreqClk,   /*!< MCG fixed frequency clock (MCGFFCLK)                      */
    kCLOCK_McgInternalRefClk, /*!< MCG internal reference clock (MCGIRCLK)                   */
    kCLOCK_McgFllClk,         /*!< MCGFLLCLK                                                 */
    kCLOCK_McgPeriphClk,      /*!< MCG peripheral clock (MCGPCLK)                            */

    /* --------------------------------- Other clock ----------------------------------*/
    kCLOCK_LpoClk,            /*!< LPO clock                                                 */

} clock_name_t;

/*------------------------------------------------------------------------------

 clock_gate_t definition:

 31                              16                              0
 -----------------------------------------------------------------
 | SIM_SCGC register offset       |   control bit offset in SCGC |
 -----------------------------------------------------------------

 For example, the SDHC clock gate is controlled by SIM_SCGC3[17], the
 SIM_SCGC3 offset in SIM is 0x1030, then kClockGateSdhc0 is defined as

              kClockGateSdhc0 = (0x1030 << 16) | 17;

------------------------------------------------------------------------------*/

#define CLK_GATE_REG_OFFSET_SHIFT           16
#define CLK_GATE_REG_OFFSET_MASK    0xFFFF0000
#define CLK_GATE_BIT_SHIFT_SHIFT             0
#define CLK_GATE_BIT_SHIFT_MASK     0x0000FFFF

#define CLK_GATE_DEFINE(reg_offset, bit_shift) \
   ((((reg_offset) << CLK_GATE_REG_OFFSET_SHIFT) & CLK_GATE_REG_OFFSET_MASK) | \
    (((bit_shift)  << CLK_GATE_BIT_SHIFT_SHIFT)  & CLK_GATE_BIT_SHIFT_MASK))

#define CLK_GATE_ABSTRACT_REG_OFFSET(x) (((x) & CLK_GATE_REG_OFFSET_MASK) \
                                         >> CLK_GATE_REG_OFFSET_SHIFT)
#define CLK_GATE_ABSTRACT_BITS_SHIFT(x) (((x) & CLK_GATE_BIT_SHIFT_MASK) \
                                         >> CLK_GATE_BIT_SHIFT_SHIFT)

/*! @brief Clock gate name used for CLOCK_EnableClock/CLOCK_DisableClock. */
typedef enum _clock_ip_name
{
    kCLOCK_IpInvalid = 0U,

    kCLOCK_Cmt0      = CLK_GATE_DEFINE(0x1034U, 2U),
    kCLOCK_I2c0      = CLK_GATE_DEFINE(0x1034U, 6U),
    kCLOCK_I2c1      = CLK_GATE_DEFINE(0x1034U, 7U),
    kCLOCK_Cmp0      = CLK_GATE_DEFINE(0x1034U, 19U),
    kCLOCK_Cmp1      = CLK_GATE_DEFINE(0x1034U, 19U),
    kCLOCK_Cmp2      = CLK_GATE_DEFINE(0x1034U, 19U),

    kCLOCK_Lptmr0    = CLK_GATE_DEFINE(0x1038U, 0U),
    kCLOCK_Tsi0      = CLK_GATE_DEFINE(0x1038U, 5U),
    kCLOCK_PortA     = CLK_GATE_DEFINE(0x1038U, 9U),
    kCLOCK_PortB     = CLK_GATE_DEFINE(0x1038U, 10U),
    kCLOCK_PortC     = CLK_GATE_DEFINE(0x1038U, 11U),
    kCLOCK_Lpuart0   = CLK_GATE_DEFINE(0x1038U, 20U),
    kCLOCK_Aesa      = CLK_GATE_DEFINE(0x1038U, 24U),
    kCLOCK_Ltc0      = CLK_GATE_DEFINE(0x1038U, 24U),
    kCLOCK_Rsim      = CLK_GATE_DEFINE(0x1038U, 25U),
    kCLOCK_Dcdc      = CLK_GATE_DEFINE(0x1038U, 26U),
    kCLOCK_Btll      = CLK_GATE_DEFINE(0x1038U, 27U),
    kCLOCK_PhyDig    = CLK_GATE_DEFINE(0x1038U, 28U),
    kCLOCK_ZigBee    = CLK_GATE_DEFINE(0x1038U, 29U),

    kCLOCK_Ftf0      = CLK_GATE_DEFINE(0x103CU, 0U),
    kCLOCK_Dmamux0   = CLK_GATE_DEFINE(0x103CU, 1U),
    kCLOCK_Trng0     = CLK_GATE_DEFINE(0x103CU, 9U),
    kCLOCK_Spi0      = CLK_GATE_DEFINE(0x103CU, 12U),
    kCLOCK_Spi1      = CLK_GATE_DEFINE(0x103CU, 13U),
    kCLOCK_Pit0      = CLK_GATE_DEFINE(0x103CU, 23U),
    kCLOCK_Tpm0      = CLK_GATE_DEFINE(0x103CU, 24U),
    kCLOCK_Tpm1      = CLK_GATE_DEFINE(0x103CU, 25U),
    kCLOCK_Tpm2      = CLK_GATE_DEFINE(0x103CU, 26U),
    kCLOCK_Adc0      = CLK_GATE_DEFINE(0x103CU, 27U),
    kCLOCK_Rtc0      = CLK_GATE_DEFINE(0x103CU, 29U),
    kCLOCK_Dac0      = CLK_GATE_DEFINE(0x103CU, 31U),

    kCLOCK_Dma0      = CLK_GATE_DEFINE(0x1040U, 8U),
} clock_ip_name_t;

/*!@brief SIM configuration structure for clock setting. */
typedef struct _sim_clock_config
{
    uint8_t  pllFllSel;  /*!< PLL/FLL/IRC48M selection.         */
    uint8_t  pllFllDiv;  /*!< PLLFLLSEL clock divider divisor.  */
    uint8_t  pllFllFrac; /*!< PLLFLLSEL clock divider fraction. */
    uint8_t  er32kSrc;   /*!< ERCLK32K source selection.        */
    uint32_t clkdiv1;    /*!< SIM_CLKDIV1.                      */
} sim_clock_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief Enable the clock for specific IP.
 *
 * @param name  Which clock to enable, see \ref clock_ip_name_t.
 */
static inline void CLOCK_EnableClock(clock_ip_name_t name)
{
    uint32_t regAddr = SIM_BASE + CLK_GATE_ABSTRACT_REG_OFFSET((uint32_t)name);
    (* (volatile uint32_t *)regAddr) |= (1U << CLK_GATE_ABSTRACT_BITS_SHIFT((uint32_t)name));
}

/*!
 * @brief Disable the clock for specific IP.
 *
 * @param name  Which clock to disable, see \ref clock_ip_name_t.
 */
static inline void CLOCK_DisableClock(clock_ip_name_t name)
{
    uint32_t regAddr = SIM_BASE + CLK_GATE_ABSTRACT_REG_OFFSET((uint32_t)name);
    (* (volatile uint32_t *)regAddr) &= ~(1U << CLK_GATE_ABSTRACT_BITS_SHIFT((uint32_t)name));
}

/*! @brief Set ERCLK32K source. */
static inline void CLOCK_SetEr32kClock(uint32_t src)
{
    SIM->SOPT1 = ((SIM->SOPT1 & ~SIM_SOPT1_OSC32KSEL_MASK) | SIM_SOPT1_OSC32KSEL(src));
}

/*! @brief Set LPUART clock source. */
static inline void CLOCK_SetLpuartClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_LPUART0SRC_MASK) | SIM_SOPT2_LPUART0SRC(src));
}

/*! @brief Set TPM clock source. */
static inline void CLOCK_SetTpmClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_TPMSRC_MASK) | SIM_SOPT2_TPMSRC(src));
}

/*! @brief Set CLKOUT source. */
static inline void CLOCK_SetClkOutClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_CLKOUTSEL_MASK) | SIM_SOPT2_CLKOUTSEL(src));
}

/*!
 * @brief
 * Set the SIM_CLKDIV1[OUTDIV1], SIM_CLKDIV1[OUTDIV4].
 */
static inline void CLOCK_SetOutDiv(uint32_t outdiv1,
                                   uint32_t outdiv4)
{
    SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) |
                   SIM_CLKDIV1_OUTDIV4(outdiv4);
}

/*!
 * @brief Gets the clock frequency for a specific clock name.
 *
 * This function checks the current clock configurations and then calculates
 * the clock frequency for a specific clock name defined in clock_name_t.
 * The MCG must be properly configured before using this function.
 *
 * @param clockName Clock names defined in clock_name_t
 * @return Clock frequency value in Hertz
 */
uint32_t CLOCK_GetFreq(clock_name_t clockName);

/*!
 * @brief Set the clock configure in SIM module.
 *
 * This function sets system layer clock settings in SIM module.
 *
 * @param config Pointer to the configure structure.
 */
void CLOCK_SetSimConfig(sim_clock_config_t const * config);

/*!
 * @brief Set the system clock dividers in SIM to safe value.
 *
 * The system level clocks (core clock, bus clock, flexbus clock and flash clock)
 * must be in allowed ranges. During MCG clock mode switch, the MCG output clock
 * changes then the system level clocks may be out of range. This function could
 * be used before MCG mode change, to make sure system level clocks are in allowed
 * range.
 *
 * @param config Pointer to the configure structure.
 */
static inline void CLOCK_SetSimSafeDivs(void)
{
    SIM->CLKDIV1 = 0x00040000U;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _FSL_CLOCK_H_ */
