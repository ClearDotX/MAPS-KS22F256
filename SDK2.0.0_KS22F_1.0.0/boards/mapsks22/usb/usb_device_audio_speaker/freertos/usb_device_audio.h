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

#ifndef __USB_DEVICE_AUDIO_H__
#define __USB_DEVICE_AUDIO_H__

/*!
 * @addtogroup usb_device_audio_drv
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Enable/Disable AUDIO CLASS 2.0 */
#if USB_DEVICE_CONFIG_AUDIO
#define USBCFG_AUDIO_CLASS_2_0 0
#endif

/*!
 * @name USB Audio class codes
 * @{
 */

/*! @brief Audio device class code */
#define USB_DEVICE_CONFIG_AUDIO_CLASS_CODE (0x01)

/*! @brief Audio device subclass code */
#define USB_DEVICE_AUDIO_STREAM_SUBCLASS (0x02)
#define USB_DEVICE_AUDIO_CONTROL_SUBCLASS (0x01)

/*! @brief Audio device class-specific descriptor type */
#define USB_DESCRIPTOR_TYPE_AUDIO_CS_INTERFACE (0x24)

/*! @brief Audio device class-specific control interface descriptor subtype */
#define USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_HEADER (0x01)
#define USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_INPUT_TERMINAL (0x02)
#define USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_OUTPUT_TERMINAL (0x03)
#define USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_FEATURE_UNIT (0x06)

/*! @brief Audio device class-specific steam interface descriptor subtype */
#define USB_DESCRIPTOR_SUBTYPE_AUDIO_STREAMING_GENERAL (0x01)
#define USB_DESCRIPTOR_SUBTYPE_AUDIO_STREAMING_FORMAT_TYPE (0x02)

/*! @brief Audio device class-specific GET CUR COMMAND  */
#define USB_DEVICE_AUDIO_GET_CUR_MUTE_CONTROL (0x8101)
#define USB_DEVICE_AUDIO_GET_CUR_VOLUME_CONTROL (0x8102)
#define USB_DEVICE_AUDIO_GET_CUR_BASS_CONTROL (0x8103)
#define USB_DEVICE_AUDIO_GET_CUR_MID_CONTROL (0x8104)
#define USB_DEVICE_AUDIO_GET_CUR_TREBLE_CONTROL (0x8105)
#define USB_DEVICE_AUDIO_GET_CUR_GRAPHIC_EQUALIZER_CONTROL (0x8106)
#define USB_DEVICE_AUDIO_GET_CUR_AUTOMATIC_GAIN_CONTROL (0x8107)
#define USB_DEVICE_AUDIO_GET_CUR_DELAY_CONTROL (0x8108)
#define USB_DEVICE_AUDIO_GET_CUR_BASS_BOOST_CONTROL (0x8109)
#define USB_DEVICE_AUDIO_GET_CUR_LOUDNESS_CONTROL (0x810A)

/*! @brief Audio device class-specific GET MIN COMMAND  */
#define USB_DEVICE_AUDIO_GET_MIN_VOLUME_CONTROL (0x8202)
#define USB_DEVICE_AUDIO_GET_MIN_BASS_CONTROL (0x8203)
#define USB_DEVICE_AUDIO_GET_MIN_MID_CONTROL (0x8204)
#define USB_DEVICE_AUDIO_GET_MIN_TREBLE_CONTROL (0x8205)
#define USB_DEVICE_AUDIO_GET_MIN_GRAPHIC_EQUALIZER_CONTROL (0x8206)
#define USB_DEVICE_AUDIO_GET_MIN_DELAY_CONTROL (0x8208)

/*! @brief Audio device class-specific GET MAX COMMAND  */
#define USB_DEVICE_AUDIO_GET_MAX_VOLUME_CONTROL (0x8302)
#define USB_DEVICE_AUDIO_GET_MAX_BASS_CONTROL (0x8303)
#define USB_DEVICE_AUDIO_GET_MAX_MID_CONTROL (0x8304)
#define USB_DEVICE_AUDIO_GET_MAX_TREBLE_CONTROL (0x8305)
#define USB_DEVICE_AUDIO_GET_MAX_GRAPHIC_EQUALIZER_CONTROL (0x8306)
#define USB_DEVICE_AUDIO_GET_MAX_DELAY_CONTROL (0x8308)

