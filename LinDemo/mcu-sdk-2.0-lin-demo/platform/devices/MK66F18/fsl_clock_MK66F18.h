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

#ifndef _FSL_CLOCK_MK66F18_H_
#define _FSL_CLOCK_MK66F18_H_

#include <stdint.h>
#include <stdbool.h>
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Clock ip name array for DMAMUX. */
#define DMAMUX_CLOCKS {kCLOCK_Dmamux0}

/*! @brief Clock ip name array for RTC. */
#define RTC_CLOCKS {kCLOCK_Rtc0}

/*! @brief Clock ip name array for ENET. */
#define ENET_CLOCKS {kCLOCK_Enet0}

/*! @brief Clock ip name array for PORT. */
#define PORT_CLOCKS {kCLOCK_PortA, kCLOCK_PortB, kCLOCK_PortC, kCLOCK_PortD, kCLOCK_PortE}

/*! @brief Clock ip name array for SAI. */
#define SAI_CLOCKS {kCLOCK_Sai0}

/*! @brief Clock ip name array for FLEXBUS. */
#define FLEXBUS_CLOCKS {kCLOCK_Flexbus0}

/*! @brief Clock ip name array for TSI. */
#define TSI_CLOCKS {kCLOCK_Tsi0}

/*! @brief Clock ip name array for LPUART. */
#define LPUART_CLOCKS {kCLOCK_Lpuart0}

/*! @brief Clock ip name array for EWM. */
#define EWM_CLOCKS {kCLOCK_Ewm0}

/*! @brief Clock ip name array for PIT. */
#define PIT_CLOCKS {kCLOCK_Pit0}

/*! @brief Clock ip name array for DSPI. */
#define DSPI_CLOCKS {kCLOCK_Spi0, kCLOCK_Spi1, kCLOCK_Spi2}

/*! @brief Clock ip name array for LPTMR. */
#define LPTMR_CLOCKS {kCLOCK_Lptmr0}

/*! @brief Clock ip name array for SDHC. */
#define SDHC_CLOCKS {kCLOCK_Sdhc0}

/*! @brief Clock ip name array for FTM. */
#define FTM_CLOCKS {kCLOCK_Ftm0, kCLOCK_Ftm1, kCLOCK_Ftm2, kCLOCK_Ftm3}

/*! @brief Clock ip name array for EDMA. */
#define EDMA_CLOCKS {kCLOCK_Dma0}

/*! @brief Clock ip name array for FLEXCAN. */
#define FLEXCAN_CLOCKS {kCLOCK_Flexcan0, kCLOCK_Flexcan1}

/*! @brief Clock ip name array for DAC. */
#define DAC_CLOCKS {kCLOCK_Dac0, kCLOCK_Dac1}

/*! @brief Clock ip name array for ADC16. */
#define ADC16_CLOCKS {kCLOCK_Adc0, kCLOCK_Adc1}

/*! @brief Clock ip name array for SDRAM. */
#define SDRAM_CLOCKS {kCLOCK_Sdramc0}

/*! @brief Clock ip name array for MMCAU. */
#define MMCAU_CLOCKS {kCLOCK_Mmcau0}

/*! @brief Clock ip name array for MPU. */
#define MPU_CLOCKS {kCLOCK_Mpu0}

/*! @brief Clock ip name array for VREF. */
#define VREF_CLOCKS {kCLOCK_Vref0}

/*! @brief Clock ip name array for CMT. */
#define CMT_CLOCKS {kCLOCK_Cmt0}

/*! @brief Clock ip name array for UART. */
#define UART_CLOCKS {kCLOCK_Uart0, kCLOCK_Uart1, kCLOCK_Uart2, kCLOCK_Uart3, kCLOCK_Uart4}

/*! @brief Clock ip name array for TPM. */
#define TPM_CLOCKS {kCLOCK_Tpm0, kCLOCK_Tpm1, kCLOCK_Tpm2}

/*! @brief Clock ip name array for RNGA. */
#define RNGA_CLOCKS {kCLOCK_Rnga0}

/*! @brief Clock ip name array for CRC. */
#define CRC_CLOCKS {kCLOCK_Crc0}

/*! @brief Clock ip name array for LMEM. */
#define LMEM_CLOCKS {kCLOCK_Lmem0}

/*! @brief Clock ip name array for I2C. */
#define I2C_CLOCKS {kCLOCK_I2c0, kCLOCK_I2c1, kCLOCK_I2c2, kCLOCK_I2c3}

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

