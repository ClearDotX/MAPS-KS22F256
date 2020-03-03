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

#ifndef _SSD_FTFx_CONFIG_H_
#define _SSD_FTFx_CONFIG_H_

/*!
 * @addtogroup SSD_FTFx_Config
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @name Flash configuration
 * @{
 */

/*! @brief  Whether to support FlexNVM in flash driver */
#if !defined(FLASH_SSD_CONFIG_ENABLE_FLEXNVM_SUPPORT)
#define FLASH_SSD_CONFIG_ENABLE_FLEXNVM_SUPPORT 1
#endif

/*! @brief Flash driver location. */
#if !defined(FLASH_DRIVER_IS_FLASH_RESIDENT)
#if (!defined(BL_TARGET_ROM) && !defined(BL_TARGET_RAM))
#define FLASH_DRIVER_IS_FLASH_RESIDENT 1
#else
#define FLASH_DRIVER_IS_FLASH_RESIDENT 0
#endif
#endif

/*! @brief Flash Driver Export option */
#if !defined(FLASH_DRIVER_IS_EXPORTED)
#if (defined(BL_TARGET_ROM) || defined(BL_TARGET_FLASH))
#define FLASH_DRIVER_IS_EXPORTED 1
#else
#define FLASH_DRIVER_IS_EXPORTED 0
#endif
#endif

/*@}*/

/*******************************************************************************
 * Externs
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @}*/

#endif /* _SSD_FTFx_CONFIG_H_ */
