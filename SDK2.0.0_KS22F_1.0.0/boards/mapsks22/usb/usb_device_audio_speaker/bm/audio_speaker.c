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

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_audio.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "audio_speaker.h"
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "fsl_sai.h"

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#include "usb_phy.h"
#endif

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OVER_SAMPLE_RATE (384U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
usb_status_t USB_DeviceAudioCallback(class_handle_t handle, uint32_t event, void *param);
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/
sai_handle_t txHandle = {0};
#if defined (DEMO_CODEC_WM8960)
wm8960_handle_t codecHandle = {0};
#else
sgtl_handle_t codecHandle = {0};
#endif

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
lpi2c_master_handle_t i2cHandle = {0};
#else
i2c_master_handle_t i2cHandle = {0};
#endif

uint8_t audio_data_buff[AUDIO_DATA_BUFF_SIZE];
sai_transfer_t xfer;
extern usb_device_class_struct_t g_UsbDeviceAudioClass;

/* Default value of audio generator device struct */
usb_audio_speaker_struct_t s_audioSpeaker = {
    .deviceHandle = NULL,
    .audioHandle = (class_handle_t)NULL,
    .speed = USB_SPEED_FULL,
    .attach = 0U,
    .copyProtect = 0x01U,
    .curMute = 0x01U,
    .curVolume = {0x00U, 0x80U},
    .minVolume = {0x00U, 0x80U},
    .maxVolume = {0xFFU, 0x7FU},
    .resVolume = {0x01U, 0x00U},
    .curBass = 0x00U,
    .minBass = 0x80U,
    .maxBass = 0x7FU,
    .resBass = 0x01U,
    .curMid = 0x00U,
    .minMid = 0x80U,
    .maxMid = 0x7FU,
    .resMid = 0x01U,
    .curTreble = 0x01U,
    .minTreble = 0x80U,
    .maxTreble = 0x7FU,
    .resTreble = 0x01U,
    .curAutomaticGain = 0x01U,
    .curDelay = {0x00U, 0x40U},
    .minDelay = {0x00U, 0x00U},
    .maxDelay = {0xFFU, 0xFFU},
    .resDelay = {0x00U, 0x01U},
    .curLoudness = 0x01U,
    .curSamplingFrequency = {0x00U, 0x00U, 0x01U},
    .minSamplingFrequency = {0x00U, 0x00U, 0x01U},
    .maxSamplingFrequency = {0x00U, 0x00U, 0x01U},
    .resSamplingFrequency = {0x00U, 0x00U, 0x01U},
#if USBCFG_AUDIO_CLASS_2_0
    .curSampleFrequency = 16000U,
    .curClockValid = 1U,
    .controlRange = {1U, 16000U, 16000U, 0U},
#endif
    .speed = USB_SPEED_FULL,
    .attach = 0U,
};

/* USB device class information */
static usb_device_class_config_struct_t s_audioConfig[1] = {{
    USB_DeviceAudioCallback, (class_handle_t)NULL, &g_UsbDeviceAudioClass,
}};

/* USB device class configuraion information */
static usb_device_class_config_list_struct_t s_audioConfigList = {
    s_audioConfig, USB_DeviceCallback, 1U,
};

/*******************************************************************************
* Code
******************************************************************************/


/* Initialize the structure information for sai. */
void init(void)
{
    sai_config_t config;
    uint32_t mclkSourceClockHz = 0U;
    sai_transfer_format_t format;

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_config_t i2cConfig = {0};
#else
    i2c_master_config_t i2cConfig = {0};
#endif
    uint32_t i2cSourceClock;

    /* Init SAI module */
    SAI_TxGetDefaultConfig(&config);
    SAI_TxInit(DEMO_SAI, &config);

    /* Configure the audio format */
    format.bitWidth = kSAI_WordWidth16bits;
    format.channel = 0U;
    format.sampleRate_Hz = kSAI_SampleRate16KHz;
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    format.masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
/* format.masterClockHz = 24576000; */
#else
    format.masterClockHz = CLOCK_GetFreq(DEMO_SAI_CLKSRC);
#endif
    format.protocol = config.protocol;
    format.stereo = kSAI_Stereo;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    format.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif

    /* Configure Sgtl5000 I2C */
    codecHandle.base = DEMO_I2C;
    codecHandle.i2cHandle = &i2cHandle;
    i2cSourceClock = CLOCK_GetFreq(DEMO_I2C_CLKSRC);

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    LPI2C_MasterCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#else
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    I2C_MasterCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#endif

#if defined (DEMO_CODEC_WM8960)
    WM8960_Init(&codecHandle, NULL);
    WM8960_ConfigDataFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
#else
    /* Use default settings for sgtl5000 */
    SGTL_Init(&codecHandle, NULL);
    /* Configure codec format */
    SGTL_ConfigDataFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
#endif
    SAI_TxCreateHandle(DEMO_SAI, &txHandle, NULL, NULL);
    mclkSourceClockHz = CLOCK_GetFreq(DEMO_SAI_CLKSRC);
    SAI_TxSetTransferFormat(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);
}

/*!
 * @brief Audio class specific request function.
 *
 * This function handles the Audio class specific requests.
 *
 * @param handle		  The Audio class handle.
 * @param event 		  The Audio class event type.
 * @param param 		  The parameter of the class specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceAudioRequest(class_handle_t handle, uint32_t event, void *param)
{
    usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;
    usb_status_t error = kStatus_USB_Success;

    switch (event)
    {
        case USB_DEVICE_AUDIO_GET_CUR_MUTE_CONTROL:
            request->buffer = &s_audioSpeaker.curMute;
            request->length = sizeof(s_audioSpeaker.curMute);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_CONTROL:
            request->buffer = s_audioSpeaker.curVolume;
            request->length = sizeof(s_audioSpeaker.curVolume);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_BASS_CONTROL:
            request->buffer = &s_audioSpeaker.curBass;
            request->length = sizeof(s_audioSpeaker.curBass);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_MID_CONTROL:
            request->buffer = &s_audioSpeaker.curMid;
            request->length = sizeof(s_audioSpeaker.curMid);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_TREBLE_CONTROL:
            request->buffer = &s_audioSpeaker.curTreble;
            request->length = sizeof(s_audioSpeaker.curTreble);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_AUTOMATIC_GAIN_CONTROL:
            request->buffer = &s_audioSpeaker.curAutomaticGain;
            request->length = sizeof(s_audioSpeaker.curAutomaticGain);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_DELAY_CONTROL:
            request->buffer = s_audioSpeaker.curDelay;
            request->length = sizeof(s_audioSpeaker.curDelay);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_SAMPLING_FREQ_CONTROL:
            request->buffer = s_audioSpeaker.curSamplingFrequency;
            request->length = sizeof(s_audioSpeaker.curSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_VOLUME_CONTROL:
            request->buffer = s_audioSpeaker.minVolume;
            request->length = sizeof(s_audioSpeaker.minVolume);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_BASS_CONTROL:
            request->buffer = &s_audioSpeaker.minBass;
            request->length = sizeof(s_audioSpeaker.minBass);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_MID_CONTROL:
            request->buffer = &s_audioSpeaker.minMid;
            request->length = sizeof(s_audioSpeaker.minMid);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_TREBLE_CONTROL:
            request->buffer = &s_audioSpeaker.minTreble;
            request->length = sizeof(s_audioSpeaker.minTreble);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_DELAY_CONTROL:
            request->buffer = s_audioSpeaker.minDelay;
            request->length = sizeof(s_audioSpeaker.minDelay);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_SAMPLING_FREQ_CONTROL:
            request->buffer = s_audioSpeaker.minSamplingFrequency;
            request->length = sizeof(s_audioSpeaker.minSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_VOLUME_CONTROL:
            request->buffer = s_audioSpeaker.maxVolume;
            request->length = sizeof(s_audioSpeaker.maxVolume);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_BASS_CONTROL:
            request->buffer = &s_audioSpeaker.maxBass;
            request->length = sizeof(s_audioSpeaker.maxBass);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_MID_CONTROL:
            request->buffer = &s_audioSpeaker.maxMid;
            request->length = sizeof(s_audioSpeaker.maxMid);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_TREBLE_CONTROL:
            request->buffer = &s_audioSpeaker.maxTreble;
            request->length = sizeof(s_audioSpeaker.maxTreble);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_DELAY_CONTROL:
            request->buffer = s_audioSpeaker.maxDelay;
            request->length = sizeof(s_audioSpeaker.maxDelay);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_SAMPLING_FREQ_CONTROL:
            request->buffer = s_audioSpeaker.maxSamplingFrequency;
            request->length = sizeof(s_audioSpeaker.maxSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_RES_VOLUME_CONTROL:
            request->buffer = s_audioSpeaker.resVolume;
            request->length = sizeof(s_audioSpeaker.resVolume);
            break;
        case USB_DEVICE_AUDIO_GET_RES_BASS_CONTROL:
            request->buffer = &s_audioSpeaker.resBass;
            request->length = sizeof(s_audioSpeaker.resBass);
            break;
        case USB_DEVICE_AUDIO_GET_RES_MID_CONTROL:
            request->buffer = &s_audioSpeaker.resMid;
            request->length = sizeof(s_audioSpeaker.resMid);
            break;
        case USB_DEVICE_AUDIO_GET_RES_TREBLE_CONTROL:
            request->buffer = &s_audioSpeaker.resTreble;
            request->length = sizeof(s_audioSpeaker.resTreble);
            break;
        case USB_DEVICE_AUDIO_GET_RES_DELAY_CONTROL:
            request->buffer = s_audioSpeaker.resDelay;
            request->length = sizeof(s_audioSpeaker.resDelay);
            break;
        case USB_DEVICE_AUDIO_GET_RES_SAMPLING_FREQ_CONTROL:
            request->buffer = s_audioSpeaker.resSamplingFrequency;
            request->length = sizeof(s_audioSpeaker.resSamplingFrequency);
            break;
#if USBCFG_AUDIO_CLASS_2_0
        case USB_DEVICE_AUDIO_GET_CUR_SAM_FREQ_CONTROL:
            request->buffer = (uint8_t *)&s_audioSpeaker.curSampleFrequency;
            request->length = sizeof(s_audioSpeaker.curSampleFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_RANGE_SAM_FREQ_CONTROL:
            request->buffer = (uint8_t *)&s_audioSpeaker.controlRange;
            request->length = sizeof(s_audioSpeaker.controlRange);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_CLOCK_VALID_CONTROL:
            request->buffer = &s_audioSpeaker.curClockValid;
            request->length = sizeof(s_audioSpeaker.curClockValid);
            break;
#endif

        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.curVolume;
            }
            else
            {
                uint16_t volume = (uint16_t)((uint16_t)s_audioSpeaker.curVolume[1] << 8U);
                volume |= (uint8_t)(s_audioSpeaker.curVolume[0]);
                usb_echo("Set Cur Volume : %x\r\n", volume);
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_MUTE_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.curMute;
            }
            else
            {
                usb_echo("Set Cur Mute : %x\r\n", s_audioSpeaker.curMute);
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_BASS_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.curBass;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_MID_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.curMid;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_TREBLE_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.curTreble;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_AUTOMATIC_GAIN_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.curAutomaticGain;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_DELAY_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.curDelay;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_SAMPLING_FREQ_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.curSamplingFrequency;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_VOLUME_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.minVolume;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_BASS_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.minBass;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_MID_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.minMid;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_TREBLE_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.minTreble;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_DELAY_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.minDelay;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_SAMPLING_FREQ_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.minSamplingFrequency;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_VOLUME_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.maxVolume;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_BASS_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.maxBass;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_MID_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.maxMid;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_TREBLE_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.maxTreble;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_DELAY_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.maxDelay;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_SAMPLING_FREQ_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.maxSamplingFrequency;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_VOLUME_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.resVolume;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_BASS_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.resBass;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_MID_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.resMid;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_TREBLE_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &s_audioSpeaker.resTreble;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_DELAY_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.resDelay;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_SAMPLING_FREQ_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = s_audioSpeaker.resSamplingFrequency;
            }
            break;
#if USBCFG_AUDIO_CLASS_2_0
        case USB_DEVICE_AUDIO_SET_CUR_SAM_FREQ_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = (uint8_t *)&s_audioSpeaker.curSampleFrequency;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_CLOCK_VALID_CONTROL:
        {
            request->buffer = &s_audioSpeaker.curClockValid;
        }
        break;

#endif
        default:
            error = kStatus_USB_InvalidRequest;
            break;
    }
    return error;
}

/*!
 * @brief Audio class specific callback function.
 *
 * This function handles the Audio class specific requests.
 *
 * @param handle		  The Audio class handle.
 * @param event 		  The Audio class event type.
 * @param param 		  The parameter of the class specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceAudioCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint16_t ep_packet_size;
    usb_device_endpoint_callback_message_struct_t *ep_cb_param;
    ep_cb_param = (usb_device_endpoint_callback_message_struct_t *)param;
    uint32_t temp = 0;

    switch (event)
    {
        case kUSB_DeviceAudioEventStreamRecvResponse:
            if ((s_audioSpeaker.attach) &&
                (ep_cb_param->length == ((USB_SPEED_HIGH == s_audioSpeaker.speed) ? HS_ISO_OUT_ENDP_PACKET_SIZE :
                                                                                    FS_ISO_OUT_ENDP_PACKET_SIZE)))
            {
                ep_packet_size = ((s_audioSpeaker.speed == USB_SPEED_HIGH) ? HS_ISO_OUT_ENDP_PACKET_SIZE :
                                                                             FS_ISO_OUT_ENDP_PACKET_SIZE);
                temp = (uint32_t)audio_data_buff;
                xfer.data = (uint8_t *)temp;
                xfer.dataSize = ep_packet_size;
                SAI_SendNonBlocking(DEMO_SAI, &txHandle, &xfer);
                /* Wait until finished */
                while (SAI_GetSendRemainingBytes(DEMO_SAI, &txHandle) != 0U)
                {
                }

                /* request next data to the current buffer */
                error = USB_DeviceAudioRecv(handle, USB_AUDIO_STREAM_ENDPOINT, audio_data_buff, ep_packet_size);
            }
            break;

        default:
            if (param && (event > 0xFF))
            {
                error = USB_DeviceAudioRequest(handle, event, param);
            }
            break;
    }

    return error;
}