#define I2C0_CLK_SRC   BUS_CLK
#define I2C1_CLK_SRC   BUS_CLK
#define I2C2_CLK_SRC   BUS_CLK
#define I2C3_CLK_SRC   BUS_CLK
#define DSPI0_CLK_SRC  BUS_CLK
#define DSPI1_CLK_SRC  BUS_CLK
#define DSPI2_CLK_SRC  BUS_CLK
#define UART0_CLK_SRC  SYS_CLK
#define UART1_CLK_SRC  SYS_CLK
#define UART2_CLK_SRC  BUS_CLK
#define UART3_CLK_SRC  BUS_CLK
#define UART4_CLK_SRC  BUS_CLK

/*! @brief Clock name used to get clock frequency. */
typedef enum _clock_name
{

    /* ----------------------------- System layer clock -------------------------------*/
    kCLOCK_CoreSysClk,        /*!< Core/system clock                                         */
    kCLOCK_PlatClk,           /*!< Platform clock                                            */
    kCLOCK_BusClk,            /*!< Bus clock                                                 */
    kCLOCK_FlexBusClk,        /*!< FlexBus clock                                             */
    kCLOCK_FlashClk,          /*!< Flash clock                                               */
    kCLOCK_FastPeriphClk,     /*!< Fast peripheral clock                                     */
    kCLOCK_PllFllSelClk,      /*!< The clock after SIM[PLLFLLSEL].                           */

    /* ---------------------------------- OSC clock -----------------------------------*/
    kCLOCK_Er32kClk,          /*!< External reference 32K clock (ERCLK32K)                   */
    kCLOCK_Osc0ErClk,         /*!< OSC0 external reference clock (OSC0ERCLK)                 */
    kCLOCK_Osc1ErClk,         /*!< OSC1 external reference clock (OSC1ERCLK)                 */
    kCLOCK_Osc0ErClkUndiv,    /*!< OSC0 external reference undivided clock(OSC0ERCLK_UNDIV). */

    /* ----------------------------- MCG and MCG-Lite clock ---------------------------*/
    kCLOCK_McgFixedFreqClk,   /*!< MCG fixed frequency clock (MCGFFCLK)                      */
    kCLOCK_McgInternalRefClk, /*!< MCG internal reference clock (MCGIRCLK)                   */
    kCLOCK_McgFllClk,         /*!< MCGFLLCLK                                                 */
    kCLOCK_McgPll0Clk,        /*!< MCGPLL0CLK                                                */
    kCLOCK_McgPll1Clk,        /*!< MCGPLL1CLK                                                */
    kCLOCK_McgExtPllClk,      /*!< EXT_PLLCLK                                                */
    kCLOCK_McgPeriphClk,      /*!< MCG peripheral clock (MCGPCLK)                            */
    kCLOCK_McgIrc48MClk,      /*!< MCG IRC48M clock                                          */

    /* --------------------------------- Other clock ----------------------------------*/
    kCLOCK_LpoClk,            /*!< LPO clock                                                 */

} clock_name_t;

