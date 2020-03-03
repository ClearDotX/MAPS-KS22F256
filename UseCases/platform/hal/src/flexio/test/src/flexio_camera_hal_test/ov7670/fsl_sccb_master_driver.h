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
#ifndef __FSL_SCCB_MASTER_DRIVER_H__
#define __FSL_SCCB_MASTER_DRIVER_H__

#include <stdlib.h>
#include <stdbool.h>
#include "fsl_i2c_hal.h"
#include "fsl_os_abstraction.h"

/*!
 * @addtogroup i2c_master
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name I2C Master
 * @{
 */
  
/*!
 * @brief Performs a blocking receive transaction on the I2C bus.
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
                                               uint32_t timeout_ms);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_SCCB_MASTER_DRIVER_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

