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
#include "wm8960.h"
#include "fsl_lpi2c_hal.h"

/*******************************************************************************
*Code
******************************************************************************/
static volatile uint32_t i2c_state = 0;

/*
* wm8960 register cache
* We can't read the WM8960 register space when we are
* using 2 wire for device control, so we cache them instead.
*/
static const uint16_t wm8960_reg[WM8960_CACHEREGNUM] = {
  0x0097, 0x0097, 0x0000, 0x0000,
  0x0000, 0x0008, 0x0000, 0x000a,
  0x01c0, 0x0000, 0x00ff, 0x00ff,
  0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x007b, 0x0100, 0x0032,
  0x0000, 0x00c3, 0x00c3, 0x01c0,
  0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000,
  0x0100, 0x0100, 0x0050, 0x0050,
  0x0050, 0x0050, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0040, 0x0000,
  0x0000, 0x0050, 0x0050, 0x0000,
  0x0002, 0x0037, 0x004d, 0x0080,
  0x0008, 0x0031, 0x0026, 0x00e9,
};

static uint16_t reg_cache[WM8960_CACHEREGNUM];

/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_TxInit
* Description   : Initialize the WM8960 Chip.
*                 Sample Rate 44.1KHz, MCLK 256*FS, SYSCLK from MCLK
*                 HP L/R out
*END**************************************************************************/
wolfson_status_t WOLFSON_TxInit(void)
{
  memcpy(reg_cache, wm8960_reg, sizeof(wm8960_reg));
  
  /* Reset all registers */
  WOLFSON_WriteReg(WM8960_RESET, 0x00);
  
  /*
  * VMID=50K, Enable VREF, AINL, AINR, ADCL and ADCR
  * I2S_IN (bit 0), I2S_OUT (bit 1), DAP (bit 4), DAC (bit 5), ADC (bit 6) are powered on
  */
  WOLFSON_WriteReg(WM8960_POWER1, 0xFC);
  
  /*
  * Enable DACL, DACR, LOUT1, ROUT1, PLL down
  */
  WOLFSON_WriteReg(WM8960_POWER2, 0x1E0);
  
  /* Enable left and right channel input PGA, left and right output mixer */
  WOLFSON_WriteReg(WM8960_POWER3, 0x3C);
  
  /* Configure SYS_FS clock to 44.1kHz, MCLK_FREQ to 256*Fs, SYSCLK derived from MCLK input */
  WOLFSON_WriteReg(WM8960_CLOCK1, 0x00);
  
  /*
  * Audio data length = 32bit, Left justified data format
  */
  WOLFSON_WriteReg(WM8960_IFACE1, 0x0D);//32bit
  //    WOLFSON_WriteReg(WM8960_IFACE1, 0x01); //16bit
  
  /*
  * LMICBOOST = 0dB, Connect left and right PGA to left and right Input Boost Mixer
  */
  WOLFSON_WriteReg(WM8960_LINPATH, 0x08);
  WOLFSON_WriteReg(WM8960_RINPATH, 0x08);
  
  /*
  * Left and right input boost, LIN3BOOST and RIN3BOOST = 0dB
  */
  WOLFSON_WriteReg(WM8960_INBMIX1, 0x50);
  WOLFSON_WriteReg(WM8960_INBMIX2, 0x50);
  
  /*
  * Left DAC and LINPUT3 to left output mixer, LINPUT3 left output mixer volume = 0dB
  */
  WOLFSON_WriteReg(WM8960_LOUTMIX, 0x180);
  
  /*
  * Right DAC and RINPUT3 to right output mixer, RINPUT3 right output mixer volume = 0dB
  */
  WOLFSON_WriteReg(WM8960_ROUTMIX, 0x180);
  
  /*
  * Left input boost mixer to left output mixer, volume = 0dB
  */
  WOLFSON_WriteReg(WM8960_BYPASS1, 0x80);
  
  /*
  * Right input boost mixer to right output mixer, volume = 0dB
  */
  WOLFSON_WriteReg(WM8960_BYPASS2, 0x80);
  
  /*
  * ADC volume, 0dB
  */
  WOLFSON_WriteReg(WM8960_LADC, 0x1C3); //0dB
  WOLFSON_WriteReg(WM8960_RADC, 0x1C3); //0dB    
  
  /*
  * Digital DAC volume, 0dB
  */
  WOLFSON_WriteReg(WM8960_LDAC, 0x1FF); //0dB
  WOLFSON_WriteReg(WM8960_RDAC, 0x1FF); //0dB
  
  /*
  * Headphone volume, LOUT1 and ROUT1, 0dB
  */
  WOLFSON_WriteReg(WM8960_LOUT1, 0x179);
  WOLFSON_WriteReg(WM8960_ROUT1, 0x179);
  
  /* Unmute DAC. */
  WOLFSON_WriteReg(WM8960_DACCTL1, 0x0000);
  
  return kStatus_WOLFSON_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_RxInit
* Description   : Initialize the WM8960 Chip for Rx.
*                 Sample Rate 8KHz, MCLK 256*FS, SYSCLK from MCLK
*                 INPUT3 L/R IN
*END**************************************************************************/
wolfson_status_t WOLFSON_RxInit(void)
{
  memcpy(reg_cache, wm8960_reg, sizeof(wm8960_reg));
  
  /* Reset all registers */
  WOLFSON_WriteReg(WM8960_RESET, 0x00);
  
  /*
  * VMID=50K, Enable VREF, AINL, AINR, ADCL and ADCR
  */
  WOLFSON_WriteReg(WM8960_POWER1, 0xFC);
  
  /* Disable DACL, DACR, LOUT1, ROUT1, PLL up */
  WOLFSON_WriteReg(WM8960_POWER2, 0x01);
  
  /* Enable left and right channel input PGA, disable output mixer */
  WOLFSON_WriteReg(WM8960_POWER3, 0x30);
  
  /*
  * Configure ADC clock to 8kHz, MCLK_FREQ to 256*Fs, SYSCLK directly from MCLK
  * The MCLK input from CPU should be 12.288MHz
  */
  WOLFSON_WriteReg(WM8960_CLOCK1, 0x180);
  
  /* Configure the BCLKDIV, not used in Slave mode */
  WOLFSON_WriteReg(WM8960_CLOCK2, 0x00);
  
  /* Audio data length = 32bit, slave mode, Left justified data format */
  WOLFSON_WriteReg(WM8960_IFACE1, 0x0D);
  
  /* ADCLRC/GPIO1 pin functions as GPIO input */
  //WOLFSON_WriteReg(WM8960_IFACE2, 0x40);
  
#if 0
  /*
  * MCLK = 12MHz, SYSCLK = 11.2896MHz
  * SYSClK to GPIO clock divide ratio = 1, Fractional mode, Divide by 2, PLLN = 7
  */
  WOLFSON_WriteReg(WM8960_PLL1, 0x37);
  
  /*
  * Assume MCLK = 12MHz, use on-chip PLL to generate 11.2896MHz Sysclk
  * PLLK = int(2^24 x (7.5264 - 7)) = 8831526 = 86C226h
  */
  WOLFSON_WriteReg(WM8960_PLL2, 0x86);
  WOLFSON_WriteReg(WM8960_PLL3, 0xC2);
  WOLFSON_WriteReg(WM8960_PLL4, 0x26);
#endif
  
  /* Disable the L/R PGA for ADC */
  WOLFSON_WriteReg(WM8960_LINPATH, 0x00);
  WOLFSON_WriteReg(WM8960_RINPATH, 0x00);
  
  /* Left and right input boost, LIN3BOOST and RIN3BOOST = 0dB */
  WOLFSON_WriteReg(WM8960_INBMIX1, 0x50);
  WOLFSON_WriteReg(WM8960_INBMIX2, 0x50);
  
  /*
  * ADC volume, 0dB
  */
  WOLFSON_WriteReg(WM8960_LADC, 0x1C3); //0dB
  WOLFSON_WriteReg(WM8960_RADC, 0x1C3); //0dB    
  
  /* Unmute DAC. */
  WOLFSON_WriteReg(WM8960_DACCTL1, 0x0000);
  
  return kStatus_WOLFSON_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_SetMasterSlave
* Description   : Set master or slave mode of WM8960.
* This function would set MS bit in WM8960_IFACE1. 
*END**************************************************************************/
wolfson_status_t WOLFSON_SetMasterSlave(bool master)
{
  if(master == 1)
  {
    WOLFSON_ModifyReg(WM8960_IFACE1, WM8960_IFACE1_MS_MASK, WM8960_IFACE1_MS(WM8960_IFACE1_MASTER));
  }
  else
  {
    WOLFSON_ModifyReg(WM8960_IFACE1, WM8960_IFACE1_MS_MASK, WM8960_IFACE1_MS(WM8960_IFACE1_SLAVE));
  }
  return kStatus_WOLFSON_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_SetModuleCmd
* Description   : Enable modules in WM8960.
* This function would open both digital and analog switch.
*END**************************************************************************/
wolfson_status_t WOLFSON_SetModuleCmd(wolfson_module_t module, bool isEnabled)
{
  wolfson_status_t ret = kStatus_WOLFSON_Success;
  switch(module)
  {
  case kWolfsonModuleADC:
    WOLFSON_ModifyReg(WM8960_POWER1, WM8960_POWER1_ADCL_MASK, 
                      ((uint16_t)isEnabled << WM8960_POWER1_ADCL_SHIFT));
    WOLFSON_ModifyReg(WM8960_POWER1, WM8960_POWER1_ADCR_MASK, 
                      ((uint16_t)isEnabled << WM8960_POWER1_ADCR_SHIFT));
    break;
  case kWolfsonModuleDAC:
    WOLFSON_ModifyReg(WM8960_POWER2, WM8960_POWER2_DACL_MASK, 
                      ((uint16_t)isEnabled << WM8960_POWER2_DACL_SHIFT));
    WOLFSON_ModifyReg(WM8960_POWER2, WM8960_POWER2_DACR_MASK, 
                      ((uint16_t)isEnabled << WM8960_POWER2_DACR_SHIFT));
    break;
  case kWolfsonModuleVREF:
    WOLFSON_ModifyReg(WM8960_POWER1, WM8960_POWER1_VREF_MASK, 
                      ((uint16_t)isEnabled << WM8960_POWER1_VREF_SHIFT));
    break;
  case kWolfsonModuleLineIn: 	
    WOLFSON_ModifyReg(WM8960_POWER1, WM8960_POWER1_AINL_MASK,
                      ((uint16_t)isEnabled << WM8960_POWER1_AINL_SHIFT));
    WOLFSON_ModifyReg(WM8960_POWER1, WM8960_POWER1_AINR_MASK,
                      ((uint16_t)isEnabled << WM8960_POWER1_AINR_SHIFT));
    break;         
  case kWolfsonModuleLineOut:
    WOLFSON_ModifyReg(WM8960_POWER2, WM8960_POWER2_LOUT1_MASK,
                      ((uint16_t)isEnabled << WM8960_POWER2_LOUT1_SHIFT));
    WOLFSON_ModifyReg(WM8960_POWER2, WM8960_POWER2_ROUT1_MASK,
                      ((uint16_t)isEnabled << WM8960_POWER2_ROUT1_SHIFT));			
    break;   
  case kWolfsonModuleSpeaker: 	
    WOLFSON_ModifyReg(WM8960_POWER2, WM8960_POWER2_SPKL_MASK,
                      ((uint16_t)isEnabled << WM8960_POWER2_SPKL_SHIFT));
    WOLFSON_ModifyReg(WM8960_POWER2, WM8960_POWER2_SPKR_MASK,
                      ((uint16_t)isEnabled << WM8960_POWER2_SPKR_SHIFT));
    break;           		
  default:
    ret = kStatus_WOLFSON_Fail;
    break;
  }
  return ret;
}

/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_SetDataRoute
* Description   : Set the audio data route in WM8960.
* This function would configure data route of WM8960.
*END**************************************************************************/
wolfson_status_t WOLFSON_SetDataRoute(wolfson_route_t route)
{
  wolfson_status_t ret = kStatus_WOLFSON_Success;
  switch(route)
  {
  case kWolfsonRouteBypass:
    /* Bypass means from line-in to HP*/
    /*
    * Left LINPUT3 to left output mixer, LINPUT3 left output mixer volume = 0dB
    */
    WOLFSON_WriteReg(WM8960_LOUTMIX, 0x80);
    
    /*
    * Right RINPUT3 to right output mixer, RINPUT3 right output mixer volume = 0dB
    */
    WOLFSON_WriteReg(WM8960_ROUTMIX, 0x80);    
    break;
  case kWolfsonRoutePlayback:
    /* Data route I2S_IN-> DAC-> HP */
    /*
    * Left DAC to left output mixer, LINPUT3 left output mixer volume = 0dB
    */
    WOLFSON_WriteReg(WM8960_LOUTMIX, 0x100);
    
    /*
    * Right DAC to right output mixer, RINPUT3 right output mixer volume = 0dB
    */
    WOLFSON_WriteReg(WM8960_ROUTMIX, 0x100);          
    break;
  case kWolfsonRoutePlaybackandRecord:
    /* I2S IN->DAC->HP  LINE_IN->ADC->I2S_OUT */
    /*
    * Left and right input boost, LIN3BOOST and RIN3BOOST = 0dB
    */
    WOLFSON_WriteReg(WM8960_INBMIX1, 0x50);
    WOLFSON_WriteReg(WM8960_INBMIX2, 0x50);    
    /*
    * Left DAC to left output mixer, LINPUT3 left output mixer volume = 0dB
    */
    WOLFSON_WriteReg(WM8960_LOUTMIX, 0x100);
    
    /*
    * Right DAC to right output mixer, RINPUT3 right output mixer volume = 0dB
    */
    WOLFSON_WriteReg(WM8960_ROUTMIX, 0x100);              
    break;
  case kWolfsonRoutePlaybackwithDAP:
    /* I2S_IN->DAP->DAC->HP */
    break;
  case kWolfsonRoutePlaybackwithDAPandRecord:
    /* I2S_IN->DAP->DAC->HP,  LINE_IN->ADC->I2S_OUT */
    break;
  case kWolfsonRouteRecord:
    /* LINE_IN->ADC->I2S_OUT */        
    /*
    * Left and right input boost, LIN3BOOST and RIN3BOOST = 0dB
    */
    WOLFSON_WriteReg(WM8960_INBMIX1, 0x50);
    WOLFSON_WriteReg(WM8960_INBMIX2, 0x50);          
    break;
  default:
    ret = kStatus_WOLFSON_Fail;
    break;
  }
  return ret;
}

/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_SetProtocol
* Description   : Set the audio transfer protocol.
* This function would configure protocol of audio transfer.
*END**************************************************************************/
wolfson_status_t WOLFSON_SetProtocol(wolfson_protocol_t protocol)
{
  wolfson_status_t ret = kStatus_WOLFSON_Success;
  switch(protocol)
  {
  case kWolfsonBusI2S:
    WOLFSON_ModifyReg(WM8960_IFACE1, WM8960_IFACE1_FORMAT_MASK, WM8960_IFACE1_FORMAT(WM8960_IFACE1_FORMAT_I2S));
    break;
  case kWolfsonBusI2SLeft:
    WOLFSON_ModifyReg(WM8960_IFACE1, WM8960_IFACE1_FORMAT_MASK, WM8960_IFACE1_FORMAT(WM8960_IFACE1_FORMAT_LJ));        
    break;
  case kWolfsonBusI2SRight:
    WOLFSON_ModifyReg(WM8960_IFACE1, WM8960_IFACE1_FORMAT_MASK, WM8960_IFACE1_FORMAT(WM8960_IFACE1_FORMAT_RJ));
    break;
  case kWolfsonBusPCMA:
    WOLFSON_ModifyReg(WM8960_IFACE1, WM8960_IFACE1_FORMAT_MASK, WM8960_IFACE1_FORMAT(WM8960_IFACE1_FORMAT_DSP));
    WOLFSON_ModifyReg(WM8960_IFACE1, WM8960_IFACE1_LRP_MASK, WM8960_IFACE1_LRP(WM8960_IFACE1_DSP_MODEA));			
    break;
  case kWolfsonBusPCMB:
    WOLFSON_ModifyReg(WM8960_IFACE1, WM8960_IFACE1_FORMAT_MASK, WM8960_IFACE1_FORMAT(WM8960_IFACE1_FORMAT_DSP));
    WOLFSON_ModifyReg(WM8960_IFACE1, WM8960_IFACE1_LRP_MASK, WM8960_IFACE1_LRP(WM8960_IFACE1_DSP_MODEB));        
    break;
  default:
    ret = kStatus_WOLFSON_Fail;
    break;
  }
  WOLFSON_ModifyReg(WM8960_IFACE1, WM8960_IFACE1_WL_MASK, WM8960_IFACE1_WL(WM8960_IFACE1_WL_32BITS));
  return ret;
}


/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_SetVolume
* Description   : Set volume of WM8960 components.
* This function would set components' volume as expected.
*END**************************************************************************/
wolfson_status_t WOLFSON_SetVolume(wolfson_module_t module, uint32_t volume)
{
  uint16_t vol = 0;
  wolfson_status_t ret = kStatus_WOLFSON_Success;
  switch(module)
  {
  case kWolfsonModuleADC:
    vol = 0x100 | volume;
    ret = WOLFSON_WriteReg(WM8960_LADC, vol);
    ret = WOLFSON_WriteReg(WM8960_RADC, vol);
    break;
  case kWolfsonModuleDAC:
    vol = 0x100 | volume;
    ret = WOLFSON_WriteReg(WM8960_LDAC, vol);
    ret = WOLFSON_WriteReg(WM8960_RDAC, vol);
    break;
  case kWolfsonModuleHP:
    vol = 0x100 | volume;
    ret = WOLFSON_WriteReg(WM8960_LOUT1, vol);
    ret = WOLFSON_WriteReg(WM8960_ROUT1, vol);
    break;
  case kWolfsonModuleLineIn:
    vol = 0x100 | volume;
    ret = WOLFSON_WriteReg(WM8960_LINVOL, vol);
    ret = WOLFSON_WriteReg(WM8960_RINVOL, vol);
    break;
  default:
    ret = kStatus_WOLFSON_Fail;
    break;
  }
  return ret;
}

/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_GetVolume
* Description   : Get volume of WM8960 components.
* This function would set components' volume as expected.
*END**************************************************************************/
uint32_t WOLFSON_GetVolume(wolfson_module_t module)
{
  uint16_t vol = 0;
  switch(module)
  {
  case kWolfsonModuleADC:
    WOLFSON_ReadReg(WM8960_LADC, &vol);
    vol &= 0xFF;
    break;
  case kWolfsonModuleDAC:
    WOLFSON_ReadReg(WM8960_LDAC, &vol);
    vol &= 0xFF;
    break;
  case kWolfsonModuleHP:
    WOLFSON_ReadReg(WM8960_LOUT1, &vol);
    vol &= 0x7F;
    break;
  case kWolfsonModuleLineOut:
    WOLFSON_ReadReg(WM8960_LINVOL, &vol);
    vol &= 0x3F;
    break;
  default:
    vol = 0;
    break;
  }
  return vol;
}


/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_SetMuteCmd
* Description   : Mute components in WM8960.
* This function would mute components as expected.
*END**************************************************************************/
wolfson_status_t WOLFSON_SetMuteCmd(wolfson_module_t module, bool isEnabled)
{
  wolfson_status_t ret = kStatus_WOLFSON_Success;
  switch(module)
  {
  case kWolfsonModuleADC:
    /*
    * Digital Mute
    */
    if(isEnabled)
    {
      ret = WOLFSON_WriteReg(WM8960_LADC, 0x100);
      ret = WOLFSON_WriteReg(WM8960_RADC, 0x100);
    }
    else
    {
      ret = WOLFSON_WriteReg(WM8960_LADC, 0x1C3); //0dB
      ret = WOLFSON_WriteReg(WM8960_RADC, 0x1C3); //0dB
    }
    break;
  case kWolfsonModuleDAC:
    if(isEnabled)
    {
      ret = WOLFSON_WriteReg(WM8960_LDAC, 0x100);
      ret = WOLFSON_WriteReg(WM8960_RDAC, 0x100);
    }
    else
    {
      ret = WOLFSON_WriteReg(WM8960_LDAC, 0x1FF); //0dB
      ret = WOLFSON_WriteReg(WM8960_RDAC, 0x1FF); //0dB
    }
    break;
  case kWolfsonModuleHP:
    /*
    * Analog mute
    */
    if(isEnabled)
    {
      ret = WOLFSON_WriteReg(WM8960_LOUT1, 0x100);
      ret = WOLFSON_WriteReg(WM8960_ROUT1, 0x100);
    }
    else
    {
      ret = WOLFSON_WriteReg(WM8960_LOUT1, 0x179); //0dB
      ret = WOLFSON_WriteReg(WM8960_ROUT1, 0x179);
    }
    break;
  case kWolfsonModuleLineOut:
    break;
  default:
    ret = kStatus_WOLFSON_Fail;
    break;
  }
  return ret;
}

/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_ConfigDataFormat
* Description   : Configure the audio data format of the WM8960.
* This function would configure the sample rate and the data length.
*END**************************************************************************/
wolfson_status_t WOLFSON_ConfigDataFormat
(
uint32_t mclk, 
uint32_t sample_rate, 
uint8_t bits
)
{
  wolfson_status_t retval;
  
  // assume the MCLK in is 12MHz
  if (sample_rate == 44100)
  {
    // SYSCLK should be 11.2896MHz
  }
  else
  {
    // SYSCLK should be 12.288MHz
  }
  
  switch(sample_rate)
  {
  case 8000:
    retval = WOLFSON_WriteReg(WM8960_CLOCK1, 0x1B0);
    break;     
  case 12000:
    retval = WOLFSON_WriteReg(WM8960_CLOCK1, 0x120);
    break;    
  case 16000:
    retval = WOLFSON_WriteReg(WM8960_CLOCK1, 0xD8);
    break;  
  case 24000:
    retval = WOLFSON_WriteReg(WM8960_CLOCK1, 0x90);
    break;   
  case 32000:
    retval = WOLFSON_WriteReg(WM8960_CLOCK1, 0x48);
    break;           
  case 44100:
    retval = WOLFSON_WriteReg(WM8960_CLOCK1, 0xD8);
    break;         
  case 48000:
    retval = WOLFSON_WriteReg(WM8960_CLOCK1, 0x00);
    break;                   
  default:
    retval = kStatus_WOLFSON_Fail;
    break;
  }
  
  /*
  * Slave mode (MS = 0), LRP = 0, 32bit WL, left justified (FORMAT[1:0]=0b01)
  */
  switch(bits)
  {
  case 16:
    retval = WOLFSON_WriteReg(WM8960_IFACE1, 0x01);
    break;
  case 20:
    retval = WOLFSON_WriteReg(WM8960_IFACE1, 0x05);
    break;
  case 24:
    retval = WOLFSON_WriteReg(WM8960_IFACE1, 0x09);
    break;
  case 32:
    retval = WOLFSON_WriteReg(WM8960_IFACE1, 0x0D);
    break;
  default:
    retval = kStatus_WOLFSON_Fail;
    break;
  }
  
  return retval;
}

/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_WriteReg
* Description   : Write the specified register of WM8960.
* The writing process is through I2C.
*END**************************************************************************/
wolfson_status_t WOLFSON_WriteReg(uint8_t reg, uint16_t val)
{
  uint8_t cmd,buff;
  lpi2c_status_t ret;
  
  /* The register address */
  cmd = (reg << 1) | ((val >> 8) & 0x0001);
  /* Data */
  buff = val & 0xFF;  
  
  reg_cache[reg] = val;
  
  ret = LPI2C_HAL_MasterSendDataPolling(WM8960_I2C_BASE,
                                        WM8960_I2C_ADDR, &cmd, 1,
                                        &buff, 1);
  if(ret != kStatus_LPI2C_Success)
  {
    return kStatus_WOLFSON_I2CFail;
  }
  return kStatus_WOLFSON_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_ReadReg
* Description   : Read the specified register value of WM8960.
* The reading process is through I2C.
*END**************************************************************************/
wolfson_status_t WOLFSON_ReadReg(uint8_t reg, uint16_t *val)
{
  if(reg > WM8960_CACHEREGNUM)
  {
    return kStatus_WOLFSON_Fail;
  }
  
  *val = reg_cache[reg];
  
  return kStatus_WOLFSON_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : WOLFSON_ModifyReg
* Description   : Modify the specified register value of WM8960.
* The modify process is through I2C.
*END**************************************************************************/
wolfson_status_t WOLFSON_ModifyReg(uint8_t reg, uint16_t mask, uint16_t val)
{
  wolfson_status_t retval;	
  uint16_t reg_val;
  
  retval = WOLFSON_ReadReg(reg, &reg_val);
  if(retval != kStatus_WOLFSON_Success)
  {
    return kStatus_WOLFSON_Fail;
  }
  
  reg_val &= ~mask;
  reg_val |= val;	
  retval = WOLFSON_WriteReg(reg, reg_val);
  if(retval != kStatus_WOLFSON_Success)
  {
    return kStatus_WOLFSON_Fail;
  }	
  return kStatus_WOLFSON_Success;
}

/*******************************************************************************
*EOF
******************************************************************************/

