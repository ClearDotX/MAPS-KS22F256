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

#include "ov7670_def.h"
#include "ov7670_driver.h"
#include "fsl_clock_manager.h"

/*******************************************************************************
 * Constants
 ******************************************************************************/
/*! @brief OV7670 resolution options                                          */ 
const ov7670_resolution_t OV7670_VGA =  {640, 480};
const ov7670_resolution_t OV7670_QVGA = {320, 240};
const ov7670_resolution_t OV7670_QQVGA ={160, 120};
const ov7670_resolution_t OV7670_CIF =  {352, 288};
const ov7670_resolution_t OV7670_QCIF = {176, 144};
const ov7670_resolution_t OV7670_QQCIF ={ 88,  72};

/*! @brief OV7670 resolution options                                          */ 
const ov7670_window_start_point_t OV7670_WINDOW_START_POINT_VGA_DEFAULT =  {140, 16};
const ov7670_window_start_point_t OV7670_WINDOW_START_POINT_QVGA_DEFAULT = {272, 16};
const ov7670_window_start_point_t OV7670_WINDOW_START_POINT_QQVGA_DEFAULT ={140, 16};
const ov7670_window_start_point_t OV7670_WINDOW_START_POINT_CIF_DEFAULT =  {140, 16};
const ov7670_window_start_point_t OV7670_WINDOW_START_POINT_QCIF_DEFAULT = {140, 16};
const ov7670_window_start_point_t OV7670_WINDOW_START_POINT_QQCIF_DEFAULT ={140, 16};

/*! @brief Night mode initialization structure data                           */  
const ov7670_output_format_cfg_t OV7670_FORMAT_RawBayerRGB =            {0x01, 0x00, 0x00};
const ov7670_output_format_cfg_t OV7670_FORMAT_ProcessedBayerRGB =      {0x05, 0x00, 0x00};
const ov7670_output_format_cfg_t OV7670_FORMAT_YUV422 =                 {0x00, 0x00, 0x00};
const ov7670_output_format_cfg_t OV7670_FORMAT_GRB422 =                 {0x04, 0x00, 0x00};
const ov7670_output_format_cfg_t OV7670_FORMAT_RGB565 =                 {0x04, 0xd0, 0x00};
const ov7670_output_format_cfg_t OV7670_FORMAT_RGB555 =                 {0x04, 0xf0, 0x00};
const ov7670_output_format_cfg_t OV7670_FORMAT_xRGB444 =                {0x04, 0xd0, 0x2};
const ov7670_output_format_cfg_t OV7670_FORMAT_RGBx444 =                {0x04, 0xd0, 0x3};

/*! @brief resolution initialization structure data                           */  
const ov7670_resolution_cfg_t OV7670_RESOLUTION_VGA =           {0x00, 0x00, 0x00, 0x3a, 0x35, 0x11, 0xf0, 0x02}; ///< 640 x 480
const ov7670_resolution_cfg_t OV7670_RESOLUTION_QVGA_ORIGINAL = {0x10, 0x00, 0x00, 0x3a, 0x35, 0x11, 0xf0, 0x02}; ///< 320 x 240
const ov7670_resolution_cfg_t OV7670_RESOLUTION_QVGA =          {0x10, 0x04, 0x19, 0x3a, 0x35, 0x11, 0xf1, 0x02}; ///< 320 x 240
const ov7670_resolution_cfg_t OV7670_RESOLUTION_QQVGA =         {0x10, 0x04, 0x1A, 0x3a, 0x35, 0x22, 0xf2, 0x02}; ///< 160 x 120

const ov7670_resolution_cfg_t OV7670_RESOLUTION_CIF =           {0x20, 0x08, 0x11, 0x3a, 0x35, 0x11, 0xf1, 0x02}; ///< 352 x 288
const ov7670_resolution_cfg_t OV7670_RESOLUTION_QCIF_ORIGINAL = {0x21, 0x08, 0x11, 0x3a, 0x35, 0x11, 0xf1, 0x02}; ///< 176 x 144
const ov7670_resolution_cfg_t OV7670_RESOLUTION_QCIF =          {0x28, 0x00, 0x11, 0x3a, 0x35, 0x11, 0xf1, 0x02}; ///< 176 x 144
const ov7670_resolution_cfg_t OV7670_RESOLUTION_QQCIF =         {0x28, 0x0c, 0x12, 0x3a, 0x35, 0x22, 0xf2, 0x02}; ///< 88 x 72

/*! @brief Special effects configuration initialization structure data       */  
const ov7670_windowing_cfg_t OV7670_WINDOW_VGA =   {0x36, 0x13, 0x01, 0x0a, 0x02, 0x7a};
const ov7670_windowing_cfg_t OV7670_WINDOW_QVGA =  {0x80, 0x15, 0x03, 0x00, 0x03, 0x7b};
const ov7670_windowing_cfg_t OV7670_WINDOW_QQVGA = {0x64, 0x16, 0x04, 0x0a, 0x03, 0x7b};
const ov7670_windowing_cfg_t OV7670_WINDOW_CIF =   {0x12, 0x15, 0x0b, 0x0a, 0x03, 0x7b};
const ov7670_windowing_cfg_t OV7670_WINDOW_QCIF =  {0x80, 0x39, 0x03, 0x0a, 0x03, 0x7b};
const ov7670_windowing_cfg_t OV7670_WINDOW_QQCIF = {0x64, 0x3a, 0x03, 0x06, 0x03, 0x7b};

