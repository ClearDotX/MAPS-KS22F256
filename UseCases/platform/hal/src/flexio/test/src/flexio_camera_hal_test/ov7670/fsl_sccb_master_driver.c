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
 */

#include <assert.h>
#include <string.h>
#include "fsl_i2c_master_driver.h"
#include "fsl_i2c_shared_function.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"
#include "fsl_sccb_master_driver.h"

#if FSL_FEATURE_SOC_I2C_COUNT

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Default timeout time(ms) for sending of address and CMD buffer */
#define I2C_TIMEOUT_MS (30)

/*******************************************************************************
 * Private Functions
 ******************************************************************************/
static i2c_status_t SCCB_DRV_MasterWait(uint32_t instance, uint32_t timeout_ms);
static i2c_status_t SCCB_DRV_SendAddress(uint32_t instance,
                                        const i2c_device_t * device,
                                        const uint8_t * cmdBuff,
                                        uint32_t cmdSize,
                                        i2c_direction_t direction,
                                        uint32_t timeout_ms);
static i2c_status_t SCCB_DRV_MasterReceive(uint32_t instance,
                                          const i2c_device_t * device,
                                          const uint8_t * cmdBuff,
                                          uint32_t cmdSize,
                                          uint8_t * rxBuff,
                                          uint32_t rxSize,
                                          uint32_t timeout_ms,
                                          bool isBlocking);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_DRV_SendAddress
 * Description   : Prepare and send out address buffer with interrupt.
 * This function is a static function which will be called by other data
 * transaction APIs.
 *
 *END**************************************************************************/
