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

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Initialize all pins used in this example
 *
 * initialization or not.
 */

port_pin_config_t i2c_pin_config = {0};

void BOARD_InitPins(void)
{
    /* Disalbe NMI */
    CLOCK_EnableClock(kCLOCK_PortA);
    PORT_SetPinMux(PORTA,4u,kPORT_PinDisabledOrAnalog);
    CLOCK_DisableClock(kCLOCK_PortA);
 
    /* Initialize UART0 pins below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortE);

    /* Affects PORTE_PCR0 register */
    PORT_SetPinMux(PORTE, 0u, kPORT_MuxAlt3);
    /* Affects PORTE_PCR1 register */
    PORT_SetPinMux(PORTE, 1u, kPORT_MuxAlt3);
    
    /* Configure sai pins */
    /* Tx BCLK */
    PORT_SetPinMux(PORTB,20u,kPORT_MuxAlt7);
    /* Tx Frame sync */
    PORT_SetPinMux(PORTB,21u,kPORT_MuxAlt6);
    /* Tx data */
    PORT_SetPinMux(PORTB,22u,kPORT_MuxAlt6);
    
        /* Master clock */
    PORT_SetPinMux(PORTA,17u,kPORT_MuxAlt6);

    /* Configure I2C0 */
    i2c_pin_config.pullSelect = kPORT_PullUp;
#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
    i2c_pin_config.openDrainEnable = kPORT_OpenDrainEnable;
    i2c_pin_config.mux = kPORT_MuxAlt2;
#endif /* FSL_FEATURE_PORT_HAS_OPEN_DRAIN */
    /* Affects PORTE_PCR0 register */
    PORT_SetPinConfig(PORTB, 0u, &i2c_pin_config);
    /* Affects PORTE_PCR1 register */
    PORT_SetPinConfig(PORTB, 1u, &i2c_pin_config);
}

