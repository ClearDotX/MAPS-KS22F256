/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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

#ifndef _APP_K80_FLEXIO_OV7670_H
#define _APP_K80_FLEXIO_OV7670_H

#include <string.h>
#include <stdio.h>

#include "board.h"

#include "pin_mux.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"

#include "fsl_sim_hal.h"
#include "fsl_dmamux_hal.h"
#include "fsl_edma_hal.h"
#include "fsl_pmc_hal.h"
#include "fsl_smc_hal.h"
#include "fsl_mcg_hal_modes.h"
#include "fsl_flexio_hal.h"
#include "fsl_flexio_camera_hal.h"
#include "fsl_gpio_hal.h"

#include "fsl_i2c_shared_function.h"
#include "fsl_i2c_master_driver.h"

#include "ov7670_driver.h"
#include "SSD1289.h"
#include "flexbus_16b.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
// Picture data
#define OV7670_BYTES_PER_PIXEL          2
#define OV7670_HORIZONTAL_POINTS        320
#define OV7670_VERTICAL_POINTS          240
#define OV7670_FRAME_PIXELS             (OV7670_HORIZONTAL_POINTS * OV7670_VERTICAL_POINTS)
#define OV7670_FRAME_BYTES              (OV7670_FRAME_PIXELS * OV7670_BYTES_PER_PIXEL)
#define LCD_HORIZONTAL_START_POINT      ((SCREEN_SIZE_LONGER_SIDE - OV7670_HORIZONTAL_POINTS)>>1)
#define LCD_VERTICAL_START_POINT        ((SCREEN_SIZE_SHORTER_SIDE - OV7670_VERTICAL_POINTS)>>1)
#define LCD_HORIZONTAL_END_POINT        (LCD_HORIZONTAL_START_POINT + OV7670_HORIZONTAL_POINTS -1)
#define LCD_VERTICAL_END_POINT          (LCD_VERTICAL_START_POINT + OV7670_VERTICAL_POINTS -1)

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/
void configure_clkout_pin(void);
void configure_app_gpio_pins(void);
void configure_app_i2c_pins();
void configure_app_flexio_pins(void);
void configure_app_flexbus_pins(void);
void PORTA_InstallCallback(uint8_t priority, void (*callbackFunc)(uint32_t pin));

/*******************************************************************************
 * Global variables
 ******************************************************************************/
extern const clock_manager_user_config_t g_RunClockConfig;
extern volatile uint16_t *p_u16CameraFrameBuffer;
extern volatile uint8_t u8PowerModeEnter;
extern flexio_camera_dev_t gFlexioCameraDevStruct;

/*******************************************************************************
 * High speed run mode clock 150MHz configuration structure initialization
 ******************************************************************************/
#define HS_RUN_CLOCK_CONFIGURATION                                              \
{                                                                               \
    .mcgConfig =                                                                \
    {                                                                           \
        .mcg_mode               = kMcgModePEE,                                  \
        .irclkEnable            = false,                                        \
        .irclkEnableInStop      = false,                                        \
        .ircs                   = kMcgIrcFast,                                  \
        .fcrdiv                 = 0U,                                           \
        .frdiv                  = 3U,                                           \
        .drs     = kMcgDcoRangeSelLow,                                          \
        .dmx32   = kMcgDmx32Default,                                            \
        .oscsel  = kMcgOscselOsc,                                               \
        .pll0EnableInFllMode = false,                                           \
        .pll0EnableInStop  = true,                                              \
        .prdiv0            = 0x00U,                                             \
        .vdiv0             = 0x09U,                                             \
    },                                                                          \
    .simConfig =                                                                \
    {                                                                           \
        .pllFllSel = kClockPllFllSelPll,                                        \
        .pllflldiv = 0,                                                         \
        .pllfllfrac = 0,                                                        \
        .er32kSrc  = kClockEr32kSrcRtc,                                         \
        .outdiv1   = 0U,                                                        \
        .outdiv2   = 1U,                                                        \
        .outdiv3   = 2U,                                                        \
        .outdiv4   = 5U,                                                        \
    },                                                                          \
    .oscerConfig =                                                              \
    {                                                                           \
        .enable       = true,                                                   \
        .enableInStop = true,                                                   \
    }                                                                           \
}

/*******************************************************************************
 * OV7670 device SCCB / I2C configuration handler structure initialization
 ******************************************************************************/
#define OV7670_HANDLER                                                          \
{                                                                               \
  .i2cInstance = 2,                                                             \
  .device.address = OV7670_I2C_ADDR,                                            \
  .device.baudRate_kbps = OV7670_I2C_BAUDRATE,                                  \
  .state =                                                                      \
  {                                                                             \
    .rxBuff = &receiveBuff,                                                     \
    .rxSize = sizeof(receiveBuff),                                              \
    .txBuff = &sendBuff,                                                        \
    .txSize = sizeof(sendBuff),                                                 \
    .status = kStatus_I2C_Success,                                              \
    .i2cIdle = 0,                                                               \
    .isBlocking = 0,                                                            \
    .isRequesting = 0,                                                          \
    .lastBaudRate_kbps = 100,                                                   \
    .irqSync =                                                                  \
    {                                                                           \
      .isWaiting = 0,                                                           \
      .semCount = 0,                                                            \
      .time_start = 0,                                                          \
      .timeout = 0,                                                             \
    },                                                                          \
  },                                                                            \
}

