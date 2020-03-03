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
 
#include "app_flexio_ov7670.h"

/* callback functions prototypes. */
static void porta_callback(uint32_t pin);

/* module function prototypes. */
static void FlexIO_Init(void);
static void eDma_Init(edma_channel_indicator_t channel, uint8_t source, edma_transfer_config_t *edma_config);
static void eDma_Reset(edma_channel_indicator_t channel, edma_transfer_config_t *edma_config);

/*  System clock configuration structure initialization. */
const clock_manager_user_config_t g_RunClockConfig = HS_RUN_CLOCK_CONFIGURATION;


#if defined(__IAR_SYSTEMS_ICC__) /* IAR */

#pragma data_alignment=16
  #pragma location = 0x20000000
  static volatile uint16_t u16CameraFrameBuffer[OV7670_FRAME_PIXELS];
#pragma data_alignment=16

#elif defined(__ARMCC_VERSION)   /* KEIL */

__align(16)
  static uint16_t u16CameraFrameBuffer[OV7670_FRAME_PIXELS] __attribute__((section(".ARM.__at_0x20000000")));
__align(16)

#else

#error "A memory for u16CameraFrameBuffer with sizeof OV7670_FRAME_PIXELS should be reserved."

#endif

static volatile uint32_t u32AddrCameraFrameBuffer = (uint32_t)&u16CameraFrameBuffer;
  
/* OV7670 configuration structures. */
static uint8_t receiveBuff = 0xFF;
static uint8_t sendBuff = 0xFF;
static ov7670_handler_t ov7670_handler = OV7670_HANDLER;
static ov7670_advanced_config_t ov7670_advanced_config = OV7670_ADVANCED_CONFIGURATION;
const ov7670_config_t ov7670_config = OV7670_CONFIGURATION;

/* FlexIO configuration structures. */
flexio_camera_dev_t gFlexioCameraDevStruct =
{
    .flexioBase = FLEXIO0,
    .datPinStartIdx = 24U, /* fxio_pin 24 -31 are used. */
    .pclkPinIdx = 1U,      /* fxio_pin 1 is used as pclk pin. */
    .hrefPinIdx = 18U,     /* flexio_pin 18 is used as href pin. */
    .shifterStartIdx = 0U, /* Shifter 0 = 7 are used. */
    .shifterCount = 8U,
    .timerIdx = 0U
};

/* eDMA configuration structures. */
static edma_transfer_config_t edma16_transfer_config = eDMA16_CONFIG;
static edma_transfer_config_t edma17_transfer_config = eDMA17_CONFIG;

int main(void)
{
    ov7670_status_t ov7670_status;

    OSA_Init();

    /* Initialize the board. */
    hardware_init();
    configure_clkout_pin(); /* Flexbus and camera's xclk pins. */
    configure_app_flexbus_pins();/* SSD1289's pins. */
    configure_app_i2c_pins(); /* OV7670's SCCB pins. */
    configure_app_flexio_pins(); /* OV7670's CPI pins.  */

    /* Initialize the LCD (SSD1289). */
    Init_SSD1289();  
    SetOrientation_SSD1289(LCD_LANDSCAPE);
    ClearScreen_SSD1289(0x0000);
    SetWindow_SSD1289(LCD_HORIZONTAL_START_POINT, LCD_VERTICAL_START_POINT,
                      LCD_HORIZONTAL_END_POINT, LCD_VERTICAL_END_POINT);

    /* Initialize the camera (OV7670 SCCB). */
    ov7670_status = OV7670_Init(&ov7670_handler, (ov7670_config_t*)&ov7670_config);
    while(ov7670_status != kStatus_OV7670_Success);
  
    /* Initialize the Flexio (OV7670 CPI). */
    FlexIO_Init();

    /* Initialize the eDMA for moving data. */
    DMAMUX_HAL_Init(DMAMUX);
    EDMA_HAL_Init(DMA0);
    /* Since the function call is not allowed in constant expression,
        * so the the API that get the buffer's address is called here in application. */
    edma17_transfer_config.srcAddr = FLEXIO_Camera_HAL_GetRxBufferAddr(&gFlexioCameraDevStruct), 
    eDma_Init(kEDMAChannel16, /* DMA Channel 16 is bound to the threshold event which is one item left to the full buffer.  */
        gFlexioCameraDevStruct.shifterStartIdx + 2U, &edma16_transfer_config);
    eDma_Init(kEDMAChannel17, /* DMA channel 17 is bound to the bull buffer event. */
        gFlexioCameraDevStruct.shifterStartIdx + 1U, &edma17_transfer_config);

    PORTA_InstallCallback(1, porta_callback); /* vsync pin in PTA13 with priority value 1. */
    configure_app_gpio_pins(); /* Enable the interrupt of OV7670's vsync pins. */

    while(1) {}
}

/*******************************************************************************
 * Flexio initialization function.
 * Flexio is configured fro camera interface. All 8 shifter registers 
 * are chained from 7 to 0. Shifter 7 in configured to capture 8-bit
 * parralel data from D24 - D31 pins. The rest shifters are 
 * configured to capture data from their previous shifter (chained).
 * Each shifter uses the same timer for data shifts. Each positive edge
 * shifter timer clock shifts data by 8-bits from left to right (from 
 * shifter 7 to shifter 0). When shifter 0 status flag occur (32 x 8-bits
 * transfers) dma request is generated. Timer 0 is configured as shifters
 * timers. D18 pin (HREF -> PTA12) is used as timer 0 trigger and is 
 * configured to enable timer on rising edge and disable timer on 
 * falling edge. Timer is counting to 64 (32 shifter clock) then 
 * restarted. The Pin D1 (PCLK -> PTB1) is used as the clock source 
 * for timer. Flexio has enabled fast register access. Hence the flexIO
 * clock is twice the bus clock (150MHZ -> 75MHz).
 ******************************************************************************/