/*! @brief Frame rate initialization structure data                           */  
const ov7670_frame_rate_cfg_t OV7670_30FPS_26MHZ_XCLK = {0x80, 0x0a, 0x00, 0x00, 0x2b, 0x00};
const ov7670_frame_rate_cfg_t OV7670_25FPS_26MHZ_XCLK = {0x80, 0x0a, 0x00, 0x00, 0x99, 0x00};
const ov7670_frame_rate_cfg_t OV7670_15FPS_26MHZ_XCLK = {0x00, 0x0a, 0x00, 0x00, 0x2b, 0x00};
const ov7670_frame_rate_cfg_t OV7670_14FPS_26MHZ_XCLK = {0x00, 0x0a, 0x00, 0x00, 0x46, 0x00};

const ov7670_frame_rate_cfg_t OV7670_30FPS_24MHZ_XCLK = {0x80, 0x0a, 0x00, 0x00, 0x00, 0x00};
const ov7670_frame_rate_cfg_t OV7670_25FPS_24MHZ_XCLK = {0x80, 0x0a, 0x00, 0x00, 0x66, 0x00};
const ov7670_frame_rate_cfg_t OV7670_15FPS_24MHZ_XCLK = {0x00, 0x0a, 0x00, 0x00, 0x00, 0x00};
const ov7670_frame_rate_cfg_t OV7670_14FPS_24MHZ_XCLK = {0x00, 0x0a, 0x00, 0x00, 0x1a, 0x00};

const ov7670_frame_rate_cfg_t OV7670_30FPS_13MHZ_XCLK = {0x00, 0x4a, 0x00, 0x00, 0x2b, 0x00};
const ov7670_frame_rate_cfg_t OV7670_25FPS_13MHZ_XCLK = {0x00, 0x4a, 0x00, 0x00, 0x99, 0x00};
const ov7670_frame_rate_cfg_t OV7670_15FPS_13MHZ_XCLK = {0x01, 0x4a, 0x00, 0x00, 0x2b, 0x00};
const ov7670_frame_rate_cfg_t OV7670_14FPS_13MHZ_XCLK = {0x01, 0x4a, 0x00, 0x00, 0x46, 0x00};

const ov7670_frame_rate_cfg_t OV7670_30FPS_12MHZ_XCLK = {0x00, 0x4a, 0x00, 0x00, 0x2b, 0x00};
const ov7670_frame_rate_cfg_t OV7670_25FPS_12MHZ_XCLK = {0x00, 0x4a, 0x00, 0x00, 0x66, 0x00};
const ov7670_frame_rate_cfg_t OV7670_15FPS_12MHZ_XCLK = {0x01, 0x4a, 0x00, 0x00, 0x2b, 0x00};
const ov7670_frame_rate_cfg_t OV7670_14FPS_12MHZ_XCLK = {0x01, 0x4a, 0x00, 0x00, 0x46, 0x00};

/*! @brief Night mode initialization structure data                           */
const ov7670_night_mode_cfg_t OV7670_NIGHT_MODE_DISABLED = {0x00};
const ov7670_night_mode_cfg_t OV7670_NIGHT_MODE_AUTO_FR_DIVBY2 = {0xa0};
const ov7670_night_mode_cfg_t OV7670_NIGHT_MODE_AUTO_FR_DIVBY4 = {0xc0};
const ov7670_night_mode_cfg_t OV7670_NIGHT_MODE_AUTO_FR_DIVBY8 = {0xe0};

/*! @brief Banding filter initialization structure data                       */
const ov7670_filter_cfg_t OV7670_FILTER_DISABLED = {0x00, 0x98, 0x7f, 0x02, 0x03, 0x02};
const ov7670_filter_cfg_t OV7670_FILTER_30FPS_60HZ = {0x20, 0x98, 0x7f, 0x02, 0x03, 0x02};
const ov7670_filter_cfg_t OV7670_FILTER_15FPS_60HZ = {0x20, 0x4c, 0x3f, 0x05, 0x07, 0x02};
const ov7670_filter_cfg_t OV7670_FILTER_25FPS_50HZ = {0x20, 0x98, 0x7f, 0x03, 0x03, 0x0a};
const ov7670_filter_cfg_t OV7670_FILTER_14FPS_50HZ = {0x20, 0x4c, 0x3f, 0x06, 0x07, 0x0a};
const ov7670_filter_cfg_t OV7670_FILTER_30FPS_60HZ_AUTO_LIGHT_FREQ_DETECT = {0x20, 0x98, 0x7f, 0x02, 0x03, 0x12};
const ov7670_filter_cfg_t OV7670_FILTER_15FPS_60HZ_AUTO_LIGHT_FREQ_DETECT = {0x20, 0x4c, 0x3f, 0x05, 0x07, 0x12};
const ov7670_filter_cfg_t OV7670_FILTER_25FPS_50HZ_AUTO_LIGHT_FREQ_DETECT = {0x20, 0x98, 0x7f, 0x03, 0x03, 0x1a};
const ov7670_filter_cfg_t OV7670_FILTER_14FPS_50HZ_AUTO_LIGHT_FREQ_DETECT = {0x20, 0x4c, 0x3f, 0x06, 0x07, 0x1a};

/*! @brief White balance initialization structure data                        */  
const ov7670_white_balance_cfg_t OV7670_WHITE_BALANCE_DEFAULT =  {0x02, 0x9a, 0xc0, 0x55, 0x02, 0x14,  \
                                                                  0xf0, 0x45, 0x61, 0x51, 0x79, 0x08};
const ov7670_white_balance_cfg_t OV7670_WHITE_BALANCE_DISABLED = {0x00, 0x9a, 0xc0, 0x55, 0x02, 0x14, \
                                                                  0xf0, 0x45, 0x61, 0x51, 0x79, 0x00};