/*! @brief Audio device class-specific GET RES COMMAND  */
#define USB_DEVICE_AUDIO_GET_RES_VOLUME_CONTROL (0x8402)
#define USB_DEVICE_AUDIO_GET_RES_BASS_CONTROL (0x8403)
#define USB_DEVICE_AUDIO_GET_RES_MID_CONTROL (0x8404)
#define USB_DEVICE_AUDIO_GET_RES_TREBLE_CONTROL (0x8405)
#define USB_DEVICE_AUDIO_GET_RES_GRAPHIC_EQUALIZER_CONTROL (0x8406)
#define USB_DEVICE_AUDIO_GET_RES_DELAY_CONTROL (0x8408)

/*! @brief Audio device class-specific SET CUR COMMAND  */
#define USB_DEVICE_AUDIO_SET_CUR_MUTE_CONTROL (0x0101)
#define USB_DEVICE_AUDIO_SET_CUR_VOLUME_CONTROL (0x0102)
#define USB_DEVICE_AUDIO_SET_CUR_BASS_CONTROL (0x0103)
#define USB_DEVICE_AUDIO_SET_CUR_MID_CONTROL (0x0104)
#define USB_DEVICE_AUDIO_SET_CUR_TREBLE_CONTROL (0x0105)
#define USB_DEVICE_AUDIO_SET_CUR_GRAPHIC_EQUALIZER_CONTROL (0x0106)
#define USB_DEVICE_AUDIO_SET_CUR_AUTOMATIC_GAIN_CONTROL (0x0107)
#define USB_DEVICE_AUDIO_SET_CUR_DELAY_CONTROL (0x0108)
#define USB_DEVICE_AUDIO_SET_CUR_BASS_BOOST_CONTROL (0x0109)
#define USB_DEVICE_AUDIO_SET_CUR_LOUDNESS_CONTROL (0x010A)
#define USB_DEVICE_AUDIO_SET_CUR_PITCH_CONTROL (0x010D)

/*! @brief Audio device class-specific SET MIN COMMAND  */
#define USB_DEVICE_AUDIO_SET_MIN_VOLUME_CONTROL (0x0202)
#define USB_DEVICE_AUDIO_SET_MIN_BASS_CONTROL (0x0203)
#define USB_DEVICE_AUDIO_SET_MIN_MID_CONTROL (0x0204)
#define USB_DEVICE_AUDIO_SET_MIN_TREBLE_CONTROL (0x0205)
#define USB_DEVICE_AUDIO_SET_MIN_GRAPHIC_EQUALIZER_CONTROL (0x0206)
#define USB_DEVICE_AUDIO_SET_MIN_DELAY_CONTROL (0x0208)

/*! @brief Audio device class-specific SET MAX COMMAND  */
#define USB_DEVICE_AUDIO_SET_MAX_VOLUME_CONTROL (0x0302)
#define USB_DEVICE_AUDIO_SET_MAX_BASS_CONTROL (0x0303)
#define USB_DEVICE_AUDIO_SET_MAX_MID_CONTROL (0x0304)
#define USB_DEVICE_AUDIO_SET_MAX_TREBLE_CONTROL (0x0305)
#define USB_DEVICE_AUDIO_SET_MAX_GRAPHIC_EQUALIZER_CONTROL (0x0306)
#define USB_DEVICE_AUDIO_SET_MAX_DELAY_CONTROL (0x0308)

/*! @brief Audio device class-specific SET RES COMMAND  */
#define USB_DEVICE_AUDIO_SET_RES_VOLUME_CONTROL (0x0402)
#define USB_DEVICE_AUDIO_SET_RES_BASS_CONTROL (0x0403)
#define USB_DEVICE_AUDIO_SET_RES_MID_CONTROL (0x0404)
#define USB_DEVICE_AUDIO_SET_RES_TREBLE_CONTROL (0x0405)
#define USB_DEVICE_AUDIO_SET_RES_GRAPHIC_EQUALIZER_CONTROL (0x0406)
#define USB_DEVICE_AUDIO_SET_RES_DELAY_CONTROL (0x0408)

/*! @brief Audio device class-specific GET SAMPLING FREQ CONTROL COMMAND  */
#define USB_DEVICE_AUDIO_GET_CUR_SAMPLING_FREQ_CONTROL (0x810C)
#define USB_DEVICE_AUDIO_GET_MIN_SAMPLING_FREQ_CONTROL (0x820C)
#define USB_DEVICE_AUDIO_GET_MAX_SAMPLING_FREQ_CONTROL (0x830C)
#define USB_DEVICE_AUDIO_GET_RES_SAMPLING_FREQ_CONTROL (0x840C)

