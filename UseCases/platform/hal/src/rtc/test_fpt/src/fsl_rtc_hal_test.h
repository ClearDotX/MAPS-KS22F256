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
#ifndef __FSL_RTC_HAL_TEST_H__
#define __FSL_RTC_HAL_TEST_H__

static uint8_t error_number = 0;

#define RTC_BASE_ADDR  0x4003D000

 
/*****************************************************************************/
/* Function Prototypes*/
/*****************************************************************************/
 
#if defined(__cplusplus)
extern "C" {
#endif

/*! @name RTC HAL Driver Test*/
/*@{*/


/*! @brief Stop here to detect errors.*/
static inline void exit_error(void)
{
    /* trap here for error  */
    error_number++;
    while(1){ }
}

/*! @brief Stop when finishing ok.*/
static inline void exit_OK(void)
{
    /* trap here for final ok  */
    while(1){ }
}

/*!
 * @brief Simple test RTC HAL driver's initialization.
 *
 * This function will basically test the RTC initialization function.
 *
 * If there is an error in the test the MCU instructions will fall into a the
 * the function 'exit_error(void)' and 'rtc_test_hal_init()' will never end.
 */
void rtc_test_hal_init(void);

/*!
 * @brief Simple test RTC HAL driver's monotonic counter increment.
 *
 * This function will basically test the RTC monotonic counter increment function.
 * This feature is not available in all devices (not in KL25 but present in K70).
 * For the KL25 the function's implementation is empty.
 * 
 * If there is an error in the test the MCU instructions will fall into a the
 * the function 'exit_error(void)' and 'rtc_test_hal_init()' will never end.
 */
void rtc_test_monotonic_counter_increment(void);
  
/*!
 * @brief Simple test RTC HAL driver's reset lock register.
 * 
 * Note: MUST do a Power On Reset (POR) before starting this test.
 * 
 * This function will basically test the reset lock register function.
 * 
 * If there is an error in the test the MCU instructions will fall into a the
 * the function 'exit_error(void)' and 'rtc_test_hal_init()' will never end.
 */
void rtc_test_reset_reg_LR(void);


/*@}*/
/*! @}*/
 
#endif /* __FSL_RTC_HAL_TEST_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/