const ov7670_white_balance_cfg_t OV7670_WHITE_BALANCE_SIMPLE =   {0x02, 0x9f, 0x10, 0x55, 0x02, 0x14,   \
                                                                  0xf0, 0x45, 0x61, 0x51, 0x79, 0x08};

/*! @brief Light mode configuration initialization structure data             */
const ov7670_light_mode_cfg_t OV7670_LIGHT_MODE_DISABLED = {0x05, 0x0a, 0x08, 0x00, 0x08};
const ov7670_light_mode_cfg_t OV7670_LIGHT_MODE_AUTO =     {0xc5, 0x3a, 0x08, 0x00, 0x08};
const ov7670_light_mode_cfg_t OV7670_LIGHT_MODE_SUNNY =    {0xc5, 0x6a, 0x5a, 0x00, 0x5c};
const ov7670_light_mode_cfg_t OV7670_LIGHT_MODE_CLOUDY =   {0xc5, 0x0a, 0x58, 0x00, 0x60};
const ov7670_light_mode_cfg_t OV7670_LIGHT_MODE_OFFICE =   {0xc5, 0x2a, 0x84, 0x00, 0x4c};
const ov7670_light_mode_cfg_t OV7670_LIGHT_MODE_HOME =     {0xc5, 0x1a, 0x96, 0x00, 0x40};

/*! @brief Color saturation configuration initialization structure data       */
const ov7670_color_saturation_cfg_t OV7670_COLOR_SATURATION_4PLUS =  {0xc0, 0xc0, 0x00, 0x33, 0x8d, 0xc0, 0x9e, 0x02};
const ov7670_color_saturation_cfg_t OV7670_COLOR_SATURATION_3PLUS =  {0x99, 0x99, 0x00, 0x28, 0x71, 0x99, 0x9e, 0x02};
const ov7670_color_saturation_cfg_t OV7670_COLOR_SATURATION_2PLUS =  {0xc0, 0xc0, 0x00, 0x33, 0x8d, 0xc0, 0x9e, 0x00};
const ov7670_color_saturation_cfg_t OV7670_COLOR_SATURATION_1PLUS =  {0x99, 0x99, 0x00, 0x28, 0x71, 0x99, 0x9e, 0x00};
const ov7670_color_saturation_cfg_t OV7670_COLOR_SATURATION_0 =      {0x80, 0x80, 0x00, 0x22, 0x5e, 0x80, 0x9e, 0x00};
const ov7670_color_saturation_cfg_t OV7670_COLOR_SATURATION_DEFAULT ={0x40, 0x34, 0x0c, 0x17, 0x29, 0x40, 0x1e, 0x00};
const ov7670_color_saturation_cfg_t OV7670_COLOR_SATURATION_1MINUS = {0x66, 0x66, 0x00, 0x1b, 0x4b, 0x66, 0x9e, 0x00};
const ov7670_color_saturation_cfg_t OV7670_COLOR_SATURATION_2MINUS = {0x40, 0x40, 0x00, 0x11, 0x2f, 0x40, 0x9e, 0x00};

/*! @brief Special effects configuration initialization structure data       */  
const ov7670_special_effect_cfg_t OV7670_SPECIAL_EFFECT_ANTIQUE =     {0x18, 0, 255};
const ov7670_special_effect_cfg_t OV7670_SPECIAL_EFFECT_SEPHIA =      {0x18, 16, 146};
const ov7670_special_effect_cfg_t OV7670_SPECIAL_EFFECT_BLUISH =      {0x18, 240, 146};
const ov7670_special_effect_cfg_t OV7670_SPECIAL_EFFECT_GREENISH =    {0x18, 0, 30};
const ov7670_special_effect_cfg_t OV7670_SPECIAL_EFFECT_REDISH =      {0x18, 90, 240};
const ov7670_special_effect_cfg_t OV7670_SPECIAL_EFFECT_BW =          {0x18, 110, 110};
const ov7670_special_effect_cfg_t OV7670_SPECIAL_EFFECT_NEGATIVE =    {0x28, 0x80, 0x80};
const ov7670_special_effect_cfg_t OV7670_SPECIAL_EFFECT_BW_NEGATIVE = {0x38, 110, 110};
const ov7670_special_effect_cfg_t OV7670_SPECIAL_EFFECT_NORMAL =      {0x0c, 0x80, 0x80};
const ov7670_special_effect_cfg_t OV7670_SPECIAL_EFFECT_DISABLED =    {0x08, 0x80, 0x80};

/*! @brief Special effects configuration initialization structure data       */  
const ov7670_gamma_curve_slope_cfg_t OV7670_GAMMA_CURVE_SLOPE_DEFAULT   = {0x24, 0x04, 0x07, 0x10, 0x28, 0x36, 0x44, 0x52, 0x60, 0x6c, 0x78, 0x8c, 0x9e, 0xbb, 0xd2, 0xe5};
const ov7670_gamma_curve_slope_cfg_t OV7670_GAMMA_CURVE_SLOPE1          = {0x20, 0x10, 0x1e, 0x35, 0x5a, 0x69, 0x76, 0x80, 0x88, 0x8f, 0x96, 0xa3, 0xaf, 0xc4, 0xd7, 0xe8};

/*******************************************************************************
 * Code
 ******************************************************************************/
static volatile uint32_t i2c_state = 0;
static ov7670_status_t OV7670_SCCB_Init(ov7670_handler_t *handler);

/*FUNCTION**********************************************************************
 *
 * Function Name : ov7670_Init
 * Description   : Initialize the ov7670 board.
 * This function has configured the ov7670 board. If the codec_config is NULL,
 * use default setting.
 *END**************************************************************************/
