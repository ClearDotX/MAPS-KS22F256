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

#define ssd1289_ID 1

#include "ssd1289.h"
#include "flexbus_16b.h"

/******************************************************************************
* Macros
******************************************************************************/

/******************************************************************************
* Internal function prototypes
******************************************************************************/


/**************************************************************//*!
*
* Global variables
*
******************************************************************/

/**************************************************************//*!
*
* Local variables
*
******************************************************************/

/**************************************************************//*!
*
* Functions bodies
*
******************************************************************/
// sends Command/Data sequence word (16 bit) or array with words - first 1 command, then 1 x data
void Command_Data(const unsigned short data[], unsigned short count)
{
  int i;

  for (i=0; i<count; i+=2)
  {
    (void)SendCmdWord_Flexbus_16b(data[i]);
    (void)SendDataWord_Flexbus_16b(data[i+1]);
  }
}

//-----------------------------------------------------------------------------
// FUNCTION:    Init_SSD1289
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for initialization of this low level driver
//
// PARAMETERS:  none
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------
unsigned char Init_SSD1289(void)
{
  const uint16_t init_data[] =
  {
      0x0000,  0x0001,

      0x0003,  0xAEAC, 0x000C,  0x0007, 0x000D,  0x000F, 0x000E,  0x2900, 0x001E,  0x00B3,

      0x0001,  0x2B3F, 0x0002,  0x0600, 0x0010,  0x0000, 0x0011,  0x60B0,

      0x0005,  0x0000, 0x0006,  0x0000, 0x0016,  0xEF1C, 0x0017,  0x0003, 0x0007,  0x0233,
      0x000B,  0x5312, 0x000F,  0x0000,

      0x0041,  0x0000, 0x0042,  0x0000, 0x0048,  0x0000, 0x0049,  0x013F, 0x0044,  0xEF00,
      0x0045,  0x0000, 0x0046,  0x013F, 0x004A,  0x0000, 0x004B,  0x0000,

      0x0030,  0x0707, 0x0031,  0x0704, 0x0032,  0x0204, 0x0033,  0x0201, 0x0034,  0x0203,
      0x0035,  0x0204, 0x0036,  0x0204, 0x0037,  0x0502, 0x003A,  0x0302, 0x003B,  0x0500,
      0x0023 , 0x0000, 0x0024 , 0x0000
  }  ;

  if(!Init_Flexbus_16b())
    return 0;

  Command_Data(&init_data[0], 2);
  OSA_TimeDelay(15);
  Command_Data(&init_data[2], 10);   
  OSA_TimeDelay(15);
  Command_Data(&init_data[12], 8); 
  OSA_TimeDelay(150);
  Command_Data(&init_data[20], 14);  
  OSA_TimeDelay(20);
  Command_Data(&init_data[34], 18);  
  OSA_TimeDelay(20);
  Command_Data(&init_data[52], 24);  
  OSA_TimeDelay(31);

  return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    DeInit_SSD1289
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for deinitialization of this low level driver
//
// PARAMETERS:  none
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
 //-----------------------------------------------------------------------------
unsigned char DeInit_SSD1289(void)
{
  return 0;
}

//-----------------------------------------------------------------------------
// FUNCTION:    SetWindow_SSD1289
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function sets the logic window in memory of LCD driver
//
// PARAMETERS:  unsigned int x1 - left cordination of logic window
//              unsigned int y1 - top cordination of logic window
//              unsigned int x2 - right cordination of logic window
//              unsigned int y2 - bottom cordination of logic window
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
  //-----------------------------------------------------------------------------
unsigned char SetWindow_SSD1289(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
  uint16_t x1_x2;
  uint16_t Addr1, Addr2;

  //Addr1 = (uint16_t)(y2 - (y1<<1));
  Addr1 = (unsigned short)(SCREEN_SIZE_SHORTER_SIDE - 1 - y1);
  Addr2 = x1;
  //x1_x2 = (uint16_t)((Addr1<<8) - y1);
  x1_x2 = (unsigned short)((Addr1<<8) + (SCREEN_SIZE_SHORTER_SIDE - 1 - y2));
  y1 = x1;
  y2 = x2;

  //Set Window
  (void)SendCmdWord_Flexbus_16b(0x0044);
  (void)SendDataWord_Flexbus_16b((unsigned short)x1_x2);
  (void)SendCmdWord_Flexbus_16b(0x0045);
  (void)SendDataWord_Flexbus_16b((unsigned short)y1);
  (void)SendCmdWord_Flexbus_16b(0x0046);
  (void)SendDataWord_Flexbus_16b((unsigned short)y2);

 // Set Start Address counter
  (void)SendCmdWord_Flexbus_16b(0x004e);
  (void)SendDataWord_Flexbus_16b((unsigned short)Addr1);
  (void)SendCmdWord_Flexbus_16b(0x004f);
  (void)SendDataWord_Flexbus_16b((unsigned short)Addr2);
  (void)SendCmdWord_Flexbus_16b(0x0022);

  return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    SetOrientation_SSD1289
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function set the new orientation of LCD
//
// PARAMETERS:  LCD_ORIENTATION_TYPE orientation    the requested new orientation
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------
unsigned char SetOrientation_SSD1289(LCD_ORIENTATION_TYPE orientation)
{
  uint16_t LCD_EntryMode = 0;

  switch(orientation)
  {
  case LCD_PORTRAIT:
    LCD_EntryMode = 0x60b0; break;
  case LCD_PORTRAIT180:
    LCD_EntryMode = 0x6080; break;
  case LCD_LANDSCAPE:
    LCD_EntryMode = 0x60a8; break;
  case LCD_LANDSCAPE180:
    LCD_EntryMode = 0x6098; break;
  default: break;
  }
  
  (void)SendCmdWord_Flexbus_16b(0x0011);
  (void)SendDataWord_Flexbus_16b(LCD_EntryMode);

  return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    ClearScreen_SSD1289
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function clear LCD to the specified color
//
// PARAMETERS:  color - color value in RGB565 format
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------
unsigned char ClearScreen_SSD1289(uint16_t color)
{
  uint32_t i;
  
  (void)SetWindow_SSD1289(0, 0, (SCREEN_SIZE_LONGER_SIDE-1), (SCREEN_SIZE_SHORTER_SIDE-1));
  for (i = 0; i < SCREEN_PIXEL_COUNT; i++)
  {
    Send_PixelColor_SSD1289(color);
  }

  return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    Send_PixelColor_SSD1289
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function send the one pixel (Word) into LCD
//
// PARAMETERS:  unsigned short Value    value of pixel color
//
// RETURNS:     none
//-----------------------------------------------------------------------------
void Send_PixelColor_SSD1289(uint16_t value)
{
  SendDataWord_Flexbus_16b((unsigned short)value);
}

//-----------------------------------------------------------------------------
// FUNCTION:    Read_PixelColor_SSD1289
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function reads the one Word(Pixel) from LCD (if this function is supported)
//
// PARAMETERS:  none
//
// RETURNS:     unsigned short - the readed value
//
//-----------------------------------------------------------------------------
uint16_t Get_PixelColor_SSD1289(void)
{
  return ReadDataWord_Flexbus_16b();
}
