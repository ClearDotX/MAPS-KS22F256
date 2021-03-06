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

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Initialize all pins used in this example
 *
 * @param disablePortClockAfterInit disable port clock after pin
 * initialization or not.
 */
void BOARD_InitPins(void)
{
    /* Disable NMI */
    CLOCK_EnableClock(kCLOCK_PortA);
    PORT_SetPinMux(PORTA, 4u, kPORT_PinDisabledOrAnalog);
    CLOCK_DisableClock(kCLOCK_PortA);
    /* Un-gate the port clock */
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);
    /* Initialize UART1 pins below */
    /* Affects PORTE_PCR0 register */
    PORT_SetPinMux(PORTE, 0u, kPORT_MuxAlt3);
    /* Affects PORTE_PCR1 register */
    PORT_SetPinMux(PORTE, 1u, kPORT_MuxAlt3);

    /* Initialize SMARTCARD pins below */
    /* Initialize UART0 IO as GPIO at first */
    PORT_SetPinMux(PORTA, 2u, kPORT_MuxAsGpio);
    GPIOA->PDDR |= (1u << 2u);
    /* Initialize card clock */
    port_pin_config_t config = {0};
    config.driveStrength = kPORT_HighDriveStrength;
    config.mux = kPORT_MuxAlt6;
    PORT_SetPinConfig(PORTA, 1u, &config);
    /* Reset */
    config.pullSelect = kPORT_PullDown;
    config.driveStrength = kPORT_HighDriveStrength;
    config.mux = kPORT_MuxAsGpio;
    PORT_SetPinConfig(PORTC, 10u, &config);
    /* Control/CMDVCC */
    /* Disable VCC during init */
    GPIOC->PSOR |= (1u << 11);
    GPIOC->PDDR |= (1u << 11);
    config.pullSelect = kPORT_PullUp;
    config.driveStrength = kPORT_HighDriveStrength;
    config.mux = kPORT_MuxAsGpio;
    PORT_SetPinConfig(PORTC, 11u, &config);
}