static i2c_status_t SCCB_DRV_SendAddress(uint32_t instance,
                                        const i2c_device_t * device,
                                        const uint8_t * cmdBuff,
                                        uint32_t cmdSize,
                                        i2c_direction_t direction,
                                        uint32_t timeout_ms)
{
    assert(instance < I2C_INSTANCE_COUNT);

    I2C_Type * base = g_i2cBase[instance];
    /* Get current runtime structure. */
    i2c_master_state_t * master = (i2c_master_state_t *)g_i2cStatePtr[instance];

    uint8_t addrByte1, addrByte2, directionBit;
    bool is10bitAddr;
    uint8_t addrBuff[2] = {0};
    uint8_t addrSize = 0;
    bool isMainXferBlocking = master->isBlocking;

    /* Send of address and CMD must be blocking without STOP */
    master->isRequesting = true;
    master->isBlocking = true;

    /*--------------- Prepare Address Buffer ------------------*/
    /* Get r/w bit according to required direction.
     * read is 1, write is 0. */
    directionBit = (direction == kI2CReceive) ? 0x1U : 0x0U;

    /* Check to see if slave address is 10 bits or not. */
    is10bitAddr = ((device->address >> 10U) == 0x1EU) ? true : false;

    /* Get address byte 1 and byte 2 according address bit number. */
    if (is10bitAddr)
    {
        addrByte1 = (uint8_t)(device->address >> 8U);
        addrByte2 = (uint8_t)device->address;
    }
    else
    {
        addrByte1 = (uint8_t)device->address;
    }

    /* Get the device address with r/w direction. If we have a sub-address,
      then that is always done as a write transfer prior to transferring
      the actual data.*/
    addrByte1 = addrByte1 << 1U;

    /* First need to write if 10-bit address or has cmd buffer. */
    addrByte1 |= (uint8_t)((is10bitAddr || cmdBuff) ? 0U : directionBit);

    /* Put slave address byte 1 into address buffer. */
    addrBuff[addrSize++] = addrByte1;

    if (is10bitAddr)
    {
        /* Put address byte 2 into address buffer. */
        addrBuff[addrSize++] = addrByte2;
    }

    /*--------------- Send Address Buffer ------------------*/
    master->txBuff = addrBuff;
    master->txSize = addrSize;

    /* Send first byte in address buffer to trigger interrupt.*/
    I2C_HAL_WriteByte(base, addrBuff[0]);

    /* Wait for the transfer to finish.*/
    SCCB_DRV_MasterWait(instance, timeout_ms);

    /*--------------------- Send CMD -----------------------*/
    if ((master->status == kStatus_I2C_Success) && cmdBuff)
    {
        master->txBuff = cmdBuff;
        master->txSize = cmdSize;

        /* Send first byte in address buffer to trigger interrupt.*/
        I2C_HAL_WriteByte(base, *cmdBuff);

        /* Wait for the transfer to finish.*/
        SCCB_DRV_MasterWait(instance, timeout_ms);
        
        if (direction == kI2CReceive)
        {
          /* Generate stop signal */
          I2C_HAL_SendStop(base);
        }
    }

    /*--------------- Send Address Again ------------------*/
    /* Send slave address again if receiving data from 10-bit address slave,
       OR conducting a cmd receive */
    if ((master->status == kStatus_I2C_Success) && (direction == kI2CReceive)
          && (is10bitAddr || cmdBuff))
    {
        /* Need to send slave address again. */
        master->txSize = 1U;
        master->txBuff = NULL;

        /* Need to generate a repeat start before changing to receive. */
        I2C_HAL_SendStart(base);

        /* Send address byte 1 again. */
        I2C_HAL_WriteByte(base, (uint8_t)(addrByte1) | 1U);

        /* Wait for the transfer to finish.*/
        SCCB_DRV_MasterWait(instance, timeout_ms);
    }

    master->isRequesting = false;
    master->isBlocking = isMainXferBlocking ;

    return master->status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SCCB_DRV_MasterWait
 * Description   : Wait transfer to finish.
 * This function is a static function which will be called by other data
 * transaction APIs.
 *
 *END**************************************************************************/
static i2c_status_t SCCB_DRV_MasterWait(uint32_t instance, uint32_t timeout_ms)
{
    assert(instance < I2C_INSTANCE_COUNT);

    i2c_master_state_t * master = (i2c_master_state_t *)g_i2cStatePtr[instance];
    osa_status_t syncStatus;

    do
    {
        syncStatus = OSA_SemaWait(&master->irqSync, timeout_ms);
    }while(syncStatus == kStatus_OSA_Idle);

    if (syncStatus != kStatus_OSA_Success)
    {
        master->status = kStatus_I2C_Timeout;
    }

    return master->status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_DRV_MasterReceive
 * Description   : Private function to handle blocking/non-blocking receive.
 * This function is a static function which will be called by other data
 * transaction APIs.
 *
 *END**************************************************************************/
static i2c_status_t SCCB_DRV_MasterReceive(uint32_t instance,
                                          const i2c_device_t * device,
                                          const uint8_t * cmdBuff,
                                          uint32_t cmdSize,
                                          uint8_t * rxBuff,
                                          uint32_t rxSize,
                                          uint32_t timeout_ms,
                                          bool isBlocking)
{
    assert(instance < I2C_INSTANCE_COUNT);
    assert(rxBuff);

    I2C_Type * base = g_i2cBase[instance];
    i2c_master_state_t * master = (i2c_master_state_t *)g_i2cStatePtr[instance];

    /* Return if current instance is used */
    if (!master->i2cIdle)
    {
        return master->status = kStatus_I2C_Busy;
    }

    /* Need to assign a pre-defined timeout value for sending address and cmd */
    if (!isBlocking)
    {
        timeout_ms = I2C_TIMEOUT_MS;
    }

    master->rxBuff = rxBuff;
    master->rxSize = rxSize;
    master->txBuff = NULL;
    master->txSize = 0;
    master->status = kStatus_I2C_Success;
    master->i2cIdle = false;
    master->isBlocking = isBlocking;

    I2C_DRV_MasterSetBaudRate(instance, device);

    /* Set direction to send for sending of address. */
    I2C_HAL_SetDirMode(base, kI2CSend);

    /* Enable i2c interrupt.*/
    I2C_HAL_ClearInt(base);
    I2C_HAL_SetIntCmd(base, true);

    /* Generate start signal. */
    I2C_HAL_SendStart(base);

    /* Send out slave address. */
    SCCB_DRV_SendAddress(instance, device, cmdBuff, cmdSize, kI2CReceive, timeout_ms);

    /* Start to receive data. */
    if (master->status == kStatus_I2C_Success)
    {
        /* Change direction to receive. */
        I2C_HAL_SetDirMode(base, kI2CReceive);

        /* Send NAK if only one byte to read. */
        if (rxSize == 0x1U)
        {
            I2C_HAL_SendNak(base);
        }
        else
        {
            I2C_HAL_SendAck(base);
        }

        /* Dummy read to trigger receive of next byte in interrupt. */
        I2C_HAL_ReadByte(base);

        if (isBlocking)
        {
            /* Wait for the transfer to finish.*/
            SCCB_DRV_MasterWait(instance, timeout_ms);
        }
    }
    else if (master->status == kStatus_I2C_Timeout)
    {
        /* Disable interrupt. */
        I2C_HAL_SetIntCmd(base, false);

        if (I2C_HAL_GetStatusFlag(base, kI2CBusBusy))
        {
            /* Generate stop signal. */
            I2C_HAL_SendStop(base);
        }

        /* Indicate I2C bus is idle. */
        master->i2cIdle = true;
    }

    return master->status;
}
                                          
/*!
 * @brief Performs a blocking receive transaction on the SCCB bus.
 *
 * Both cmdBuff and rxBuff are byte aligned, user needs to prepare these buffers
 * according to related protocol if slave devices data are not byte-aligned. 
 *
 * @param instance    The I2C peripheral instance number.
 * @param device      The pointer to the I2C device information structure.
 * @param cmdBuff     The pointer to the commands to be transferred, could be NULL.
 * @param cmdSize     The length in bytes of the commands to be transferred, could be 0.
 * @param rxBuff      The pointer to the data to be transferred, cannot be NULL.
 * @param rxSize      The length in bytes of the data to be transferred, cannot be 0.
 * @param timeout_ms  A timeout for the transfer in microseconds.
 * @return Error or success status returned by API.
 */
i2c_status_t SCCB_DRV_MasterReceiveDataBlocking(uint32_t instance,
                                               const i2c_device_t * device,
                                               const uint8_t * cmdBuff,
                                               uint32_t cmdSize,
                                               uint8_t * rxBuff,
                                               uint32_t rxSize,
                                               uint32_t timeout_ms)
{
    return SCCB_DRV_MasterReceive(instance, device, cmdBuff, cmdSize, rxBuff,
                                 rxSize, timeout_ms, true);
}

#endif /* FSL_FEATURE_SOC_I2C_COUNT */
/*******************************************************************************
 * EOF
 ******************************************************************************/

