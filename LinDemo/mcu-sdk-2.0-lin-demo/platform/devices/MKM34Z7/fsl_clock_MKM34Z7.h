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

#ifndef _FSL_CLOCK_MKM34Z7_H_
#define _FSL_CLOCK_MKM34Z7_H_

#include <stdint.h>
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Clock ip name array for DMAMUX. */
#define DMAMUX_CLOCKS {kCLOCK_Dmamux0}

/*! @brief Clock ip name array for RTC. */
#define RTC_CLOCKS {kCLOCK_Rtc0}

/*! @brief Clock ip name array for SPI. */
#define SPI_CLOCKS {kCLOCK_Spi0, kCLOCK_Spi1}

/*! @brief Clock ip name array for SLCD. */
#define SLCD_CLOCKS {kCLOCK_Slcd0}

/*! @brief Clock ip name array for EWM. */
#define EWM_CLOCKS {kCLOCK_Ewm0}

/*! @brief Clock ip name array for AFE. */
#define AFE_CLOCKS {kCLOCK_Afe0}

/*! @brief Clock ip name array for MMAU. */
#define MMAU_CLOCKS {kCLOCK_Mmau0}

/*! @brief Clock ip name array for LPUART. */
#define LPUART_CLOCKS {kCLOCK_Lpuart0}

/*! @brief Clock ip name array for ADC16. */
#define ADC16_CLOCKS {kCLOCK_Adc0}

/*! @brief Clock ip name array for XBAR. */
#define XBAR_CLOCKS {kCLOCK_Xbar}

/*! @brief Clock ip name array for MMCAU. */
#define MMCAU_CLOCKS {kCLOCK_Mmcau0}

/*! @brief Clock ip name array for MPU. */
#define MPU_CLOCKS {kCLOCK_Mpu0}

/*! @brief Clock ip name array for VREF. */
#define VREF_CLOCKS {kCLOCK_Vref0}

/*! @brief Clock ip name array for DMA. */
#define DMA_CLOCKS {kCLOCK_Dma0}

/*! @brief Clock ip name array for PORT. */
#define PORT_CLOCKS {kCLOCK_PortA, kCLOCK_PortB, kCLOCK_PortC, kCLOCK_PortD, kCLOCK_PortE, kCLOCK_PortF, kCLOCK_PortG, kCLOCK_PortH, kCLOCK_PortI, kCLOCK_PortK, kCLOCK_PortL, kCLOCK_PortM}

/*! @brief Clock ip name array for UART. */
#define UART_CLOCKS {kCLOCK_Uart0, kCLOCK_Uart1, kCLOCK_Uart2, kCLOCK_Uart3}

/*! @brief Clock ip name array for PIT. */
#define PIT_CLOCKS {kCLOCK_Pit0, kCLOCK_Pit1}

/*! @brief Clock ip name array for RNGA. */
#define RNGA_CLOCKS {kCLOCK_Rnga0}

/*! @brief Clock ip name array for CRC. */
#define CRC_CLOCKS {kCLOCK_Crc0}

/*! @brief Clock ip name array for I2C. */
#define I2C_CLOCKS {kCLOCK_I2c0, kCLOCK_I2c1}

/*! @brief Clock ip name array for LPTMR. */
#define LPTMR_CLOCKS {kCLOCK_Lptmr0}

/*! @brief Clock ip name array for TMR. */
#define TMR_CLOCKS {kCLOCK_Tmr0, kCLOCK_Tmr1, kCLOCK_Tmr2, kCLOCK_Tmr3}

/*! @brief Clock ip name array for PDB. */
#define PDB_CLOCKS {kCLOCK_Pdb0}

/*! @brief Clock ip name array for FTF. */
#define FTF_CLOCKS {kCLOCK_Ftf0}

/*! @brief Clock ip name array for CMP. */
#define CMP_CLOCKS {kCLOCK_Cmp0, kCLOCK_Cmp1, kCLOCK_Cmp2}

/*!
 * @brief LPO clock frequency.
 */
#define LPO_CLK_FREQ 1000U

/*! @brief Peripherals clock source definition. */
#define SYS_CLK kCLOCK_CoreSysClk
#define BUS_CLK kCLOCK_BusClk

#define I2C0_CLK_SRC    BUS_CLK
#define I2C1_CLK_SRC    BUS_CLK
#define SPI0_CLK_SRC    SYS_CLK
#define SPI1_CLK_SRC    SYS_CLK
#define UART0_CLK_SRC   BUS_CLK
#define UART1_CLK_SRC   SYS_CLK
#define UART2_CLK_SRC   BUS_CLK
#define UART3_CLK_SRC   SYS_CLK

