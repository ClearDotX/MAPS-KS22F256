/* ###################################################################
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
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
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file pin_mux.c
** @version 1.2
** @brief
**
*/         
/*!
**  @addtogroup pin_mux_module pin_mux module documentation
**  @{
*/         

#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_clock_manager.h"
#include "fsl_mcg_hal.h"
#include "fsl_debug_console.h"
#include "fsl_smc_hal.h"
#include "pin_mux.h"
#include "app_flexio_ov7670.h"
#include <stdbool.h>

static void App_ClockInit(void);

void hardware_init(void) 
{
  mcg_modes_t mcg_mode_test;

  /* enable clock for PORTs */
  CLOCK_SYS_EnablePortClock(PORTA_IDX);
  CLOCK_SYS_EnablePortClock(PORTB_IDX);
  CLOCK_SYS_EnablePortClock(PORTC_IDX);
  CLOCK_SYS_EnablePortClock(PORTD_IDX);
  CLOCK_SYS_EnablePortClock(PORTE_IDX);
  
  /* Enable clock out pin to OSCERCLK */
  CLOCK_HAL_SetClkOutSel(SIM, kClockClkoutSelOsc0erClk);
  
  /* Enable clock gate to I2C2 module*/
  SIM_HAL_EnableClock(SIM, kSimClockGateI2c2);
  
  /* Enable clock gate to DMA module*/
  SIM_HAL_EnableClock(SIM, kSimClockGateDmamux0);
  SIM_HAL_EnableClock(SIM, kSimClockGateDma0);
  
  /* Enable clock gate to FlexIO module*/
  CLOCK_HAL_SetFlexioSrc(SIM, 0, kClockFlexioSrcPllFllSelDiv);
  CLOCK_HAL_SetPllfllSel(SIM, kClockPllFllSelPll);
  CLOCK_HAL_SetPllFllDiv(SIM, 0, 0);
  SIM_HAL_EnableClock(SIM, kSimClockGateFlexio0);
  
  /* Enable clock gate to FlexIO module*/
  SIM_HAL_SetFlexbusSecurityLevelMode(SIM, kSimFbslLevel3);
  SIM_HAL_EnableClock(SIM, kSimClockGateFlexbus0);
  
  /* Init board clock */
  App_ClockInit();
  mcg_mode_test = (CLOCK_HAL_GetMcgMode(MCG));
  while(mcg_mode_test != kMcgModePEE);
}

static void App_ClockInit(void)
{
  SMC_HAL_SetProtection(SMC, kAllowPowerModeVlp);
  PORT_HAL_SetMuxMode(EXTAL0_PORT, EXTAL0_PIN, EXTAL0_PINMUX);
  PORT_HAL_SetMuxMode(XTAL0_PORT, XTAL0_PIN, XTAL0_PINMUX);
  BOARD_InitOsc0();
  CLOCK_SYS_SetConfiguration(&g_RunClockConfig);
  CLOCK_HAL_SetPll0EnableInStopCmd(MCG, g_RunClockConfig.mcgConfig.pll0EnableInStop);
}

/***************************************************************************//*!
 * @brief   GPIO pins initialization
 * @details Function provided configuration of VSYNC pin as GPIO input with
 *          falling edge interrupt enabled.
 ******************************************************************************/
void configure_app_gpio_pins(void)
{
  PORT_HAL_SetMuxMode(PORTA,13u,kPortMuxAsGpio); ///< VSYNC
  PORT_HAL_SetPullMode(PORTA,13u,kPortPullUp);
  PORT_HAL_SetPullCmd(PORTA,13u,true);
  PORT_HAL_SetPinIntMode(PORTA,13u,kPortIntRisingEdge);
}

/***************************************************************************//*!
 * @brief   CLKOUT initialization 
 * @details Function enables PTC3 pin as CLOKOUT with OSCERCLK (12MHz). This pin 
 *          is used as source clock for OV7670 camera (XCLK pin).
 ******************************************************************************/
void configure_clkout_pin(void)
{
  /* Affects PORTC_PCR3 register */
  PORT_HAL_SetMuxMode(PORTC,3u,kPortMuxAlt5);   ///< CLKOUT -> OV7670_XCLK -> 12MHz
}

/***************************************************************************//*!
 * @brief   I2C2 pins initialization
 * @details Function provided configuration of SDA and SCL pins of I2C2 module.
 * @param   void
 * @return  void
 * @note    Non.
 * @warning Non.
 ******************************************************************************/
