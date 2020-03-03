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

#define flexbus_16b_ID 1

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


//-----------------------------------------------------------------------------
// FUNCTION:    Init_Flexbus_16b
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for initialization of this low level driver
//
// PARAMETERS:  none
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------
unsigned char Init_Flexbus_16b(void)
{
  #ifdef DISPLAY_MCU_USER_INIT
    DISPLAY_MCU_USER_INIT
  #endif

  FB_CSAR0 = FLEX_DC_ADDRESS;
  FB_CSMR0 = FLEX_ADRESS_MASK | FB_CSMR_V_MASK;
  FB_CSCR0 = FB_CSCR_BLS_MASK | FB_CSCR_AA_MASK | FB_CSCR_PS(2);

  return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    DeInit_Flexbus_16b
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for deinitialization of this low level driver
//
// PARAMETERS:  none
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------
unsigned char DeInit_Flexbus_16b(void)
{
   return 0;
}

//-----------------------------------------------------------------------------
// FUNCTION:    SendDataWord_Flexbus_16b
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function send the one 16 bit variable into LCD
//
// PARAMETERS:  unsigned short value    variable to send
//
// RETURNS:     none
//-----------------------------------------------------------------------------
void SendDataWord_Flexbus_16b(unsigned short value)
{
  *((unsigned short*)FLEX_BASE_ADDRESS) = value;
}

//-----------------------------------------------------------------------------
// FUNCTION:    SendCmdWord_Flexbus_16b
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function send the one 16 bit command into LCD
//
// PARAMETERS:  unsigned short cmd    command to send
//
// RETURNS:     none
//-----------------------------------------------------------------------------
void SendCmdWord_Flexbus_16b(unsigned short cmd)
{
  *((unsigned short*)FLEX_DC_ADDRESS) = cmd;
}


//-----------------------------------------------------------------------------
// FUNCTION:    ReadDataWord_Flexbus_16b
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)
//
// PARAMETERS:  none
//
// RETURNS:     unsigned short - the readed value
//
//-----------------------------------------------------------------------------
unsigned short ReadDataWord_Flexbus_16b(void)
{
  return *((unsigned short*)FLEX_BASE_ADDRESS);
}


//-----------------------------------------------------------------------------
// FUNCTION:    ReadCmdWord_Flexbus_16b
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function reads the one 16 bit command from LCD (if this function is supported)
//
// PARAMETERS:  none
//
// RETURNS:     unsigned short - the readed value
//
//-----------------------------------------------------------------------------
unsigned short ReadCmdWord_Flexbus_16b(void)
{
  return *((unsigned short*)FLEX_DC_ADDRESS);
}