/*******************************************************************************
 * OV7670 device configuration structure initialization
 ******************************************************************************/
#define OV7670_CONFIGURATION                                                    \
{                                                                               \
  .output_format = (ov7670_output_format_cfg_t*)&OV7670_FORMAT_RGB565,          \
  .resolution =                                                                 \
  {                                                                             \
    .width = OV7670_HORIZONTAL_POINTS,                                          \
    .heigth = OV7670_VERTICAL_POINTS,                                           \
  },                                                                            \
  .frame_rate = (ov7670_frame_rate_cfg_t*)&OV7670_30FPS_12MHZ_XCLK,             \
  .contrast = 0x30,                                                             \
  .brightness = 0x10,                                                           \
  .advanced_config = &ov7670_advanced_config,                                   \
}

/*******************************************************************************
 * OV7670 device configuration structure initialization
 ******************************************************************************/
#define OV7670_ADVANCED_CONFIGURATION                                           \
{                                                                               \
  .filter = (ov7670_filter_cfg_t*)&OV7670_FILTER_DISABLED,                      \
  .night_mode = (ov7670_night_mode_cfg_t*)&OV7670_NIGHT_MODE_DISABLED,          \
  .white_balance = (ov7670_white_balance_cfg_t*)&OV7670_WHITE_BALANCE_SIMPLE,   \
  .light_mode = (ov7670_light_mode_cfg_t*)&OV7670_LIGHT_MODE_DISABLED,          \
  .color_saturation = (ov7670_color_saturation_cfg_t*)&OV7670_COLOR_SATURATION_2PLUS,\
  .special_effect = (ov7670_special_effect_cfg_t*)&OV7670_SPECIAL_EFFECT_DISABLED,\
  .gamma_curve_slope = (ov7670_gamma_curve_slope_cfg_t*)&OV7670_GAMMA_CURVE_SLOPE_DEFAULT,\
}

/*******************************************************************************
 * DMA0 channel 16 configuration structure.
 *  details This DMA channels should ensure 16-bit (2 bytes) data transders from
 *          camera frame buffer to flexbus (LCD SSD1289). At the end it means
 *          4800 (major loop) x 16 x 16-bit data transfers (32 bytes minor loops).
 *          These transfers ensures frame displaying. 
 *          Data are transfered based on shifter 0 status flag
 *          (all shifter full-filled by appropriate data).
 ******************************************************************************/
#define eDMA16_CONFIG                                                           \
{                                                                               \
  .srcAddr            = (uint32_t)u16CameraFrameBuffer,                         \
  .destAddr           = (uint32_t)FLEX_BASE_ADDRESS,                            \
  .srcTransferSize    = kEDMATransferSize_2Bytes,                               \
  .destTransferSize   = kEDMATransferSize_2Bytes,                               \
  .srcOffset          = 2,                                                      \
  .destOffset         = 0,                                                      \
  .srcLastAddrAdjust  = -((sizeof(u16CameraFrameBuffer))),                      \
  .destLastAddrAdjust = 0,                                                      \
  .srcModulo          = kEDMAModuloDisable,                                     \
  .destModulo         = kEDMAModuloDisable,                                     \
  .minorLoopCount     = 32,                                                     \
  .majorLoopCount     = ((sizeof(u16CameraFrameBuffer)>>5)),                    \
}

/*******************************************************************************
 * DMA0 channel 17 configuration structure.
 *  details This DMA channels should ensure 32 bytes data transders (burns) from
 *          shifter0-7 buffer to the one frame data circular buffer (150kB).
 *          At the end it means 4800 (major loop) x 32-bytes (minor loops) 
 *          transfers. Data are transfered based on shifter 0 status flag  
 *          (all shifter full-filled by appropriate data).
 ******************************************************************************/
#define eDMA17_CONFIG                                                           \
{                                                                               \
  .srcAddr            = 0U, /* Dummy. */                                        \
  .destAddr           = (uint32_t)u16CameraFrameBuffer,                         \
  .srcTransferSize    = kEDMATransferSize_32Bytes,                              \
  .destTransferSize   = kEDMATransferSize_32Bytes,                              \
  .srcOffset          = 0,                                                      \
  .destOffset         = 32,                                                     \
  .srcLastAddrAdjust  = 0,                                                      \
  .destLastAddrAdjust = -((sizeof(u16CameraFrameBuffer))),                      \
  .srcModulo          = kEDMAModuloDisable,                                     \
  .destModulo         = kEDMAModuloDisable,                                     \
  .minorLoopCount     = 32,                                                     \
  .majorLoopCount     = ((sizeof(u16CameraFrameBuffer)>>5)),                    \
}

#endif /*_APP_FLEXIO_OV7670_H*/

