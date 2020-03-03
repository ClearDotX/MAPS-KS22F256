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
#ifndef __FSL_LPTMR_HAL_UNIT_TEST_H__
#define __FSL_LPTMR_HAL_UNIT_TEST_H__

#include "fsl_lptmr_hal.h"
#include <stdint.h>
#include "fsl_device_registers.h"
#include "fsl_clock_manager.h"

/*!
 * @addtogroup LPTMR_HAL_unit_test
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Structure for all LPTMR status flags */
typedef union _lptmr_test_status
{
    uint64_t all;
    struct LptmrTestStatusBits
    {
        unsigned testStatusEnable: 1;
        unsigned testStatusDisable : 1;
        unsigned testStatusClearIntFlag : 1;
        unsigned testStatusSetIntCmd : 1;
        unsigned testStatusPinSelect : 1;
        unsigned testStatusPinPolatiry : 1;
        unsigned testStatusSetFreeRunningCmd: 1;
        unsigned testStatusTimerMode: 1;
        unsigned testStatusPrescalerValue: 1;
        unsigned testStatusSetPrescalerCmd: 1;
        unsigned testStatusPrescaleClockSource: 1;
        unsigned testStatusCompareValue: 1;
    } B;
} lptmr_test_status_t;

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*@}  */

/*! @}*/

#endif /* __FSL_LPTMR_HAL_UNIT_TEST_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

