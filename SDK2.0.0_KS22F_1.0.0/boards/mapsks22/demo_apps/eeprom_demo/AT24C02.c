/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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
 *
 */

#include "AT24C02.h"
#include "fsl_lpi2c.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* lpi2c handler */
lpi2c_master_handle_t g_m_handle;

/*******************************************************************************
 * Code
 ******************************************************************************/

at24c02_status_t AT24C02_I2CInit(at24c02_handler_t *handler)
{
    /* i2c master init */
    uint32_t sourceClock = 0;
    lpi2c_master_config_t masterConfig;

    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = AT24C02_I2C_BAUDRATE;
    sourceClock = CLOCK_GetFreq(AT24C02_I2C_CLK_SRC);
    LPI2C_MasterInit(handler->i2cBase, &masterConfig, sourceClock);
    return kStatus_AT24C02_Success;
}

at24c02_status_t AT24C02_ByteWrite(at24c02_handler_t *handler, uint8_t address, uint8_t data)
{   
    LPI2C_MasterStart(AT24C02_I2C_BASE,handler->device_addr,kLPI2C_Write);
    
    LPI2C_MasterSend(AT24C02_I2C_BASE,&address,1);

    LPI2C_MasterSend(AT24C02_I2C_BASE,&data,1);
    
    LPI2C_MasterStop(AT24C02_I2C_BASE);

    return kStatus_AT24C02_Success;
}

at24c02_status_t AT24C02_ACKPoll(at24c02_handler_t *handler)
{    
    uint32_t delayForTwr = CLOCK_GetFreq(kCLOCK_CoreSysClk)/200; /* Twr time is 5ms*/
    /* Used for the Twr time: 5ms */
    for(int32_t i=0;i<delayForTwr;i++)
    {
        __NOP();
    }

    return kStatus_AT24C02_Success;
}

at24c02_status_t AT24C02_ByteRead(at24c02_handler_t *handler, uint8_t address, uint8_t *data)
{
    LPI2C_MasterStart(AT24C02_I2C_BASE,handler->device_addr,kLPI2C_Write);
    
    LPI2C_MasterSend(AT24C02_I2C_BASE,&address,1);
    
    LPI2C_MasterStart(AT24C02_I2C_BASE,handler->device_addr,kLPI2C_Read);
    
    LPI2C_MasterReceive(AT24C02_I2C_BASE,data,1);
    
    LPI2C_MasterStop(AT24C02_I2C_BASE);

    return kStatus_AT24C02_Success;
}