static void FlexIO_Init(void)
{
  /* Configure the flexio as camera. */
  FLEXIO_Camera_HAL_Configure(&gFlexioCameraDevStruct);

  /* Clear all the flag. */
  FLEXIO_Camera_HAL_ClearRxBufferFullFlag(&gFlexioCameraDevStruct);
  FLEXIO_HAL_ClearShifterErrorFlags(FLEXIO0, 0xFF);
  FLEXIO_HAL_ClearTimerStatusFlags(FLEXIO0, 0xFF);

  FLEXIO_Camera_HAL_SetRxBufferDmaCmd(&gFlexioCameraDevStruct, 0x03, true);

  /* Configure the flexio common function. */
  FLEXIO_HAL_SetFastAccessCmd(FLEXIO0, true);
  FLEXIO_HAL_SetFlexioEnableCmd(FLEXIO0, true);
}

/*******************************************************************************
 * eDMA module initialization
 * Function provided initilaization of DMA module.
 ******************************************************************************/
static void eDma_Init(
    edma_channel_indicator_t channel,    /* EDMA channel. */
    uint8_t source,                      /* Trigger source. */
    edma_transfer_config_t *edma_config  /* Transfer parameter. */
)
{  
  /* DMA MUX initialization. */
  DMAMUX_HAL_SetChannelCmd(DMAMUX, channel, false);
  DMAMUX_HAL_SetTriggerSource(DMAMUX, channel, source);
    
  /* eDMA initialization. */
  EDMA_HAL_SetChannelArbitrationMode(DMA0, kEDMAChnArbitrationRoundrobin);
  EDMA_HAL_SetGroupPriority(DMA0, kEDMAGroup0PriorityLowGroup1PriorityHigh);
  EDMA_HAL_SetGroupArbitrationMode(DMA0, kEDMAGroupArbitrationRoundrobin);
  
  EDMA_HAL_HTCDSetSrcAddr(DMA0, channel, edma_config->srcAddr);
  EDMA_HAL_HTCDSetSrcOffset(DMA0, channel, edma_config->srcOffset);
  EDMA_HAL_HTCDSetSrcLastAdjust(DMA0, channel, edma_config->srcLastAddrAdjust);
  
  EDMA_HAL_HTCDSetDestAddr(DMA0, channel, edma_config->destAddr);
  EDMA_HAL_HTCDSetDestOffset(DMA0, channel, edma_config->destOffset);
  EDMA_HAL_HTCDSetDestLastAdjust(DMA0, channel, edma_config->destLastAddrAdjust);
  
  EDMA_HAL_HTCDSetNbytes(DMA0, channel, edma_config->minorLoopCount);
  EDMA_HAL_HTCDSetMajorCount(DMA0, channel, edma_config->majorLoopCount );
  EDMA_HAL_HTCDSetAttribute(DMA0, channel, edma_config->srcModulo, 
                                                 edma_config->destModulo,
                                                 edma_config->srcTransferSize,
                                                 edma_config->destTransferSize);
  /* Clear all the flags. */
  EDMA_HAL_ClearDoneStatusFlag(DMA0, channel);
  
  EDMA_HAL_HTCDSetDisableDmaRequestAfterTCDDoneCmd(DMA0, channel, false);
  
  EDMA_HAL_SetAsyncRequestInStopModeCmd(DMA0, channel, true); /* The DMA can work in CPU's stop mode. */
  
  EDMA_HAL_SetDmaRequestCmd(DMA0, channel, true);
  DMAMUX_HAL_SetChannelCmd(DMAMUX, channel, true);
}

/*******************************************************************************
 * eDMA module reset (reset of src, dest addresss and CITTER reinit)
 * After called, the DMA would reset the tranfer pointer. Once the request comes,
 * the transfer would start at the initial position.
 ******************************************************************************/
static void eDma_Reset(edma_channel_indicator_t channel, edma_transfer_config_t *edma_config)
{
  EDMA_HAL_HTCDSetSrcAddr(DMA0, channel, edma_config->srcAddr);
  EDMA_HAL_HTCDSetDestAddr(DMA0, channel, edma_config->destAddr);
  EDMA_HAL_HTCDSetMajorCount(DMA0, channel, edma_config->majorLoopCount);
}

/*******************************************************************************
 * porta_callback function
 * Function provided handling of porta IRQ.
 * pin - pin mask 
 ******************************************************************************/
static void porta_callback(uint32_t pin)
{
  if (pin == (1<<13)) /* vsyhc pin. */
  {
    /* Reset the transfer of flexio_camera to buffer, and buffer to LCD. */
    eDma_Reset(kEDMAChannel16, &edma16_transfer_config); /* buffer to LCD. */
    eDma_Reset(kEDMAChannel17, &edma17_transfer_config); /* flexio_camera to buffer. */
    /* Reset the LCD. */
    SetWindow_SSD1289(LCD_HORIZONTAL_START_POINT, LCD_VERTICAL_START_POINT,
                      LCD_HORIZONTAL_END_POINT, LCD_VERTICAL_END_POINT);
  }
}

/* EOF app_flexio_ov7670. */

