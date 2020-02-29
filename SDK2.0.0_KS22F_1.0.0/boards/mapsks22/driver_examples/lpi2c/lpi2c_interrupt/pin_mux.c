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
#include "fsl_common.h"

/*!
 * @brief Initialize all pins used in this example
 *
 * @param disablePortClockAfterInit disable port clock after pin
 * initialization or not.
 */
void BOARD_InitPins()
{
    /* I2C pin config */
    port_pin_config_t i2cConfig = {0};

    /* Disable NMI */
    CLOCK_EnableClock(kCLOCK_PortA);
    PORT_SetPinMux(PORTA, 4U, kPORT_PinDisabledOrAnalog);
    CLOCK_DisableClock(kCLOCK_PortA);

    /* Configure LPUART pins */
    CLOCK_EnableClock(kCLOCK_PortE);
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3);

    i2cConfig.pullSelect = kPORT_PullUp;
    i2cConfig.openDrainEnable = true;
    i2cConfig.mux = kPORT_MuxAlt2;

    /* LPI2C0 */
    CLOCK_EnableClock(kCLOCK_PortB);
    PORT_SetPinConfig(PORTB, 0U, &i2cConfig);
    PORT_SetPinConfig(PORTB, 1U, &i2cConfig);

    /* LPI2C1 */
    CLOCK_EnableClock(kCLOCK_PortC);
    PORT_SetPinConfig(PORTC, 10U, &i2cConfig);
    PORT_SetPinConfig(PORTC, 11U, &i2cConfig);
}