/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle		  The USB device handle.
 * @param event 		  The USB device event type.
 * @param param 		  The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint8_t *temp8 = (uint8_t *)param;
    uint16_t *temp16 = (uint16_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            s_audioSpeaker.attach = 0U;
            error = kStatus_USB_Success;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
            if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &s_audioSpeaker.speed))
            {
                USB_DeviceSetSpeed(handle, s_audioSpeaker.speed);
            }

#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (USB_AUDIO_SPEAKER_CONFIGURE_INDEX == (*temp8))
            {
                s_audioSpeaker.attach = 1U;
                s_audioSpeaker.currentConfiguration = *temp8;
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if (s_audioSpeaker.attach)
            {
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);

                if (s_audioSpeaker.currentInterfaceAlternateSetting[interface] != alternateSetting)
                {
                    s_audioSpeaker.currentInterfaceAlternateSetting[interface] = alternateSetting;
                    if (alternateSetting == 1U)
                    {
                        USB_DeviceAudioRecv(s_audioSpeaker.audioHandle, USB_AUDIO_STREAM_ENDPOINT, audio_data_buff,
                                            ((USB_SPEED_HIGH == s_audioSpeaker.speed) ? HS_ISO_OUT_ENDP_PACKET_SIZE :
                                                                                        FS_ISO_OUT_ENDP_PACKET_SIZE));
                    }
                }
            }
            break;
        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                *temp8 = s_audioSpeaker.currentConfiguration;
                error = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                /* Get current alternate setting of the interface request */
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                if (interface < USB_AUDIO_SPEAKER_INTERFACE_COUNT)
                {
                    *temp16 = (*temp16 & 0xFF00U) | s_audioSpeaker.currentInterfaceAlternateSetting[interface];
                    error = kStatus_USB_Success;
                }
                else
                {
                    error = kStatus_USB_InvalidRequest;
                }
            }
            break;
        case kUSB_DeviceEventGetDeviceDescriptor:
            if (param)
            {
                /* Get device descriptor request */
                error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (param)
            {
                /* Get device configuration descriptor request */
                error = USB_DeviceGetConfigurationDescriptor(handle,
                                                             (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetStringDescriptor:
            if (param)
            {
                /* Get device string descriptor request */
                error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;
        default:
            break;
    }

    return error;
}

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(s_audioSpeaker.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(s_audioSpeaker.deviceHandle);
}
#endif

/*!
 * @brief Application initialization function.
 *
 * This function initializes the application.
 *
 * @return None.
 */
void APPInit(void)
{
    uint8_t irqNo;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t ehciIrq[] = USBHS_IRQS;
    irqNo = ehciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    CLOCK_EnableUsbhs0Clock(kCLOCK_UsbSrcPll0, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t khciIrq[] = USB_IRQS;
    irqNo = khciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    SystemCoreClockUpdate();

#if ((defined FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED) && (FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED))
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcIrc48M, 48000000U);
#else
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcPll0, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
#endif /* FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED */
#endif
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
    MPU->CESR = 0;
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

/*
 * If the SOC has USB KHCI dedicated RAM, the RAM memory needs to be clear after
 * the KHCI clock is enabled. When the demo uses USB EHCI IP, the USB KHCI dedicated
 * RAM can not be used and the memory can't be accessed.
 */
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U))
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS) && (FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS > 0U))
    for (int i = 0; i < FSL_FEATURE_USB_KHCI_USB_RAM; i++)
    {
        ((uint8_t*)FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS */
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM */

    init();

    if (kStatus_USB_Success != USB_DeviceClassInit(CONTROLLER_ID, &s_audioConfigList, &s_audioSpeaker.deviceHandle))
    {
        usb_echo("USB device failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device audio generator demo\r\n");
        s_audioSpeaker.audioHandle = s_audioConfigList.config->classHandle;
    }

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNo, USB_DEVICE_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ((IRQn_Type)irqNo);

    USB_DeviceRun(s_audioSpeaker.deviceHandle);
}

/*!
 * @brief Application task function.
 *
 * This function runs the task for application.
 *
 * @return None.
 */
#if defined(__CC_ARM) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    CLOCK_SetPllFllSelClock(3U); /* IRC48 MHz clock. */
    CLOCK_SetLpi2c0Clock(1);

    APPInit();

    while (1)
    {
#if USB_DEVICE_CONFIG_USE_TASK
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
        USB_DeviceEhciTaskFunction(s_audioSpeaker.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
        USB_DeviceKhciTaskFunction(s_audioSpeaker.deviceHandle);
#endif
#endif
    }

}