/*! @brief USB clock source definition. */
typedef enum _clock_usb_src
{
     kCLOCK_UsbSrcPll0   = SIM_SOPT2_USBSRC(1U) | SIM_SOPT2_PLLFLLSEL(1U), /*!< Use PLL0.      */
     kCLOCK_UsbSrcUsbPfd = SIM_SOPT2_USBSRC(1U) | SIM_SOPT2_PLLFLLSEL(2U), /*!< Use USBPFDCLK. */
     kCLOCK_UsbSrcIrc48M = SIM_SOPT2_USBSRC(1U) | SIM_SOPT2_PLLFLLSEL(3U), /*!< Use IRC48M.    */
     kCLOCK_UsbSrcExt    = SIM_SOPT2_USBSRC(0U)                            /*!< Use USB_CLKIN. */
} clock_usb_src_t;

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
    kCLOCK_I2c2      = CLK_GATE_DEFINE(0x1028U, 6U),
    kCLOCK_I2c3      = CLK_GATE_DEFINE(0x1028U, 7U),
    kCLOCK_Uart4     = CLK_GATE_DEFINE(0x1028U, 10U),

    kCLOCK_Enet0     = CLK_GATE_DEFINE(0x102CU, 0U),
    kCLOCK_Lpuart0   = CLK_GATE_DEFINE(0x102CU, 4U),
    kCLOCK_Tpm1      = CLK_GATE_DEFINE(0x102CU, 9U),
    kCLOCK_Tpm2      = CLK_GATE_DEFINE(0x102CU, 10U),
    kCLOCK_Dac0      = CLK_GATE_DEFINE(0x102CU, 12U),
    kCLOCK_Dac1      = CLK_GATE_DEFINE(0x102CU, 13U),

    kCLOCK_Rnga0     = CLK_GATE_DEFINE(0x1030U, 0U),
    kCLOCK_Usbhs0    = CLK_GATE_DEFINE(0x1030U, 1U),
    kCLOCK_UsbhsPhy0 = CLK_GATE_DEFINE(0x1030U, 2U),
    kCLOCK_UsbhsDcd0 = CLK_GATE_DEFINE(0x1030U, 3U),
    kCLOCK_Flexcan1  = CLK_GATE_DEFINE(0x1030U, 4U),
    kCLOCK_Spi2      = CLK_GATE_DEFINE(0x1030U, 12U),
    kCLOCK_Sdhc0     = CLK_GATE_DEFINE(0x1030U, 17U),
    kCLOCK_Ftm3      = CLK_GATE_DEFINE(0x1030U, 25U),
    kCLOCK_Adc1      = CLK_GATE_DEFINE(0x1030U, 27U),

    kCLOCK_Ewm0      = CLK_GATE_DEFINE(0x1034U, 1U),
    kCLOCK_Cmt0      = CLK_GATE_DEFINE(0x1034U, 2U),
    kCLOCK_I2c0      = CLK_GATE_DEFINE(0x1034U, 6U),
    kCLOCK_I2c1      = CLK_GATE_DEFINE(0x1034U, 7U),
    kCLOCK_Uart0     = CLK_GATE_DEFINE(0x1034U, 10U),
    kCLOCK_Uart1     = CLK_GATE_DEFINE(0x1034U, 11U),
    kCLOCK_Uart2     = CLK_GATE_DEFINE(0x1034U, 12U),
    kCLOCK_Uart3     = CLK_GATE_DEFINE(0x1034U, 13U),
    kCLOCK_Usbfs0    = CLK_GATE_DEFINE(0x1034U, 18U),
    kCLOCK_Cmp0      = CLK_GATE_DEFINE(0x1034U, 19U),
    kCLOCK_Cmp1      = CLK_GATE_DEFINE(0x1034U, 19U),
    kCLOCK_Cmp2      = CLK_GATE_DEFINE(0x1034U, 19U),
    kCLOCK_Vref0     = CLK_GATE_DEFINE(0x1034U, 20U),

    kCLOCK_Lptmr0    = CLK_GATE_DEFINE(0x1038U, 0U),
    kCLOCK_Tsi0      = CLK_GATE_DEFINE(0x1038U, 5U),
    kCLOCK_PortA     = CLK_GATE_DEFINE(0x1038U, 9U),
    kCLOCK_PortB     = CLK_GATE_DEFINE(0x1038U, 10U),
    kCLOCK_PortC     = CLK_GATE_DEFINE(0x1038U, 11U),
    kCLOCK_PortD     = CLK_GATE_DEFINE(0x1038U, 12U),
    kCLOCK_PortE     = CLK_GATE_DEFINE(0x1038U, 13U),

    kCLOCK_Ftf0      = CLK_GATE_DEFINE(0x103CU, 0U),
    kCLOCK_Dmamux0   = CLK_GATE_DEFINE(0x103CU, 1U),
    kCLOCK_Flexcan0  = CLK_GATE_DEFINE(0x103CU, 4U),
    kCLOCK_Spi0      = CLK_GATE_DEFINE(0x103CU, 12U),
    kCLOCK_Spi1      = CLK_GATE_DEFINE(0x103CU, 13U),
    kCLOCK_Sai0      = CLK_GATE_DEFINE(0x103CU, 15U),
    kCLOCK_Crc0      = CLK_GATE_DEFINE(0x103CU, 18U),
    kCLOCK_Usbdcd0   = CLK_GATE_DEFINE(0x103CU, 21U),
    kCLOCK_Pdb0      = CLK_GATE_DEFINE(0x103CU, 22U),
    kCLOCK_Pit0      = CLK_GATE_DEFINE(0x103CU, 23U),
    kCLOCK_Ftm0      = CLK_GATE_DEFINE(0x103CU, 24U),
    kCLOCK_Ftm1      = CLK_GATE_DEFINE(0x103CU, 25U),
    kCLOCK_Ftm2      = CLK_GATE_DEFINE(0x103CU, 26U),
    kCLOCK_Adc0      = CLK_GATE_DEFINE(0x103CU, 27U),
    kCLOCK_Rtc0      = CLK_GATE_DEFINE(0x103CU, 29U),

    kCLOCK_Flexbus0  = CLK_GATE_DEFINE(0x1040U, 0U),
    kCLOCK_Dma0      = CLK_GATE_DEFINE(0x1040U, 1U),
    kCLOCK_Mpu0      = CLK_GATE_DEFINE(0x1040U, 2U),
    kCLOCK_Sdramc0   = CLK_GATE_DEFINE(0x1040U, 3U),
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

