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

#ifndef _SSD_FTFx_MISC_H_
#define _SSD_FTFx_MISC_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>

/*!
 * @addtogroup SSD_FTFx_Misc
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, a) ((x) & (uint32_t)(-((int32_t)(a))))
#endif
#ifndef ALIGN_UP
#define ALIGN_UP(x, a) (-((int32_t)((uint32_t)(-((int32_t)(x))) & (uint32_t)(-((int32_t)(a))))))
#endif

/* clang-format off */
#define BYTES_JOIN_TO_WORD_1_3(x, y)             ( (((uint32_t)(x)&0xFFU)<<24) | ((uint32_t)(y)&0xFFFFFFU) )
#define BYTES_JOIN_TO_WORD_2_2(x, y)             ( (((uint32_t)(x)&0xFFFFU)<<16) | ((uint32_t)(y)&0xFFFFU) )
#define BYTES_JOIN_TO_WORD_3_1(x, y)             ( (((uint32_t)(x)&0xFFFFFFU)<<8) | ((uint32_t)(y)&0xFFU) )
#define BYTES_JOIN_TO_WORD_1_1_2(x, y, z)        ( (((uint32_t)(x)&0xFFU)<<24) | (((uint32_t)(y)&0xFFU)<<16) | ((uint32_t)(z)&0xFFFFU) )
#define BYTES_JOIN_TO_WORD_1_2_1(x, y, z)        ( (((uint32_t)(x)&0xFFU)<<24) | (((uint32_t)(y)&0xFFFFU)<<8) | ((uint32_t)(z)&0xFFU) )
#define BYTES_JOIN_TO_WORD_2_1_1(x, y, z)        ( (((uint32_t)(x)&0xFFFFU)<<16) | (((uint32_t)(y)&0xFFU)<<8) | ((uint32_t)(z)&0xFFU) )
#define BYTES_JOIN_TO_WORD_1_1_1_1(x, y, z, w)   ( (((uint32_t)(x)&0xFFU)<<24) | (((uint32_t)(y)&0xFFU)<<16) | (((uint32_t)(z)&0xFFU)<<8) | ((uint32_t)(w)&0xFFU) )
/* clang-format on */

/*******************************************************************************
 * Externs
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @}*/

#endif /* _SSD_FTFx_MISC_H_ */