/*! @brief Audio device class-specific SET SAMPLING FREQ CONTROL COMMAND  */
#define USB_DEVICE_AUDIO_SET_CUR_SAMPLING_FREQ_CONTROL (0x010C)
#define USB_DEVICE_AUDIO_SET_MIN_SAMPLING_FREQ_CONTROL (0x020C)
#define USB_DEVICE_AUDIO_SET_MAX_SAMPLING_FREQ_CONTROL (0x030C)
#define USB_DEVICE_AUDIO_SET_RES_SAMPLING_FREQ_CONTROL (0x040C)

#if USBCFG_AUDIO_CLASS_2_0
/*! @brief Audio 2.0 device class-specific SET/GET SAMPLING FREQ CONTROL COMMAND  */
#define USB_DEVICE_AUDIO_SET_CUR_SAM_FREQ_CONTROL (0x0501)
#define USB_DEVICE_AUDIO_SET_CUR_CLOCK_VALID_CONTROL (0x0502)
#define USB_DEVICE_AUDIO_GET_CUR_SAM_FREQ_CONTROL (0x8501)
#define USB_DEVICE_AUDIO_GET_RANGE_SAM_FREQ_CONTROL (0x9501)
#define USB_DEVICE_AUDIO_GET_CUR_CLOCK_VALID_CONTROL (0x8502)
#endif

#if USBCFG_AUDIO_CLASS_2_0
#define USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_CLOCK_SOURCE_UNIT (0x0A)
#define USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_CLOCK_SELECTOR_UNIT (0x0B)
#define USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_CLOCK_MULTIPLIER_UNIT (0x0C)
#define USB_DESCRIPTOR_SUBTYPE_AUDIO_CONTROL_SAMPLE_RATE_CONVERTER_UNIT (0x0D)

#define USB_DEVICE_AUDIO_CS_CONTROL_UNDEFINED (0x00)
#define USB_DEVICE_AUDIO_CS_SAM_FREQ_CONTROL (0x01)
#define USB_DEVICE_AUDIO_CS_CLOCK_VALID_CONTROL (0x02)
#define USB_DEVICE_AUDIO_REQUEST_CUR (0x01)
#define USB_DEVICE_AUDIO_REQUEST_RANGE (0x02)

#endif

/* Commands for USB device AUDIO Class specific request */
#define USB_DEVICE_AUDIO_SET_CUR_VOLUME_REQUEST (0x01)
#define USB_DEVICE_AUDIO_SET_MIN_VOLUME_REQUEST (0x02)
#define USB_DEVICE_AUDIO_SET_MAX_VOLUME_REQUEST (0x03)
#define USB_DEVICE_AUDIO_SET_RES_VOLUME_REQUEST (0x04)

#define USB_DEVICE_AUDIO_GET_CUR_VOLUME_REQUEST (0x81)
#define USB_DEVICE_AUDIO_GET_MIN_VOLUME_REQUEST (0x82)
#define USB_DEVICE_AUDIO_GET_MAX_VOLUME_REQUEST (0x83)
#define USB_DEVICE_AUDIO_GET_RES_VOLUME_REQUEST (0x84)

/* Commands for USB device AUDIO terminal control selector */
#define USB_DEVICE_AUDIO_COPY_PROTECT_CONTROL_SELECTOR (0x01)

/* Commands for USB device AUDIO feature unit control selector */
#define USB_DEVICE_AUDIO_MUTE_CONTROL_SELECTOR (0x01)
#define USB_DEVICE_AUDIO_VOLUME_CONTROL_SELECTOR (0x02)
#define USB_DEVICE_AUDIO_BASS_CONTROL_SELECTOR (0x03)
#define USB_DEVICE_AUDIO_MID_CONTROL_SELECTOR (0x04)
#define USB_DEVICE_AUDIO_TREBLE_CONTROL_SELECTOR (0x05)
#define USB_DEVICE_AUDIO_GRAPHIC_EQUALIZER_CONTROL_SELECTOR (0x06)
#define USB_DEVICE_AUDIO_AUTOMATIC_GAIN_CONTROL_SELECTOR (0x07)
#define USB_DEVICE_AUDIO_DELAY_CONTROL_SELECTOR (0x08)
#define USB_DEVICE_AUDIO_BASS_BOOST_CONTROL_SELECTOR (0x09)
#define USB_DEVICE_AUDIO_LOUDNESS_CONTROL_SELECTOR (0x0A)