ov7670_status_t OV7670_Init(ov7670_handler_t *handler, ov7670_config_t *ov7670_cfg)
{
    uint8_t u8TempVal0, u8TempVal1;

    if(i2c_state == 0)
    {
        OV7670_SCCB_Init(handler);
        i2c_state ++;
    }
    else
    {
        return kStatus_OV7670_Fail;
    }
    
    /* Reset Device */
    OV7670_WriteReg(handler, OV7670_COM7_REG, 0x80);
    /* wait for a least 1ms */
    OSA_TimeDelay(5); /* 5ms */
    
    /* Read product ID nuumber MSB */
    if (OV7670_ReadReg(handler, OV7670_PID_REG, &u8TempVal0) != kStatus_OV7670_Success)
    {
        return kStatus_OV7670_Fail;
    }
    /* Read product ID nuumber MSB */
    if (OV7670_ReadReg(handler, OV7670_VER_REG, &u8TempVal1) != kStatus_OV7670_Success)
    {
        return kStatus_OV7670_Fail;
    }
    if ((u8TempVal0 != OV7670_PID_NUM) && (u8TempVal1 != OV7670_VER_NUM))
    {
        return kStatus_OV7670_Fail;
    }

    /* NULL pointer means default setting. */
    if(ov7670_cfg == NULL)
    {
        /* Reset Device */
        OV7670_WriteReg(handler, OV7670_COM7_REG, 0x80);
        /* wait for a bit */
        OSA_TimeDelay(5); /* 5ms */
    }
    else
    {
      OV7670_Configuration(handler, ov7670_cfg);
    }
    return kStatus_OV7670_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OV7670_SCCB_Init
 * Description   : Initialize the SCCB/I2C transfer.
 * The ov7670 codec is controlled by SCCB/I2C, using SCCB/I2C transfer can access the ov7670 register.
 *END**************************************************************************/
static ov7670_status_t OV7670_SCCB_Init(ov7670_handler_t *handler)
{
    /* The master structure initialize */
    I2C_DRV_MasterInit(handler->i2cInstance, &handler->state);
    OSA_TimeDelay(300);
    /* Configure the device info of I2C */
    handler->device.baudRate_kbps = OV7670_I2C_BAUDRATE;
    handler->device.address = OV7670_I2C_ADDR;
    I2C_DRV_MasterSetBaudRate(handler->i2cInstance, &handler->device);
  
    return kStatus_OV7670_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OV7670_Deinit
 * Description   : Deinit the ov7670 board.
 * This function would free the i2c source applied.
 *END**************************************************************************/
ov7670_status_t OV7670_Deinit(ov7670_handler_t *handler)
{
    i2c_state --;
    if(i2c_state == 0)
    {
        I2C_DRV_MasterDeinit(handler->i2cInstance);
    }
    return kStatus_OV7670_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OV7670_WriteReg
 * Description   : Write the specified register of ov7670.
 * The writing process is through I2C.
 *END**************************************************************************/
ov7670_status_t OV7670_WriteReg(ov7670_handler_t *handler, uint8_t reg, uint8_t val)
{
    i2c_device_t *device = &(handler->device);
    uint8_t cmd[1],buff[1];
    uint8_t retval = 0;
    /* The register address */
    cmd[0] = reg;
    /* Data */
    buff[0] = val;
    retval = I2C_DRV_MasterSendDataBlocking(handler->i2cInstance,device,cmd,1,buff,1,OV7670_I2C_TIMEOUT_MS);
    if(retval != kStatus_I2C_Success)
    {
        return kStatus_OV7670_I2CFail;
    }
    return kStatus_OV7670_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OV7670_ReadReg
 * Description   : Read the specified register value of ov7670.
 * The reading process is through I2C.
 *END**************************************************************************/
ov7670_status_t OV7670_ReadReg(ov7670_handler_t *handler, uint8_t reg, uint8_t *val)
{
    i2c_device_t *device = &(handler->device);
    uint8_t buff[1];
    uint8_t data[1];
    uint8_t retval = 0;
    buff[0] = reg;
    retval = SCCB_DRV_MasterReceiveDataBlocking(handler->i2cInstance,device,buff,1,data,1,OV7670_I2C_TIMEOUT_MS);
    if(retval != kStatus_I2C_Success)
    {
        return kStatus_OV7670_I2CFail;
    }
    *val = data[0];
    return kStatus_OV7670_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OV7670_ModifyReg
 * Description   : Modify the specified register value of ov7670.
 * The modify process is through I2C.
 *END**************************************************************************/
ov7670_status_t OV7670_ModifyReg(ov7670_handler_t *handler, uint8_t reg, uint8_t clr_mask, uint8_t val)
{
    uint8_t retval = 0;
    uint8_t reg_val;
    retval = OV7670_ReadReg(handler, reg, &reg_val);
    if(retval != kStatus_OV7670_Success)
    {
        return kStatus_OV7670_Fail;
    }
    reg_val &= ~clr_mask;
    reg_val |= val;
    retval = OV7670_WriteReg(handler, reg, reg_val);
    if(retval != kStatus_OV7670_Success)
    {
        return kStatus_OV7670_Fail;
    }
    return kStatus_OV7670_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OV7670_ContrastAdj
 * Description   : Adjust the contrast of ov7670.
 *END**************************************************************************/
ov7670_status_t OV7670_ContrastAdj(ov7670_handler_t *handler, uint8_t val)
{
  uint8_t retval = 0;
  
  retval = OV7670_WriteReg(handler, OV7670_CONTRAS_CENTER_REG, 0x80);
  retval |= OV7670_ModifyReg(handler, OV7670_MTXS_REG, 0x80, 0x00);
  retval |= OV7670_WriteReg(handler, OV7670_CONTRAS_REG, val);
  if(retval != kStatus_I2C_Success)
  {
      return kStatus_OV7670_I2CFail;
  }
  return kStatus_OV7670_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OV7670_BrightnessAdj
 * Description   : Adjust the brightness of ov7670.
 *END**************************************************************************/
ov7670_status_t OV7670_BrightnessAdj(ov7670_handler_t *handler, uint8_t val)
{
  uint8_t retval = 0;
    
  retval = OV7670_WriteReg(handler, OV7670_BRIGHT_REG, val);
  if(retval != kStatus_I2C_Success)
  {
      return kStatus_OV7670_I2CFail;
  }
  return kStatus_OV7670_Success;
}


/*!
 * @brief OV7670 configuration.
 * @param @ref ov7670_handler_t structure.
 * @param @ref ov7670_config_t structure.
 */
ov7670_status_t OV7670_Configuration(ov7670_handler_t *handler, ov7670_config_t *ov7670_cfg)
{
  OV7670_OutputFormat(handler, ov7670_cfg->output_format);
  OV7670_Resolution(handler, &ov7670_cfg->resolution);
  uint32_t u32TempResolution = (ov7670_cfg->resolution.width<<16)|ov7670_cfg->resolution.heigth;
  ov7670_windowing_cfg_t *window_cfg;
  switch(u32TempResolution)
  {
  case 0x028001e0: 
    window_cfg = (ov7670_windowing_cfg_t*)&OV7670_WINDOW_VGA; break;
  case 0x014000f0: 
    window_cfg = (ov7670_windowing_cfg_t*)&OV7670_WINDOW_QVGA; break;
  case 0x00a00078: 
    window_cfg = (ov7670_windowing_cfg_t*)&OV7670_WINDOW_QQVGA; break;
  case 0x01600120: 
    window_cfg = (ov7670_windowing_cfg_t*)&OV7670_WINDOW_CIF; break;
  case 0x00b00090: 
    window_cfg = (ov7670_windowing_cfg_t*)&OV7670_WINDOW_QCIF; break;
  case 0x00580048: 
    window_cfg = (ov7670_windowing_cfg_t*)&OV7670_WINDOW_QQCIF; break;
  default: return kStatus_OV7670_Fail; // not supported resolution
  }
  OV7670_SetWindow(handler, window_cfg);
  OV7670_FrameRateAdjustment(handler, ov7670_cfg->frame_rate);
  
  if (ov7670_cfg->advanced_config == NULL)
  {
    OV7670_BandingFilterSelection(handler, (ov7670_filter_cfg_t*)&OV7670_FILTER_25FPS_50HZ);
    OV7670_NighMode(handler, (ov7670_night_mode_cfg_t*)&OV7670_NIGHT_MODE_DISABLED);
    OV7670_WhiteBalance(handler, (ov7670_white_balance_cfg_t*)&OV7670_WHITE_BALANCE_SIMPLE);
    OV7670_LightMode(handler, (ov7670_light_mode_cfg_t*)&OV7670_LIGHT_MODE_DISABLED);
    OV7670_ColorSaturation(handler, (ov7670_color_saturation_cfg_t*)&OV7670_COLOR_SATURATION_2PLUS);
    OV7670_SpecialEffects(handler, (ov7670_special_effect_cfg_t*)&OV7670_SPECIAL_EFFECT_DISABLED);
    OV7670_GammaCurveSlope(handler, (ov7670_gamma_curve_slope_cfg_t*)&OV7670_GAMMA_CURVE_SLOPE_DEFAULT);
  }
  else
  {
    OV7670_BandingFilterSelection(handler, ov7670_cfg->advanced_config->filter);
    OV7670_NighMode(handler, ov7670_cfg->advanced_config->night_mode);
    OV7670_WhiteBalance(handler, ov7670_cfg->advanced_config->white_balance);
    OV7670_LightMode(handler, ov7670_cfg->advanced_config->light_mode);
    OV7670_ColorSaturation(handler, ov7670_cfg->advanced_config->color_saturation);
    OV7670_SpecialEffects(handler, ov7670_cfg->advanced_config->special_effect);
    OV7670_GammaCurveSlope(handler, ov7670_cfg->advanced_config->gamma_curve_slope);
  }
  
  OV7670_ContrastAdj(handler, ov7670_cfg->contrast);
  OV7670_BrightnessAdj(handler, ov7670_cfg->brightness);
  OV7670_WriteReg(handler, 0xb0, 0x84); // ? because of colors
  OV7670_WriteReg(handler, 0xff, 0xff);
  
  return kStatus_OV7670_Success;
}


/*!
 * @brief OV7670 Output format configuration.
 * @param @ref ov7670_handler_t structure.
 * @param @ref ov7670_output_format_cfg_t structure.
 */
ov7670_status_t OV7670_OutputFormat(ov7670_handler_t *handler, ov7670_output_format_cfg_t *output_format_cfg)
{
  OV7670_ModifyReg(handler, OV7670_COM7_REG, 0x05, output_format_cfg->com7);
  OV7670_ModifyReg(handler, OV7670_RGB444_REG, 0x03, output_format_cfg->rgb444);
  OV7670_ModifyReg(handler, OV7670_COM15_REG, 0x30, output_format_cfg->com15);
  
  return kStatus_OV7670_Success;
}

/*!
 * @brief OV7670 Resolution configuration.
 * @param handler ov7670 handler structure.
 * @param @ref ov7670_resolution_t structure.
 */
ov7670_status_t OV7670_Resolution(ov7670_handler_t *handler, ov7670_resolution_t *resolution)
{
  ov7670_resolution_cfg_t *resolution_cfg;
  uint32_t u32TempResolution;
  
  u32TempResolution = (resolution->width<<16)|resolution->heigth;
  switch(u32TempResolution)
  {
  case 0x028001e0: 
    resolution_cfg = (ov7670_resolution_cfg_t*)&OV7670_RESOLUTION_VGA; break;
  case 0x014000f0: 
    resolution_cfg = (ov7670_resolution_cfg_t*)&OV7670_RESOLUTION_QVGA; break;
  case 0x00a00078: 
    resolution_cfg = (ov7670_resolution_cfg_t*)&OV7670_RESOLUTION_QQVGA; break;
  case 0x01600120: 
    resolution_cfg = (ov7670_resolution_cfg_t*)&OV7670_RESOLUTION_CIF; break;
  case 0x00b00090: 
    resolution_cfg = (ov7670_resolution_cfg_t*)&OV7670_RESOLUTION_QCIF; break;
  case 0x00580048: 
    resolution_cfg = (ov7670_resolution_cfg_t*)&OV7670_RESOLUTION_QQCIF; break;
  default: return kStatus_OV7670_Fail; // not supported resolution
  }
  
  OV7670_ModifyReg(handler, OV7670_COM7_REG, 0x38, resolution_cfg->com7);
  OV7670_ModifyReg(handler, OV7670_COM3_REG, 0x0c, resolution_cfg->com3);
  OV7670_WriteReg(handler, OV7670_COM14_REG, resolution_cfg->com14);
  OV7670_WriteReg(handler, OV7670_SCALING_XSC_REG, resolution_cfg->scaling_xsc);
  OV7670_WriteReg(handler, OV7670_SCALING_YSC_REG, resolution_cfg->scaling_ysc);
  OV7670_ModifyReg(handler, OV7670_SCALING_DCWCTR_REG, 0x33, resolution_cfg->scaling_dcwctr);
  OV7670_WriteReg(handler, OV7670_SCALING_PCLK_DIV_REG, resolution_cfg->scaling_pclk_div);
  OV7670_WriteReg(handler, OV7670_SCALING_PCLK_DELAY_REG, resolution_cfg->scaling_pclk_delay);
  
  /* Autotomatically set output window after resolution change                */
  OV7670_ModifyReg(handler, OV7670_TSLB_REG,0x01, 0x01);
  
  return kStatus_OV7670_Success;
}

/*!
 * @brief OV7670 Windowing configuration.
 * @param @ref ov7670_handler_t structure.
 * @param @ref ov7670_windowing_cfg_t structure.
 */
ov7670_status_t OV7670_SetWindow(ov7670_handler_t *handler, ov7670_windowing_cfg_t *windowing_cfg)
{
  OV7670_ModifyReg(handler, OV7670_TSLB_REG,0x01, 0x00);
  
  OV7670_WriteReg(handler, OV7670_HREF_REG, windowing_cfg->href);
  OV7670_WriteReg(handler, OV7670_HSTART_REG, windowing_cfg->hstart);
  OV7670_WriteReg(handler, OV7670_HSTOP_REG, windowing_cfg->hstop);
  OV7670_WriteReg(handler, OV7670_VREF_REG, windowing_cfg->vref);
  OV7670_WriteReg(handler, OV7670_VSTART_REG, windowing_cfg->vstart);
  OV7670_WriteReg(handler, OV7670_VSTOP_REG, windowing_cfg->vstop);
  
  return kStatus_OV7670_Success;
}

/*!
 * @brief OV7670 frame rate adjustment.
 * @param @ref ov7670_handler_t structure.
 * @param @ref ov7670_frame_rate_cfg_t structure.
 */
ov7670_status_t OV7670_FrameRateAdjustment(ov7670_handler_t *handler, ov7670_frame_rate_cfg_t *frame_rate_cfg)
{
  OV7670_WriteReg(handler, OV7670_CLKRC_REG, frame_rate_cfg->clkrc); OSA_TimeDelay(1);
  OV7670_WriteReg(handler, OV7670_DBLV_REG, frame_rate_cfg->dblv); OSA_TimeDelay(1);
  OV7670_WriteReg(handler, OV7670_EXHCH_REG, frame_rate_cfg->exhch); OSA_TimeDelay(1);
  OV7670_WriteReg(handler, OV7670_EXHCL_REG, frame_rate_cfg->exhcl); OSA_TimeDelay(1);
  OV7670_WriteReg(handler, OV7670_DM_LNL_REG, frame_rate_cfg->dm_lnl); OSA_TimeDelay(1);
  OV7670_WriteReg(handler, OV7670_DM_LNL_REG, frame_rate_cfg->dm_lnh); OSA_TimeDelay(1);
  
  return kStatus_OV7670_Success;
}

/*!
 * @brief OV7670 night mode configuration.
 * @param @ref ov7670_handler_t structure.
 * @param @ref ov7670_night_mode_cfg_t structure.
 */
ov7670_status_t OV7670_NighMode(ov7670_handler_t *handler, ov7670_night_mode_cfg_t *night_mode_cfg)
{
  OV7670_ModifyReg(handler, OV7670_COM11_REG, 0xe0, night_mode_cfg->com11); OSA_TimeDelay(1);
  
  return kStatus_OV7670_Success;
}

/*!
 * @brief OV7670 Banding filter selection configuration.
 * @param @ref ov7670_handler_t structure.
 * @param @ref ov7670_filter_cfg_t structure.
 */
ov7670_status_t OV7670_BandingFilterSelection(ov7670_handler_t *handler, ov7670_filter_cfg_t *filter_cfg) 
{
  OV7670_WriteReg(handler, OV7670_BD50ST_REG, filter_cfg->bd50st); //50Hz banding filter
  OV7670_WriteReg(handler, OV7670_BD60ST_REG, filter_cfg->bd60st); //60Hz banding filter
  OV7670_WriteReg(handler, OV7670_BD50MAX_REG, filter_cfg->bd50max); //x step for 50hz
  OV7670_WriteReg(handler, OV7670_BD60MAX_REG, filter_cfg->bd60max); //y step for 60hz
  OV7670_ModifyReg(handler, OV7670_COM11_REG, 0x1a, filter_cfg->com11); //Automatic Detect banding filter
  OV7670_ModifyReg(handler, OV7670_COM8_REG, 0x20, filter_cfg->com8); //banding filter enable
  
  return kStatus_OV7670_Success;
}

/*!
 * @brief OV7670 White balance configuration.
 * @param @ref ov7670_handler_t structure.
 * @param @ref ov7670_white_balance_cfg_t structure.
 */
ov7670_status_t OV7670_WhiteBalance(ov7670_handler_t *handler, ov7670_white_balance_cfg_t *white_balance_cfg)
{
  OV7670_ModifyReg(handler, OV7670_COM8_REG, 0x02, white_balance_cfg->com8); //AWB on/off
  OV7670_WriteReg(handler, OV7670_AWBCTR0_REG, white_balance_cfg->awbctr0);
  OV7670_WriteReg(handler, OV7670_AWBCTR1_REG, white_balance_cfg->awbctr1);
  OV7670_WriteReg(handler, OV7670_AWBCTR2_REG, white_balance_cfg->awbctr2);
  OV7670_WriteReg(handler, OV7670_AWBCTR3_REG, white_balance_cfg->awbctr3);
  OV7670_WriteReg(handler, OV7670_AWBC1_REG, white_balance_cfg->awbc1);
  OV7670_WriteReg(handler, OV7670_AWBC2_REG, white_balance_cfg->awbc2);
  OV7670_WriteReg(handler, OV7670_AWBC3_REG, white_balance_cfg->awbc3);
  OV7670_WriteReg(handler, OV7670_AWBC4_REG, white_balance_cfg->awbc4);
  OV7670_WriteReg(handler, OV7670_AWBC5_REG, white_balance_cfg->awbc5);
  OV7670_WriteReg(handler, OV7670_AWBC6_REG, white_balance_cfg->awbc6);
  OV7670_WriteReg(handler, 0x59, 0x91);
  OV7670_WriteReg(handler, 0x5a, 0x94);
  OV7670_WriteReg(handler, 0x5b, 0xaa);
  OV7670_WriteReg(handler, 0x5c, 0x71);
  OV7670_WriteReg(handler, 0x5d, 0x8d);
  OV7670_WriteReg(handler, 0x5e, 0x0f);
  OV7670_WriteReg(handler, 0x5f, 0xf0);
  OV7670_WriteReg(handler, 0x60, 0xf0);
  OV7670_WriteReg(handler, 0x61, 0xf0);
  OV7670_ModifyReg(handler, OV7670_COM16_REG, 0x08, white_balance_cfg->com16); //AWB gain on
  
  return kStatus_OV7670_Success;
}

/*!
 * @brief OV7670 Light mode configuration.
 * @param handler ov7670 handler structure.
 * @param @ref ov7670_light_mode_cfg_t structure.
 */
ov7670_status_t OV7670_LightMode(ov7670_handler_t *handler, ov7670_light_mode_cfg_t *light_mode_cfg)
{
  OV7670_ModifyReg(handler, OV7670_COM8_REG, 0xc5, light_mode_cfg->com8);
  OV7670_ModifyReg(handler, OV7670_COM9_REG, 0x7a, light_mode_cfg->com9);
  OV7670_WriteReg(handler, OV7670_RED_REG, light_mode_cfg->red);
  OV7670_WriteReg(handler, OV7670_GGAIN_REG, light_mode_cfg->green);
  OV7670_WriteReg(handler, OV7670_BLUE_REG, light_mode_cfg->blue);
  OV7670_WriteReg(handler, OV7670_GAIN_REG, 0x00); OSA_TimeDelay(1);
  
  // Exposure value
  OV7670_WriteReg(handler, OV7670_AECH_REG, 0x00); OSA_TimeDelay(1);
  OV7670_WriteReg(handler, OV7670_AECHH_REG, 0x00); OSA_TimeDelay(1);
  OV7670_ModifyReg(handler, OV7670_COM1_REG, 0x3, 0x00); OSA_TimeDelay(1);
  // AGC/AEC stable operation region configuration
  OV7670_WriteReg(handler, OV7670_AEW_REG, 0x75); OSA_TimeDelay(1);
  OV7670_WriteReg(handler, OV7670_AEB_REG, 0x63); OSA_TimeDelay(1);
  OV7670_WriteReg(handler, OV7670_VPT_REG, 0xd4); OSA_TimeDelay(1);
  
  return kStatus_OV7670_Success;
}

/*!
 * @brief OV7670 Color saturation configuration.
 * @param @ref ov7670_handler_t structure.
 * @param @ref ov7670_color_saturation_cfg_t structure.
 */
ov7670_status_t OV7670_ColorSaturation(ov7670_handler_t *handler, ov7670_color_saturation_cfg_t *color_saturation_cfg)
{
  OV7670_WriteReg(handler, OV7670_MTX1_REG, color_saturation_cfg->mtx1);
  OV7670_WriteReg(handler, OV7670_MTX2_REG, color_saturation_cfg->mtx2);
  OV7670_WriteReg(handler, OV7670_MTX3_REG, color_saturation_cfg->mtx3);
  OV7670_WriteReg(handler, OV7670_MTX4_REG, color_saturation_cfg->mtx4);
  OV7670_WriteReg(handler, OV7670_MTX5_REG, color_saturation_cfg->mtx5);
  OV7670_WriteReg(handler, OV7670_MTX6_REG, color_saturation_cfg->mtx6);
  OV7670_WriteReg(handler, OV7670_MTXS_REG, color_saturation_cfg->mtxs);
  OV7670_ModifyReg(handler, OV7670_COM16_REG, 0x02, color_saturation_cfg->com16);
  
  return kStatus_OV7670_Success;
}

/*!
 * @brief OV7670 Special effects configuration.
 * @param @ref ov7670_handler_t structure.
 * @param @ref ov7670_special_effect_cfg_t structure.
 */
ov7670_status_t OV7670_SpecialEffects(ov7670_handler_t *handler, ov7670_special_effect_cfg_t *special_effect_cfg)
{
  OV7670_ModifyReg(handler, OV7670_TSLB_REG, 0xfe, special_effect_cfg->tslb);
  OV7670_WriteReg(handler, OV7670_MANU_REG, special_effect_cfg->manu);
  OV7670_WriteReg(handler, OV7670_MANV_REG, special_effect_cfg->manv);
  
  return kStatus_OV7670_Success;
}

/*!
 * @brief OV7670 Configure the window by setting initial coordinates and 
 *               resolution.
 * @param @ref ov7670_handler_t structure.
 * @param @ref ov7670_window_start_point_t structure.
 * @param @ref ov7670_resolution_t structure.
 */
ov7670_status_t OV7670_SetWindowByCoordinates(ov7670_handler_t *handler, ov7670_window_start_point_t *start_point, ov7670_resolution_t *resolution)
{
  uint16_t u16Temp, u16Href, u16Vref;
  
  OV7670_ModifyReg(handler, OV7670_TSLB_REG,0x01, 0x00);
  
  u16Temp = start_point->hstart_coordinate + resolution->width;
  u16Href = (u16Temp&0x07);
  u16Href = u16Href<<3;
  u16Href |= (start_point->hstart_coordinate&0x07);
  u16Href |= 0xc0;
  OV7670_WriteReg(handler, OV7670_HREF_REG, (uint8_t)u16Href);
  u16Temp = u16Temp>>3;
  OV7670_WriteReg(handler, OV7670_HSTOP_REG, (uint8_t)u16Temp);
  u16Temp = ((start_point->hstart_coordinate&0x7f8)>>3);
  OV7670_WriteReg(handler, OV7670_HSTART_REG, (uint8_t)u16Temp);
  
  u16Temp = start_point->vstart_coordinate + resolution->heigth;
  u16Vref = (u16Temp&0x03);
  u16Vref = u16Vref<<2;
  u16Vref |= (start_point->vstart_coordinate&0x03);
  u16Vref &= 0xF0;
  OV7670_ModifyReg(handler, OV7670_VREF_REG, 0xc0, (uint8_t)u16Vref);
  u16Temp = u16Temp>>2;
  OV7670_WriteReg(handler, OV7670_VSTOP_REG, (uint8_t)u16Temp);
  u16Temp = ((start_point->vstart_coordinate&0x3fc)>>2);
  OV7670_WriteReg(handler, OV7670_VSTART_REG, (uint8_t)u16Temp);
  
  return kStatus_OV7670_Success;
}

/*!
 * @brief OV7670 Gamma curve slope configuration.
 * @param @ref ov7670_handler_t structure.
 * @param @ref ov7670_gamma_curve_slope_cfg_t structure.
 */
ov7670_status_t OV7670_GammaCurveSlope(ov7670_handler_t *handler, ov7670_gamma_curve_slope_cfg_t *gamma_curve_slope_cfg)
{
  OV7670_WriteReg(handler, OV7670_SLOP_REG, gamma_curve_slope_cfg->slope); 
  OV7670_WriteReg(handler, OV7670_GAM1_REG, gamma_curve_slope_cfg->gam1); 
  OV7670_WriteReg(handler, OV7670_GAM2_REG, gamma_curve_slope_cfg->gam2); 
  OV7670_WriteReg(handler, OV7670_GAM3_REG, gamma_curve_slope_cfg->gam3); 
  OV7670_WriteReg(handler, OV7670_GAM4_REG, gamma_curve_slope_cfg->gam4); 
  OV7670_WriteReg(handler, OV7670_GAM5_REG, gamma_curve_slope_cfg->gam5); 
  OV7670_WriteReg(handler, OV7670_GAM6_REG, gamma_curve_slope_cfg->gam6); 
  OV7670_WriteReg(handler, OV7670_GAM7_REG, gamma_curve_slope_cfg->gam7); 
  OV7670_WriteReg(handler, OV7670_GAM8_REG, gamma_curve_slope_cfg->gam8); 
  OV7670_WriteReg(handler, OV7670_GAM9_REG, gamma_curve_slope_cfg->gam9); 
  OV7670_WriteReg(handler, OV7670_GAM10_REG, gamma_curve_slope_cfg->gam10);
  OV7670_WriteReg(handler, OV7670_GAM11_REG, gamma_curve_slope_cfg->gam11);
  OV7670_WriteReg(handler, OV7670_GAM12_REG, gamma_curve_slope_cfg->gam12);
  OV7670_WriteReg(handler, OV7670_GAM13_REG, gamma_curve_slope_cfg->gam13);
  OV7670_WriteReg(handler, OV7670_GAM14_REG, gamma_curve_slope_cfg->gam14);
  OV7670_WriteReg(handler, OV7670_GAM15_REG, gamma_curve_slope_cfg->gam15);
  
  return kStatus_OV7670_Success;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
