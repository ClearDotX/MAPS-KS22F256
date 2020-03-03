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

#include "fsl_device_registers.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "board.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
port_pin_config_t g_UART_TX_PinConfig = {
    kPORT_PullDown,
    kPORT_PullEnable,
    kPORT_FastSlewRate,
    0,
    kPORT_PassiveFilterDisable,
#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
    kPORT_OpenDrainDisable,
#else
    0,
#endif /* FSL_FEATURE_PORT_HAS_OPEN_DRAIN */
    kPORT_HighDriveStrength,
    0,
    kPORT_MuxAlt7,
    0,
#if defined(FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK) && FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK
    kPORT_UnLockRegister,
#else
    0,
#endif /* FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK */
};

port_pin_config_t g_UART_RX_PinConfig = {
    kPORT_PullDown,
    kPORT_PullEnable,
    kPORT_FastSlewRate,
    0,
    kPORT_PassiveFilterDisable,
#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
    kPORT_OpenDrainDisable,
#else
    0,
#endif /* FSL_FEATURE_PORT_HAS_OPEN_DRAIN */
    kPORT_HighDriveStrength,
    0,
    kPORT_MuxAlt7,
    0,
#if defined(FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK) && FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK
    kPORT_UnLockRegister,
#else
    0,
#endif /* FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK */
};

gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput, 0,
    };

/*!
 * @brief Initialize all pins used in this example
 *
 * @param disablePortClockAfterInit disable port clock after pin
 * initialization or not.
 */
void BOARD_InitPins(void)
{
    /* Initialize UART0 pins below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortB);

    /* Affects PORTB_PCR0 register */
    PORT_SetPinMux(PORTB, 0u, kPORT_MuxAlt7);
    /* Affects PORTB_PCR1 register */
    PORT_SetPinMux(PORTB, 1u, kPORT_MuxAlt7);
    /* LED 0 - 3, PTB3, PTB9, PTB10, PTB11 */
    PORT_SetPinMux(BOARD_LED_RED1_GPIO_PORT, 3u, kPORT_MuxAsGpio);
    PORT_SetPinMux(BOARD_LED_RED2_GPIO_PORT, 9u, kPORT_MuxAsGpio);
    PORT_SetPinMux(BOARD_LED_RED3_GPIO_PORT, 10u, kPORT_MuxAsGpio);
    PORT_SetPinMux(BOARD_LED_GREEN_GPIO_PORT, 11u, kPORT_MuxAsGpio);
    LED_RED1_INIT(LOGIC_LED_OFF);
    LED_RED2_INIT(LOGIC_LED_OFF);
    LED_RED3_INIT(LOGIC_LED_OFF);
    LED_GREEN_INIT(LOGIC_LED_OFF);
}