/*! @brief Set SDHC0 clock source. */
static inline void CLOCK_SetSdhc0Clock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_SDHCSRC_MASK) | SIM_SOPT2_SDHCSRC(src));
}

/*! @brief Set LPUART clock source. */
static inline void CLOCK_SetLpuartClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_LPUARTSRC_MASK) | SIM_SOPT2_LPUARTSRC(src));
}

/*! @brief Set TPM clock source. */
static inline void CLOCK_SetTpmClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_TPMSRC_MASK) | SIM_SOPT2_TPMSRC(src));
}

/*! @brief Set debug trace clock source. */
static inline void CLOCK_SetTraceClock(uint32_t src, uint32_t divValue, uint32_t fracValue)
{
    SIM->SOPT2   = ((SIM->SOPT2 & ~SIM_SOPT2_TRACECLKSEL_MASK) | SIM_SOPT2_TRACECLKSEL(src));
    SIM->CLKDIV4 = SIM_CLKDIV4_TRACEDIV(divValue) | SIM_CLKDIV4_TRACEFRAC(fracValue);
}

/*! @brief Set PLLFLLSEL clock source. */
static inline void CLOCK_SetPllFllSelClock(uint32_t src, uint32_t divValue, uint32_t fracValue)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_PLLFLLSEL_MASK) | SIM_SOPT2_PLLFLLSEL(src));
    SIM->CLKDIV3 = SIM_CLKDIV3_PLLFLLDIV(divValue) | SIM_CLKDIV3_PLLFLLFRAC(fracValue);
}

/*! @brief Set CLKOUT source. */
static inline void CLOCK_SetClkOutClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_CLKOUTSEL_MASK) | SIM_SOPT2_CLKOUTSEL(src));
}

/*! @brief Set RTC_CLKOUT source. */
static inline void CLOCK_SetRtcClkOutClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_RTCCLKOUTSEL_MASK) | SIM_SOPT2_RTCCLKOUTSEL(src));
}

/*! @brief Enable USB HS clock.
 *
 * @param src  USB HS clock source.
 * @param freq The frequency specified by src.
 * @retval true The clock is set successfully.
 * @retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhs0Clock(clock_usb_src_t src, uint32_t freq);

/*! @brief Disable USB HS clock.
 *
 * Disable USB HS clock.
 */
static inline void CLOCK_DisableUsbhs0Clock(void)
{
    SIM->SOPT2 &= ~SIM_SOPT2_USBREGEN_MASK;
    SIM->SCGC3 &= ~(SIM_SCGC3_USBHS_MASK | SIM_SCGC3_USBHSPHY_MASK);
}

/*! @brief Enable USB FS clock.
 *
 * @param src  USB FS clock source.
 * @param freq The frequency specified by src.
 * @retval true The clock is set successfully.
 * @retval false The clock source is invalid to get proper USB FS clock.
 */
bool CLOCK_EnableUsbfs0Clock(clock_usb_src_t src, uint32_t freq);

/*! @brief Disable USB FS clock.
 *
 * Disable USB FS clock.
 */
static inline void CLOCK_DisableUsbfs0Clock(void)
{
    CLOCK_DisableClock(kCLOCK_Usbfs0);
}

/*!
 * @brief
 * Set the SIM_CLKDIV1[OUTDIV1], SIM_CLKDIV1[OUTDIV2], SIM_CLKDIV1[OUTDIV3], SIM_CLKDIV1[OUTDIV4].
 */
static inline void CLOCK_SetOutDiv(uint32_t outdiv1,
                                   uint32_t outdiv2,
                                   uint32_t outdiv3,
                                   uint32_t outdiv4)
{
    SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) |
                   SIM_CLKDIV1_OUTDIV2(outdiv2) |
                   SIM_CLKDIV1_OUTDIV3(outdiv3) |
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
    SIM->CLKDIV1 = 0x02260000U;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _FSL_CLOCK_H_ */