/* Commands for USB device AUDIO endpoint control selector */
#define USB_DEVICE_AUDIO_SAMPLING_FREQ_CONTROL_SELECTOR (0x01)
#define USB_DEVICE_AUDIO_PITCH_CONTROL_SELECTOR (0x02)
/*! @}*/

/*!
 * @name USB Audio class setup request types
 * @{
 */
/*! @brief Audio device class setup request set type */
#define USB_DEVICE_AUDIO_SET_REQUSET_INTERFACE (0x21)
#define USB_DEVICE_AUDIO_SET_REQUSET_ENDPOINT (0x22)

/*! @brief Audio device class setup request get type */
#define USB_DEVICE_AUDIO_GET_REQUSET_INTERFACE (0xA1)
#define USB_DEVICE_AUDIO_GET_REQUSET_ENDPOINT (0xA2)
/*! @}*/

/*! @brief Available common EVENT types in audio class callback */
typedef enum
{
    kUSB_DeviceAudioEventStreamSendResponse = 0x01U, /*!< Send data completed in stream pipe */
    kUSB_DeviceAudioEventStreamRecvResponse,         /*!< Data received in stream pipe */
    kUSB_DeviceAudioEventControlSendResponse,        /*!< Send data completed in audio control pipe */
} usb_device_audio_event_t;

/*!
 * @brief The audio device class-specific information.
 * The structure is used to pass the audio entity information filled by application.
 * Such as
 *     entity id(unit or terminal ID),
 *     entity type(unit or terminal type),
 *     and terminal type if the entity is a terminal.
 */
typedef struct _usb_device_audio_entity_struct
{
    uint8_t entityId;
    uint8_t entityType;
    uint16_t terminalType;
} usb_device_audio_entity_struct_t;

/*!
 * @brief The audio device class-specific information list.
 * The structure is used to pass the audio entity informations filled by application.
 * The type of each entity is usb_device_audio_entity_struct_t.
 * And the structure pointer is kept in usb_device_interface_struct_t::classSpecific.
 * Such as, if there are three entities(Out terminal, Camera terminal and processing unit),
 * the value of the count field is 3, the entity field saves the every entity information.
 */
typedef struct _usb_device_audio_entities_struct
{
    usb_device_audio_entity_struct_t *entity;
    uint8_t count;
} usb_device_audio_entities_struct_t;

/*! @brief The audio device class status structure */
typedef struct _usb_device_audio_struct
{
    usb_device_handle handle;                              /*!< The device handle */
    usb_device_class_config_struct_t *configStruct;        /*!< The configuration of the class. */
    usb_device_interface_struct_t *controlInterfaceHandle; /*!< Current control interface handle */
    usb_device_interface_struct_t *streamInterfaceHandle;  /*!< Current stream interface handle */
    uint8_t configuration;                                 /*!< Current configuration */
    uint8_t controlInterfaceNumber;                        /*!< The control interface number of the class */
    uint8_t controlAlternate;                              /*!< Current alternate setting of the control interface */
    uint8_t streamInterfaceNumber;                         /*!< The stream interface number of the class */
    uint8_t streamAlternate;                               /*!< Current alternate setting of the stream interface */
    uint8_t streamInPipeBusy;                              /*!< Stream IN pipe busy flag */
    uint8_t streamOutPipeBusy;                             /*!< Stream OUT pipe busy flag */
} usb_device_audio_struct_t;