/*! @brief Clock name used to get clock frequency. */
typedef enum _clock_name
{

    /* ----------------------------- System layer clock -------------------------------*/
    kCLOCK_CoreSysClk,        /*!< Core/system clock                                         */
    kCLOCK_PlatClk,           /*!< Platform clock                                            */
    kCLOCK_BusClk,            /*!< Bus clock                                                 */
    kCLOCK_FlashClk,          /*!< Flash clock                                               */
    kCLOCK_PllFllSelClk,      /*!< The clock after SIM[PLLFLLSEL].                           */

    /* ---------------------------------- OSC clock -----------------------------------*/
    kCLOCK_Er32kClk,          /*!< External reference 32K clock (ERCLK32K)                   */
    kCLOCK_Osc0ErClk,         /*!< OSC0 external reference clock (OSC0ERCLK)                 */

    /* ----------------------------- MCG and MCG-Lite clock ---------------------------*/
    kCLOCK_McgFixedFreqClk,   /*!< MCG fixed frequency clock (MCGFFCLK)                      */
    kCLOCK_McgInternalRefClk, /*!< MCG internal reference clock (MCGIRCLK)                   */
    kCLOCK_McgFllClk,         /*!< MCGFLLCLK                                                 */
    kCLOCK_McgPll0Clk,        /*!< MCGPLL0CLK                                                */
    kCLOCK_McgExtPllClk,      /*!< EXT_PLLCLK                                                */
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
    kCLOCK_Ewm0      = CLK_GATE_DEFINE(0x1034U, 1U),
    kCLOCK_I2c0      = CLK_GATE_DEFINE(0x1034U, 7U),
    kCLOCK_I2c1      = CLK_GATE_DEFINE(0x1034U, 8U),
    kCLOCK_Uart0     = CLK_GATE_DEFINE(0x1034U, 10U),
    kCLOCK_Uart1     = CLK_GATE_DEFINE(0x1034U, 11U),
    kCLOCK_Uart2     = CLK_GATE_DEFINE(0x1034U, 12U),
    kCLOCK_Uart3     = CLK_GATE_DEFINE(0x1034U, 13U),
    kCLOCK_Vref0     = CLK_GATE_DEFINE(0x1034U, 15U),
    kCLOCK_Cmp0      = CLK_GATE_DEFINE(0x1034U, 18U),
    kCLOCK_Cmp1      = CLK_GATE_DEFINE(0x1034U, 18U),
    kCLOCK_Cmp2      = CLK_GATE_DEFINE(0x1034U, 18U),
    kCLOCK_Spi0      = CLK_GATE_DEFINE(0x1034U, 21U),
    kCLOCK_Spi1      = CLK_GATE_DEFINE(0x1034U, 22U),

    kCLOCK_Slcd0     = CLK_GATE_DEFINE(0x1038U, 3U),
    kCLOCK_PortA     = CLK_GATE_DEFINE(0x1038U, 6U),
    kCLOCK_PortB     = CLK_GATE_DEFINE(0x1038U, 7U),
    kCLOCK_PortC     = CLK_GATE_DEFINE(0x1038U, 8U),
    kCLOCK_PortD     = CLK_GATE_DEFINE(0x1038U, 9U),
    kCLOCK_PortE     = CLK_GATE_DEFINE(0x1038U, 10U),
    kCLOCK_PortF     = CLK_GATE_DEFINE(0x1038U, 11U),
    kCLOCK_PortG     = CLK_GATE_DEFINE(0x1038U, 12U),
    kCLOCK_PortH     = CLK_GATE_DEFINE(0x1038U, 13U),
    kCLOCK_PortI     = CLK_GATE_DEFINE(0x1038U, 14U),
    kCLOCK_Rtc0      = CLK_GATE_DEFINE(0x1038U, 16U),
    kCLOCK_Rtcreg    = CLK_GATE_DEFINE(0x1038U, 17U),
    kCLOCK_Xbar      = CLK_GATE_DEFINE(0x1038U, 21U),
    kCLOCK_Tmr0      = CLK_GATE_DEFINE(0x1038U, 23U),
    kCLOCK_Tmr1      = CLK_GATE_DEFINE(0x1038U, 24U),
    kCLOCK_Tmr2      = CLK_GATE_DEFINE(0x1038U, 25U),
    kCLOCK_Tmr3      = CLK_GATE_DEFINE(0x1038U, 26U),

    kCLOCK_Ftf0      = CLK_GATE_DEFINE(0x103CU, 0U),
    kCLOCK_Dmamux0   = CLK_GATE_DEFINE(0x103CU, 1U),
    kCLOCK_Rnga0     = CLK_GATE_DEFINE(0x103CU, 9U),
    kCLOCK_Lpuart0   = CLK_GATE_DEFINE(0x103CU, 10U),
    kCLOCK_Adc0      = CLK_GATE_DEFINE(0x103CU, 11U),
    kCLOCK_Pit0      = CLK_GATE_DEFINE(0x103CU, 13U),
    kCLOCK_Pit1      = CLK_GATE_DEFINE(0x103CU, 14U),
    kCLOCK_Afe0      = CLK_GATE_DEFINE(0x103CU, 16U),
    kCLOCK_Crc0      = CLK_GATE_DEFINE(0x103CU, 20U),
    kCLOCK_Pdb0      = CLK_GATE_DEFINE(0x103CU, 22U),
    kCLOCK_PortJ     = CLK_GATE_DEFINE(0x103CU, 23U),
    kCLOCK_PortK     = CLK_GATE_DEFINE(0x103CU, 24U),
    kCLOCK_PortL     = CLK_GATE_DEFINE(0x103CU, 25U),
    kCLOCK_PortM     = CLK_GATE_DEFINE(0x103CU, 26U),
    kCLOCK_Lptmr0    = CLK_GATE_DEFINE(0x103CU, 28U),

    kCLOCK_Mpu0      = CLK_GATE_DEFINE(0x1040U, 0U),
    kCLOCK_Dma0      = CLK_GATE_DEFINE(0x1040U, 1U),
    kCLOCK_Cau0      = CLK_GATE_DEFINE(0x1040U, 2U),
} clock_ip_name_t;