void configure_app_i2c_pins(void)
{
  // Configure I2C2 pins
  PORT_HAL_SetMuxMode(PORTA,10UL,kPortMuxAlt2); ///< I2C2 SDA, SIO_D
  PORT_HAL_SetOpenDrainCmd(PORTA,10UL,true);
  PORT_HAL_SetMuxMode(PORTA,11UL,kPortMuxAlt2); ///< I2C2 SCL, SIO_C
  PORT_HAL_SetOpenDrainCmd(PORTA,11UL,true);
}

/***************************************************************************//*!
 * @brief   GPIO pins initialization
 * @details Function provided configuration of FlexIO pins.
 * @param   void
 * @return  void
 * @note    Non.
 * @warning Non.
 ******************************************************************************/
void configure_app_flexio_pins(void)
{
  // Configure FlexIO pins
  PORT_HAL_SetMuxMode(PORTB,1u,kPortMuxAlt7); ///< FXIO_D1 -> OV7670_PCLK
  PORT_HAL_SetMuxMode(PORTA,12u,kPortMuxAlt5); ///< FXIO_D18 -> OV7670_HREF
  
  PORT_HAL_SetMuxMode(PORTD,8u,kPortMuxAlt7); ///< FXIO_D24 -> OV7670_D0
  PORT_HAL_SetMuxMode(PORTD,9u,kPortMuxAlt7); ///< FXIO_D25 -> OV7670_D1
  PORT_HAL_SetMuxMode(PORTD,10u,kPortMuxAlt7); ///< FXIO_D26 -> OV7670_D2
  PORT_HAL_SetMuxMode(PORTD,11u,kPortMuxAlt7); ///< FXIO_D27 -> OV7670_D3
  PORT_HAL_SetMuxMode(PORTD,12u,kPortMuxAlt7); ///< FXIO_D28 -> OV7670_D4 
  PORT_HAL_SetMuxMode(PORTD,13u,kPortMuxAlt7); ///< FXIO_D29 -> OV7670_D5
  PORT_HAL_SetMuxMode(PORTD,14u,kPortMuxAlt7); ///< FXIO_D30 -> OV7670_D6
  PORT_HAL_SetMuxMode(PORTD,15u,kPortMuxAlt7); ///< FXIO_D31 -> OV7670_D7
}

/***************************************************************************//*!
 * @brief   FlexBUS pins initialization
 * @details Function provided configuration of FlexBUS pins.
 * @param   void
 * @return  void
 * @note    Non.
 * @warning Non.
 ******************************************************************************/
void configure_app_flexbus_pins(void)
{
  // Configure Flexbus pins
  PORT_HAL_SetMuxMode(PORTD,1u,kPortMuxAlt5); ///< FB_CS0
  PORT_HAL_SetMuxMode(PORTD,0u,kPortMuxAlt5); ///< FB_ALE
  PORT_HAL_SetMuxMode(PORTC,11u,kPortMuxAlt5); ///< FB_RW
  
  PORT_HAL_SetMuxMode(PORTD,6u,kPortMuxAlt5); ///< FB_AD0
  PORT_HAL_SetMuxMode(PORTD,5u,kPortMuxAlt5); ///< FB_AD1
  PORT_HAL_SetMuxMode(PORTD,4u,kPortMuxAlt5); ///< FB_AD2
  PORT_HAL_SetMuxMode(PORTD,3u,kPortMuxAlt5); ///< FB_AD3
  PORT_HAL_SetMuxMode(PORTD,2u,kPortMuxAlt5); ///< FB_AD4
  PORT_HAL_SetMuxMode(PORTC,10u,kPortMuxAlt5); ///< FB_AD5
  PORT_HAL_SetMuxMode(PORTC,9u,kPortMuxAlt5); ///< FB_AD6
  PORT_HAL_SetMuxMode(PORTC,8u,kPortMuxAlt5); ///< FB_AD7
  PORT_HAL_SetMuxMode(PORTC,7u,kPortMuxAlt5); ///< FB_AD8
  PORT_HAL_SetMuxMode(PORTC,6u,kPortMuxAlt5); ///< FB_AD9
  PORT_HAL_SetMuxMode(PORTC,5u,kPortMuxAlt5); ///< FB_AD10
  PORT_HAL_SetMuxMode(PORTC,4u,kPortMuxAlt5); ///< FB_AD11
  PORT_HAL_SetMuxMode(PORTC,2u,kPortMuxAlt5); ///< FB_AD12
  PORT_HAL_SetMuxMode(PORTC,1u,kPortMuxAlt5); ///< FB_AD13
  PORT_HAL_SetMuxMode(PORTC,0u,kPortMuxAlt5); ///< FB_AD14
  PORT_HAL_SetMuxMode(PORTB,18u,kPortMuxAlt5); ///< FB_AD15
  PORT_HAL_SetMuxMode(PORTB,17u,kPortMuxAlt5); ///< FB_AD16   
}

/*!
** @}
*/
/*
*/
