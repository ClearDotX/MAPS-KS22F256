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
#include <stdio.h>
#include "fsl_rtc_hal.h"
#include "fsl_rtc_hal_test.h"

#include "fsl_device_registers.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*! @brief hal tests main function*/
void rtc_test_hal_init(void)
{
    rtc_datetime_t datetimeToSet, datetimeToGet;
    uint32_t secToSet, secToGet;
    uint32_t number_errors = 0;
    bool result;

    datetimeToSet.year = 2013;
    datetimeToSet.month = 10;
    datetimeToSet.day = 13;
    datetimeToSet.hour = 18;
    datetimeToSet.minute = 55;
    datetimeToSet.second = 30;

    /*function under test*/
    RTC_HAL_Init(RTC_BASE_ADDR);

    if((HW_RTC_CR(RTC_BASE_ADDR).B.OSCE) || HW_RTC_SR(RTC_BASE_ADDR).B.TCE)
    {
        number_errors++;
        printf("Disable RTC failed\r\n");
    }
    /*RTC Set datetime*/
    RTC_HAL_SetDatetime(RTC_BASE_ADDR, &datetimeToSet);
    RTC_HAL_ConvertDatetimeToSecs(&datetimeToSet, &secToSet);
    if(secToSet != HW_RTC_TSR(RTC_BASE_ADDR).B.TSR)
    {
        number_errors++;
        printf("Set Datetime failed\r\n");
    }
    /*RTC Set date time in secs*/
//    secToSet += 10;
    RTC_HAL_SetDatetimeInsecs(RTC_BASE_ADDR, secToSet);
    if(secToSet != HW_RTC_TSR(RTC_BASE_ADDR).B.TSR)
    {
        number_errors++;
        printf("Set Datetime in secs failed\r\n");
    }
    /*RTC Get datetime*/
    RTC_HAL_GetDatetime(RTC_BASE_ADDR, &datetimeToGet);
    RTC_HAL_ConvertDatetimeToSecs(&datetimeToGet, &secToGet);
    if(secToSet != secToGet)
    {
        number_errors++;
        printf("Get datetime failed");
    }
    /*RTC Get datetime in secs*/
    RTC_HAL_GetDatetimeInSecs(RTC_BASE_ADDR, &secToGet);
    if(secToSet != secToGet)
    {
        number_errors++;
        printf("Get datetime in secs failed");
    }
    /*Set Alarm*/
    datetimeToSet.second += 10;
    result = RTC_HAL_SetAlarm(RTC_BASE_ADDR, &datetimeToSet);
    if(!result)
    {
        number_errors++;
        printf("Set Alarm failed\r\n");
    }

    RTC_HAL_SetDatetime(RTC_BASE_ADDR, &datetimeToSet);
    RTC_HAL_ConvertDatetimeToSecs(&datetimeToSet, &secToSet);
    if(secToSet != HW_RTC_TAR(RTC_BASE_ADDR).B.TAR)
    {
        number_errors++;
        printf("Set Alarm failed\r\n");
    }
    /*Get Alarm*/
    RTC_HAL_GetAlarm(RTC_BASE_ADDR, &datetimeToGet);
    RTC_HAL_GetDatetimeInSecs(RTC_BASE_ADDR, &secToGet);
    if(secToSet != secToGet)
    {
        number_errors++;
        printf("Get Alarm failed");
    }
    /*Set Alarm*/
    datetimeToSet.second -= 20;
    result = RTC_HAL_SetAlarm(RTC_BASE_ADDR, &datetimeToSet);
    if(result)
    {
        number_errors++;
        printf("Set Alarm failed\r\n");
    }
    if(number_errors == 0)
    {
        printf("RTC Hal Test Pass!\r\n");
    }

}
/*******************************************************************************
 * EOF
 ******************************************************************************/