#if USBCFG_AUDIO_CLASS_2_0
STRUCT_PACKED
struct _usb_device_control_range_struct
{
    uint16_t samplingFrequencyNumber;
    uint32_t samplingFrequencyMin;
    uint32_t samplingFrequencyMax;
    uint32_t samplingFrequencyRes;
} STRUCT_UNPACKED;
typedef struct _usb_device_control_range_struct usb_device_control_range_struct_t;
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @name USB Audio Class Driver
 * @{
 */
/*!
 * @brief Initializes the USB Audio class.
 *
 * This function obtains a usb device handle according to the controller id, initializes the Audio class
 * with the class configure parameters and creats the mutex for each pipe.
 *
 * @param controllerId The id of the controller. The value can be choosen from kUSB_ControllerKhci0,
 *  kUSB_ControllerKhci1, kUSB_ControllerEhci0 or kUSB_ControllerEhci1.
 * @param config The user configuration structure of type usb_device_class_config_struct_t. The user
 *  populates the members of this structure and passes the pointer of this structure
 *  into this function.
 * @param handle  It is out parameter. The class handle of the Audio class.
 * @return A USB error code or kStatus_USB_Success.
 * @retval kStatus_USB_Success The Audio class is initialized successfully.
 * @retval kStatus_USB_Busy No Audio device handle available for allocation.
 * @retval kStatus_USB_InvalidHandle The Audio device handle allocation failure.
 * @retval kStatus_USB_InvalidParameter The usb device handle allocation failure.
 */
extern usb_status_t USB_DeviceAudioInit(uint8_t controllerId,
                                        usb_device_class_config_struct_t *config,
                                        class_handle_t *handle);

/*!
 * @brief De-Initializes the USB Audio class.
 *
 * This function destroys the mutex for each pipe, deinit each endpoint of the Audio class and free
 * the Audio class handle.
 *
 * @param handle The class handle of the Audio class.
 * @return A USB error code or kStatus_USB_Success.
 * @retval kStatus_USB_Success The Audio class is de-initialized successfully.
 * @retval kStatus_USB_Error The endpoint deinit failure.
 * @retval kStatus_USB_InvalidHandle The Audio device handle or the Audio class handle is invalid.
 * @retval kStatus_USB_InvalidParameter The endpoint number of the Audio class handle is invalid.
 */
extern usb_status_t USB_DeviceAudioDeinit(class_handle_t handle);

/*!
 * @brief Handles the USB Audio class event.
 *
 * This function responses to various events includiing the common device events and the class specific events.
 * For class specific events, it calls the class callback defined in the application to deal with the class specific
 * event.
 *
 * @param handle The class handle of the Audio class.
 * @param event The event type.
 * @param param The class handle of the Audio class.
 * @return A USB error code or kStatus_USB_Success.
 * @retval kStatus_USB_Success The Audio class is de-initialized successfully.
 * @retval kStatus_USB_Error The configure structure of the Audio class handle is invalid.
 * @retval kStatus_USB_InvalidHandle The Audio device handle or the Audio class handle is invalid.
 * @retval kStatus_USB_InvalidParameter The endpoint number of the Audio class handle is invalid.
 * @retval Others The error code returned by class callback in application.
 */
extern usb_status_t USB_DeviceAudioEvent(void *handle, uint32_t event, void *param);

/*!
 * @brief Prime the endpoint to send packet to host.
 *
 * This function checks whether the endpoint is sending packet, then it primes the endpoint
 * with the buffer address and the buffer length if the pipe is not busy. Otherwise, it ignores this transfer by
 * returnning an error code.
 *
 * @param handle The class handle of the Audio class.
 * @param ep The endpoint number of the transfer.
 * @param buffer The pointer to the buffer to be transferrd.
 * @param length The length of the buffer to be transferrd.
 * @return A USB error code or kStatus_USB_Success.
 * @retval kStatus_USB_Success Prime to send packet successfully.
 * @retval kStatus_USB_Busy The endpoint is busy in transferring.
 * @retval kStatus_USB_InvalidHandle The Audio device handle or the Audio class handle is invalid.
 * @retval kStatus_USB_ControllerNotFound The controller interface is invalid.
 */
extern usb_status_t USB_DeviceAudioSend(class_handle_t handle, uint8_t ep, uint8_t *buffer, uint32_t length);

/*!
 * @brief Prime the endpoint to receive packet from host.
 *
 * This function checks whether the endpoint is receiving packet, then it primes the endpoint
 * with the buffer address and the buffer length if the pipe is not busy. Otherwise, it ignores this transfer by
 * returnning an error code.
 *
 * @param handle The class handle of the Audio class.
 * @param ep The endpoint number of the transfer.
 * @param buffer The pointer to the buffer to be transferrd.
 * @param length The length of the buffer to be transferrd.
 * @return A USB error code or kStatus_USB_Success.
 * @retval kStatus_USB_Success Prime to receive packet successfully.
 * @retval kStatus_USB_Busy The endpoint is busy in transferring.
 * @retval kStatus_USB_InvalidHandle The Audio device handle or the Audio class handle is invalid.
 * @retval kStatus_USB_ControllerNotFound The controller interface is invalid.
 */
extern usb_status_t USB_DeviceAudioRecv(class_handle_t handle, uint8_t ep, uint8_t *buffer, uint32_t length);

/*! @}*/

/*! @}*/

#endif /* __USB_DEVICE_AUDIO_H__ */