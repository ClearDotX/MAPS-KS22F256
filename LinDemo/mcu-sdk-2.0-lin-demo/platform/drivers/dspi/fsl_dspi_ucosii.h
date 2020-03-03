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
#ifndef __FSL_DSPI_FREERTOS_H__
#define __FSL_DSPI_FREERTOS_H__

#include <ucos_ii.h>

#include "fsl_dspi.h"

/*!
 * @addtogroup dspi_driver
 * @{
 */

/**********************************************************************************************************************
 * Definitions
 *********************************************************************************************************************/

/*! @brief DSPI FreeRTOS handle */
typedef struct _dspi_rtos_handle
{
    SPI_Type *base;                  /*!< DSPI base address */
    dspi_master_handle_t drv_handle; /*!< Handle of the underlying driver, treated as opaque by the RTOS layer */
    status_t async_status;
    OS_EVENT *mutex;   /*!< Mutex to lock the handle during a trasfer */
#define RTOS_DSPI_COMPLETE 0x1
    OS_FLAG_GRP *event;   /*!< Semaphore to notify and unblock task when transfer ends */
} dspi_rtos_handle_t;

/**********************************************************************************************************************
 * API
 *********************************************************************************************************************/

status_t DSPI_RTOS_Init(dspi_rtos_handle_t *handle,
                        SPI_Type *base,
                        const dspi_master_config_t *masterConfig,
                        uint32_t srcClock_Hz);
status_t DSPI_RTOS_Deinit(dspi_rtos_handle_t *handle);
status_t DSPI_RTOS_Transfer(dspi_rtos_handle_t *handle, dspi_transfer_t *transfer);

#endif /* __FSL_DSPI_FREERTOS_H__ */