/*!@brief SIM configuration structure for clock setting. */
typedef struct _sim_clock_config
{
    uint8_t  pllFllSel;  /*!< PLL/FLL/IRC48M selection.    */
    uint8_t  er32kSrc;   /*!< ERCLK32K source selection.   */
    uint32_t clkdiv1;    /*!< SIM_CLKDIV1.                 */
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

/*! @brief Set LPUART0 clock source. */
static inline void CLOCK_SetLpuartClock(uint32_t src)
{
    SIM->CTRL_REG = ((SIM->CTRL_REG & ~SIM_CTRL_REG_LPUARTSRC_MASK) | SIM_CTRL_REG_LPUARTSRC(src));
}

/*! @brief Set XBAR clock source. */
static inline void CLOCK_SetXbarClock(uint32_t src)
{
    SIM->CTRL_REG = ((SIM->CTRL_REG & ~SIM_CTRL_REG_XBARCLKOUT_MASK) | SIM_CTRL_REG_XBARCLKOUT(src));
}

/*! @brief Set the clock selection of AFECLKSEL. */
static inline void CLOCK_SetAfeClkSrc(uint32_t src)
{
    SIM->MISC_CTL = ((SIM->MISC_CTL & ~SIM_MISC_CTL_AFECLKSEL_MASK) | SIM_MISC_CTL_AFECLKSEL(src));
}

/*! @brief Set PLLFLLSEL clock source. */
static inline void CLOCK_SetPllFllSelClock(uint32_t src)
{
    SIM->CTRL_REG = ((SIM->CTRL_REG & ~SIM_CTRL_REG_PLLFLLSEL_MASK) | SIM_CTRL_REG_PLLFLLSEL(src));
}

/*! @brief Set CLKOUT source. */
static inline void CLOCK_SetClkOutClock(uint32_t src)
{
    SIM->CTRL_REG = ((SIM->CTRL_REG & ~SIM_CTRL_REG_CLKOUT_MASK) | SIM_CTRL_REG_CLKOUT(src));
}

/*! @brief Set ADC trigger clock source. */
static inline void CLOCK_SetAdcTriggerClock(uint32_t src)
{
    SIM->CTRL_REG = ((SIM->CTRL_REG & ~SIM_CTRL_REG_ADCTRGSEL_MASK) | SIM_CTRL_REG_ADCTRGSEL(src));
}

/*!
 * @brief
 * Set the SIM_CLKDIV1[OUTDIV1], SIM_CLKDIV1[OUTDIV2], SIM_CLKDIV1[OUTDIV3], SIM_CLKDIV1[OUTDIV4].
 */
static inline void CLOCK_SetOutDiv(uint32_t sysClk,
                                   uint32_t busClk,
                                   uint32_t flashClk)
{
    SIM->CLKDIV1 = SIM_CLKDIV1_CLKDIVSYS(sysClk) |
                   SIM_CLKDIV1_CLKDIVBUS(busClk) |
                   SIM_CLKDIV1_FLASHCLKMODE(flashClk);
}

/*!
 * @brief Gets the clock frequency for AFE module.
 *
 * This function checks the current mode configurations in MISC_CTL register.
 *
 * @return Clock frequency value in Hertz
 */
uint32_t CLOCK_GetAfeFreq(void);

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
    SIM->CLKDIV1 = 0x03000000U;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _FSL_CLOCK_H_ */
