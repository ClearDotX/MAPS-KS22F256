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

#ifndef __USB_HOST_AUDIO_H__
#define __USB_HOST_AUDIO_H__

/*******************************************************************************
 * audio class private structure, enumeration, macro
 ******************************************************************************/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* structure for AUDIO class descriptor according to the 6.2.1 in AUDIO spec*/
#define AUDIO_FU_MUTE 0x01
#define AUDIO_FU_VOLUME 0x02
#define AUDIO_FU_BASS 0x03
#define AUDIO_FU_MID 0x04
#define AUDIO_FU_TREBLE 0x05
#define AUDIO_FU_GRAPHIC_EQ 0x06
#define AUDIO_FU_AGC 0x07
#define AUDIO_FU_DELAY 0x08
#define AUDIO_FU_BASS_BOOST 0x09

/* Audio class codes */
#define SET_COMMAND (0x00)
#define GET_COMMAND (0x80)
#define CUR_REQUEST (0x01)
#define MIN_REQUEST (0x02)
#define MAX_REQUEST (0x03)
#define RES_REQUEST (0x04)
#define MEM_REQUEST (0x05)
#define GET_STATUS (0xFF)

#define ITF_REQUEST (0x21)
#define EP_REQUEST (0x22)

#define AUDIO_FU_MUTE_MASK 0x01
#define AUDIO_FU_VOLUME_MASK 0x02
#define AUDIO_FU_BASS_MASK 0x04
#define AUDIO_FU_MID_MASK 0x08
#define AUDIO_FU_TREBLE_MASK 0x10
#define AUDIO_FU_GRAPHIC_EQ_MASK 0x20
#define AUDIO_FU_AGC_MASK 0x40
#define AUDIO_FU_DELAY_MASK 0x80
#define AUDIO_FU_BASS_BOOST_MASK 0x01

/* USB audio Endpoint Control Selectors */
#define AUDIO_EP_CONTROL_UNDEFINED (0x00)
#define AUDIO_SAMPLING_FREQ_CONTROL (0x01)
#define AUDIO_PITCH_CONTROL (0x02)

#define AUDIO_SAMPLING_FREQ_MASK (0x01)
#define AUDIO_PITCH_MASK (0x02)
typedef enum _fu_request_code
{
    kUSB_AudioCurMute = 0,
    kUSB_AudioCurVolume,
    kUSB_AudioMinVolume,
    kUSB_AudioMaxVolume,
    kUSB_AudioResVolume,
    NUMBER_OF_FEATURE_COMMANDS,
} fu_request_code_t;

typedef enum _ep_request_code
{
    kUSB_AudioCurPitch = 0,
    kUSB_AudioCurSamplingFreq,
    kUSB_AudioMinSamplingFreq,
    kUSB_AudioMaxSamplingFreq,
    kUSB_AudioResSamplingFreq,
    NUMBER_OF_ENDPOINT_COMMANDS,
} ep_request_code_t;

typedef union _audio_descriptor_union
{
    uint8_t *bufr;
    usb_descriptor_common_t *common;
    usb_descriptor_device_t *device;
    usb_descriptor_configuration_t *configuration;
    usb_descriptor_interface_t *interface;
    usb_descriptor_endpoint_t *endpoint;
} audio_descriptor_union_t;

/* audio command struct */
typedef struct _usb_audio_request
{
    uint8_t controlMask;
    uint8_t typeRequest;
    uint8_t codeRequest;
    uint8_t requestValue;
    uint8_t length;
} usb_audio_request_t;

#define USB_DESC_SUBTYPE_AUDIO_CS_HEADER 0x01
#define USB_DESC_SUBTYPE_AUDIO_CS_IT 0x02
#define USB_DESC_SUBTYPE_AUDIO_CS_OT 0x03
#define USB_DESC_SUBTYPE_AUDIO_CS_FU 0x06
#define USB_DESC_CLASS_ENDPOINT_GENERAL 0x01
#define USB_DESC_SUBTYPE_AS_CS_GENERAL 0X01
#define USB_DESC_SUBTYPE_AS_CS_FORMAT_TYPE 0X02

/*******************************************************************************
 * audio class public structure, enumeration, macro, function
 ******************************************************************************/
/*!
 * @addtogroup usb_host_audio_drv
 * @{
 */
/*! @brief Audio class code */
#define USB_AUDIO_CLASS_CODE 1
/*! @brief Audio class  control interface code*/
#define USB_AUDIO_SUBCLASS_CODE_CONTROL 1
/*! @brief Audio class  stream interface code*/
#define USB_AUDIO_SUBCLASS_CODE_AUDIOSTREAMING 2

/*! @brief AUDIO class-specific feature unit get current mute command*/
#define USB_AUDIO_GET_CUR_MUTE 0x80
/*! @brief AUDIO class-specific feature unit set current mute command*/
#define USB_AUDIO_SET_CUR_MUTE 0x00
/*! @brief AUDIO class-specific feature unit get current volume command*/
#define USB_AUDIO_GET_CUR_VOLUME 0x81
/*! @brief AUDIO class-specific feature unit set current volume command*/
#define USB_AUDIO_SET_CUR_VOLUME 0x01
/*! @brief AUDIO class-specific feature unit get minimum volume command*/
#define USB_AUDIO_GET_MIN_VOLUME 0x82
/*! @brief AUDIO class-specific feature unit set minimum volume command*/
#define USB_AUDIO_SET_MIN_VOLUME 0x02
/*! @brief AUDIO class-specific feature unit get maximum volume command*/
#define USB_AUDIO_GET_MAX_VOLUME 0x83
/*! @brief AUDIO class-specific feature unit set maximum volume command*/
#define USB_AUDIO_SET_MAX_VOLUME 0x03
/*! @brief AUDIO class-specific feature unit get resolution volume command*/
#define USB_AUDIO_GET_RES_VOLUME 0x84
/*! @brief AUDIO class-specific feature unit set resolution volume command*/
#define USB_AUDIO_SET_RES_VOLUME 0x04

/*! @brief AUDIO class-specific endpoint get current pitch control command*/
#define USB_AUDIO_GET_CUR_PITCH 0x80
/*! @brief AUDIO class-specific endpoint set current pitch control command*/
#define USB_AUDIO_SET_CUR_PITCH 0x00
/*! @brief AUDIO class-specific endpoint get current sampling frequency command*/
#define USB_AUDIO_GET_CUR_SAMPLING_FREQ 0x81
/*! @brief AUDIO class-specific endpoint set current sampling frequency command*/
#define USB_AUDIO_SET_CUR_SAMPLING_FREQ 0x01
/*! @brief AUDIO class-specific endpoint get minimum sampling frequency command*/
#define USB_AUDIO_GET_MIN_SAMPLING_FREQ 0x82
/*! @brief AUDIO class-specific endpoint set minimum sampling frequency command*/
#define USB_AUDIO_SET_MIN_SAMPLING_FREQ 0x02
/*! @brief AUDIO class-specific endpoint get maximum sampling frequency command*/
#define USB_AUDIO_GET_MAX_SAMPLING_FREQ 0x83
/*! @brief AUDIO class-specific endpoint set maximum sampling frequency command*/
#define USB_AUDIO_SET_MAX_SAMPLING_FREQ 0x03
/*! @brief AUDIO class-specific endpoint get resolution sampling frequency command*/
#define USB_AUDIO_GET_RES_SAMPLING_FREQ 0x84
/*! @brief AUDIO class-specific endpoint set resolution sampling frequency command*/
#define USB_AUDIO_SET_RES_SAMPLING_FREQ 0x04

/*! @brief audio control interface header descriptor structure */
typedef struct _usb_audio_ctrl_header_desc
{
    uint8_t blength;            /*!< total size of the header descriptor*/
    uint8_t bdescriptortype;    /*!< descriptor type of audio header descriptor*/
    uint8_t bdescriptorsubtype; /*!< subtype of audio header descriptor*/
    uint8_t bcdcdc[2];          /*!< Audio Device Class Specification Release Number in Binary-Coded Decimal*/
    uint8_t wtotallength[2];    /*!< Total number of bytes returned for the class-specific AudioControl interface
                                   descriptor. Includes the combined length of this descriptor header and all Unit and
                                   Terminal descriptors.*/
    uint8_t bincollection;      /*!< The number of AudioStreaming and MIDIStreaming interfaces in the Audio Interface
                                   Collection to which this AudioControl interface belongs to*/
} usb_audio_ctrl_header_desc_t;

/*! @brief audio control interface input terminal descriptor structure */
typedef struct _usb_audio_ctrl_it_desc
{
    uint8_t blength;            /*!< total size of the input terminal descriptor*/
    uint8_t bdescriptortype;    /*!< descriptor type of audio input terminal descriptor*/
    uint8_t bdescriptorsubtype; /*!< subtype of audio input terminal descriptor*/
    uint8_t bterminalid; /*!< Constant uniquely identifying the Terminal within the audio function. This value is used
                            in all requests to address this Terminal*/
    uint8_t wterminaltype[2];  /*!< Constant characterizing the type of Terminal*/
    uint8_t bassocterminal;    /*!< ID of the Output Terminal to which this Input Terminal is associated*/
    uint8_t bnrchannels;       /*!< Number of logical output channels in the Terminal's output audio channel cluster*/
    uint8_t wchannelconfig[2]; /*!< Describes the spatial location of the logical channels.*/
    uint8_t ichannelnames;     /*!< Index of a string descriptor, describing the name of the first logical channel*/
    uint8_t iterminal;         /*!<Index of a string descriptor, describing the Input Terminal*/
} usb_audio_ctrl_it_desc_t;

/*! @brief audio control interface output terminal descriptor structure */
typedef struct _usb_audio_ctrl_ot_desc
{
    uint8_t blength;            /*!< total size of the output terminal descriptor*/
    uint8_t bdescriptortype;    /*!< descriptor type of audio output terminal descriptor*/
    uint8_t bdescriptorsubtype; /*!< subtype of audio output terminal descriptor*/
    uint8_t bterminalid; /*!< Constant uniquely identifying the Terminal within the audio function. This value is used
                            in all requests to address this Terminal*/
    uint8_t wterminaltype[2]; /*!< Constant characterizing the type of Terminal*/
    uint8_t bassocterminal;   /*!<Constant, identifying the Input Terminal to which this Output Terminal is associated*/
    uint8_t bsourceid;        /*!<ID of the Unit or Terminal to which this Terminal is connected*/
    uint8_t iterminal;        /*!< Index of a string descriptor, describing the Output Terminal*/
} usb_audio_ctrl_ot_desc_t;

/*! @brief audio control interface feature unit descriptor structure */
typedef struct _usb_audio_ctrl_fu_desc
{
    uint8_t blength;            /*!< total size of the output terminal descriptor*/
    uint8_t bdescriptortype;    /*!< descriptor type of audio output terminal descriptor*/
    uint8_t bdescriptorsubtype; /*!< subtype of audio output terminal descriptor*/
    uint8_t bunitid;   /*!< Constant uniquely identifying the unit within the audio function. This value is used in all
                          requests to address this unit*/
    uint8_t bsourceid; /*!< ID of the Unit or Terminal to which this Feature Unit is connected*/
    uint8_t bcontrolsize; /*!< Size in bytes of an element of the bmaControls*/
} usb_audio_ctrl_fu_desc_t;

/*! @brief audio as isochronous audio data endpoint descriptor structure */
typedef struct _usb_audio_stream_specific_iso_endp_desc
{
    uint8_t blength;            /*!< total size of the descriptor*/
    uint8_t bdescriptortype;    /*!< descriptor type of the descriptor*/
    uint8_t bdescriptorsubtype; /*!< subtype of the descriptor*/
    uint8_t bmattributes;   /*!< A bit in the range D6..0 set to 1 indicates that the mentioned Control is supported by
                               this endpoint*/
    uint8_t blockdlayunits; /*!< Indicates the units used for the wLockDelay field*/
    uint8_t wlockdelay[2];  /*!< Indicates the time it takes this endpoint to reliably lock its internal clock recovery
                               circuitry. Units used depend on the value of the bLockDelayUnits field.*/
} usb_audio_stream_specific_iso_endp_desc_t;

/*! @brief audio standard as isochronous Synch endpoint descriptor structure */
typedef struct _usb_audio_stream_synch_endp_desc
{
    uint8_t blength;           /*!< total size of the descriptor*/
    uint8_t bdescriptortype;   /*!< descriptor type of the endpoint descriptor*/
    uint8_t bendpointaddress;  /*!< The address of the endpoint on the USB device described by this descriptor*/
    uint8_t bmattributes;      /*!< D3..2: Synchronization type, D1..0: Transfer type*/
    uint8_t wmaxpacketsize[2]; /*!< Maximum packet size this endpoint is capable of sending or receiving when this
                                  configuration is selected*/
    uint8_t binterval;         /*!< Interval for polling endpoint for data transfers expressed in milliseconds*/
    uint8_t brefresh;      /*!< This field indicates the rate at which an isochronous synchronization pipe provides new
                              synchronization feedback data*/
    uint8_t bsynchaddress; /*!< Must be reset to zero*/
} usb_audio_stream_synch_endp_desc_t;

/*! @brief audio class-specific as interface descriptor structure */
typedef struct _usb_audio_stream_spepific_as_intf_desc
{
    uint8_t blength;            /*!< total size of the descriptor*/
    uint8_t bdescriptortype;    /*!< descriptor type of the descriptor*/
    uint8_t bdescriptorsubtype; /*!< subtype of the descriptor*/
    uint8_t bterminallink; /*!< The Terminal ID of the Terminal to which the endpoint of this interface is connected*/
    uint8_t bdelay;        /*!< Expressed in number of frames*/
    uint8_t wformattag[2]; /*!< The Audio Data Format that has to be used to communicate with this interface*/
} usb_audio_stream_spepific_as_intf_desc_t;

/* format type descriptor */
/*! @brief audio  format type descriptor structure */
typedef struct _usb_audio_stream_format_type_desc
{
    uint8_t blength;            /*!< total size of the descriptor*/
    uint8_t bdescriptortype;    /*!< descriptor type of the descriptor*/
    uint8_t bdescriptorsubtype; /*!< subtype of the descriptor*/
    uint8_t bformattype;        /*!< Constant identifying the Format Type the AudioStreaming interface is using*/
    uint8_t bnrchannels;        /*!< Number of channels of device*/
    uint8_t bsubframesize;      /*!< bytes per audio subframe*/
    uint8_t bbitresolution;     /*!< bits per sample*/
    uint8_t bsamfreqtype;       /*!< frequency supported*/
    uint8_t tsamfreq[1][3];     /*!< sample frequency*/
} usb_audio_stream_format_type_desc_t;

/*! @brief audio instance structure, audio usb_host_class_handle pointer to this structure */
typedef struct _audio_instance
{
    usb_host_handle hostHandle;                  /*!< this instance's related host handle*/
    usb_device_handle deviceHandle;              /*!< this instance's related device handle*/
    usb_host_interface_handle streamIntfHandle;  /*!< this instance's audio stream interface handle*/
    usb_host_interface_handle controlIntfHandle; /*!< this instance's control stream interface handle*/
    usb_audio_stream_spepific_as_intf_desc_t
        *asIntfDesc; /*!< audio class class-specificas as interface descriptor pointer*/
    usb_audio_stream_format_type_desc_t
        *formatTypeDesc; /*!< audio class class-specificformat type  descriptor pointer*/
    usb_audio_stream_specific_iso_endp_desc_t
        *isoEndpDesc;                  /*!< audio class class-specific iso audio data endpoint descriptor pointer*/
    usb_host_pipe_handle isoInPipe;    /*!< audio class iso in pipe*/
    usb_host_pipe_handle isoOutPipe;   /*!< audio class iso out pipe*/
    transfer_callback_t inCallbackFn;  /*!< audio class iso in transfer callback function*/
    void *inCallbackParam;             /*!< audio class iso in transfer callback parameter*/
    transfer_callback_t outCallbackFn; /*!< audio class iso out transfer callback function*/
    void *outCallbackParam;            /*!< audio class iso out transfer callback function*/
    usb_audio_ctrl_header_desc_t *headerDesc; /*!< audio class header descriptor pointer*/
    usb_audio_ctrl_it_desc_t *itDesc;         /*!< audio class input terminal descriptor pointer*/
    usb_audio_ctrl_ot_desc_t *otDesc;         /*!< audio class output terminal descriptor pointer*/
    usb_audio_ctrl_fu_desc_t *fuDesc;         /*!< audio class  ifeature unit descriptor pointer*/
    usb_host_pipe_handle controlPipe;         /*!< aduio class  device control pipe*/
    transfer_callback_t controlCallbackFn;    /*!< aduio control transfer callback function*/
    void *controlCallbackParam;               /*!< aduio control transfer callback function*/
    usb_host_transfer_t *controlTransfer;     /*!< on-going control transfer*/
    uint16_t inPacketSize;                    /*!< audio iso in max packet size*/
    uint16_t outPacketSize;                   /*!< audio iso out max packet size*/
    uint8_t isSetup;                          /*!< is audio sertup transfer transmiting*/
    uint8_t isoEpNum;                         /*!< audio stream iso endpoint number*/
    uint8_t streamIfnum;                      /*!< audio stream iso interface number*/
} audio_instance_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @name USB host audio class APIs
 * @{
 */

/*!
 * @brief initialize the audio instance.
 *
 * This function allocate the resource for audio instance.
 *
 * @param deviceHandle       The device handle.
 * @param classHandlePtr return class handle.
 *
 * @retval kStatus_USB_Success        The device is initialized successfully.
 * @retval kStatus_USB_AllocFail      Allocate memory fail.
 */
extern usb_status_t USB_HostAudioInit(usb_device_handle deviceHandle, usb_host_class_handle *classHandlePtr);

/*!
 * @brief de-initialize the aduio instance.
 *
 * This function release the resource for aduio instance.
 *
 * @param deviceHandle   the device handle.
 * @param classHandle the class handle.
 *
 * @retval kStatus_USB_Success        The device is de-initialized successfully.
 */
extern usb_status_t USB_HostAudioDeinit(usb_device_handle deviceHandle, usb_host_class_handle classHandle);

/*!
 * @brief set aduio class stream interface.
 *
 * This function bind the interface with the audio instance.
 *
 * @param classHandle        The class handle.
 * @param interfaceHandle          The interface handle.
 * @param alternateSetting  The alternate setting value.
 * @param callbackFn          this callback is called after this function completes.
 * @param callbackParam    The first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        The device is initialized successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 * @retval kStatus_USB_Busy           callback return status, there is no idle pipe.
 * @retval kStatus_USB_TransferStall  callback return status, the transfer is stall by device.
 * @retval kStatus_USB_Error          callback return status, open pipe fail, please reference to USB_HostOpenPipe.
 */
extern usb_status_t USB_HostAudioStreamSetInterface(usb_host_class_handle classHandle,
                                                    usb_host_interface_handle interfaceHandle,
                                                    uint8_t alternateSetting,
                                                    transfer_callback_t callbackFn,
                                                    void *callbackParam);

/*!
 * @brief set aduio class control interface.
 *
 * This function bind the interface with the audio instance.
 *
 * @param classHandle        The class handle.
 * @param interfaceHandle          The interface handle.
 * @param alternateSetting  The alternate setting value.
 * @param callbackFn          This callback is called after this function completes.
 * @param callbackParam    The first parameter in the callback function.
 *
 * @retval kStatus_USB_Success         The device is initialized successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 * @retval kStatus_USB_Busy           callback return status, there is no idle pipe.
 * @retval kStatus_USB_TransferStall  callback return status, the transfer is stall by device.
 * @retval kStatus_USB_Error          callback return status, open pipe fail, please reference to USB_HostOpenPipe.
 */
extern usb_status_t USB_HostAudioControlSetInterface(usb_host_class_handle classHandle,
                                                     usb_host_interface_handle interfaceHandle,
                                                     uint8_t alternateSetting,
                                                     transfer_callback_t callbackFn,
                                                     void *callbackParam);

/*!
 * @brief get pipe max packet size.
 *
 * @param classHandle the class handle.
 * @param pipeType    It's value is USB_ENDPOINT_CONTROL, USB_ENDPOINT_ISOCHRONOUS, USB_ENDPOINT_BULK or
 * USB_ENDPOINT_INTERRUPT.
 *                     Please reference to usb_spec.h
 * @param direction    pipe direction.
 *
 * @retval 0        The classHandle is NULL.
 * @retval max packet size.
 */
extern uint16_t USB_HostAudioPacketSize(usb_host_class_handle classHandle, uint8_t pipeType, uint8_t direction);

/*!
 * @brief audio stream receive data.
 *
 * This function implements aduio receiving data.
 *
 * @param classHandle      The class handle.
 * @param buffer                The buffer pointer.
 * @param bufferLen          The buffer length.
 * @param callbackFn         This callback is called after this function completes.
 * @param callbackParam  The first parameter in the callback function.
 *
 * @retval kStatus_USB_Success          Receive request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy               There is no idle transfer.
 * @retval kStatus_USB_Error              pipe is not initialized.
 *                                                       Or, send transfer fail, please reference to USB_HostRecv.
 */
extern usb_status_t USB_HostAudioStreamRecv(usb_host_class_handle classHandle,
                                            uint8_t *buffer,
                                            uint32_t bufferLen,
                                            transfer_callback_t callbackFn,
                                            void *callbackParam);

/*!
 * @brief audio stream send data.
 *
 * This function implements aduio sending data.
 *
 * @param classHandle      The class handle.
 * @param buffer                The buffer pointer.
 * @param bufferLen          The buffer length.
 * @param callbackFn         This callback is called after this function completes.
 * @param callbackParam  The first parameter in the callback function.
 *
 * @retval kStatus_USB_Success          Receive request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy               There is no idle transfer.
 * @retval kStatus_USB_Error              pipe is not initialized.
 *                                                       Or, send transfer fail, please reference to USB_HostSend.
 */
extern usb_status_t USB_HostAudioStreamSend(usb_host_class_handle classHandle,
                                            uint8_t *buffer,
                                            uint32_t bufferLen,
                                            transfer_callback_t callbackFn,
                                            void *callbackParam);

/*!
 * @brief get audio stream current altsetting descriptor.
 *
 * This function implements get audio stream current altsetting descriptor.
 *
 * @param classHandle               The class handle.
 * @param asIntfDesc                 The pointer of class specific AS interface descriptor.
 * @param formatTypeDesc       The pointer of format type descriptor.
 * @param isoEndpSpecDesc    The pointer of specific iso endp descriptor.
 *
 * @retval kStatus_USB_Success          Get audio stream current altsetting descriptor request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 *
 */
extern usb_status_t USB_HostAudioStreamGetCurrentAltsettingDescriptors(
    usb_host_class_handle classHandle,
    usb_audio_stream_spepific_as_intf_desc_t **asIntfDesc,
    usb_audio_stream_format_type_desc_t **formatTypeDesc,
    usb_audio_stream_specific_iso_endp_desc_t **isoEndpDesc);

/*!
 * @brief usb audio feature unit request.
 *
 * This function implements usb audio feature unit request.
 *
 * @param classHandle      The class handle.
 * @param channeNo        The channel number of audio feature unit.
 * @param buf                    The feature unit request buffer pointer.
 * @param cmdCode          The feature unit command code, for example USB_AUDIO_GET_CUR_MUTE etc.
 * @param callbackFn         This callback is called after this function completes.
 * @param callbackParam  The first parameter in the callback function.
 *
 * @retval kStatus_USB_Success            Feature unit request successfully.
 * @retval kStatus_USB_InvalidHandle    The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy                There is no idle transfer.
 * @retval kStatus_USB_Error                Send transfer fail, please reference to USB_HostSendSetup.
 *
 */
extern usb_status_t USB_HostAudioFeatureUnitRequest(usb_host_class_handle classHandle,
                                                    uint8_t channelNo,
                                                    void *buf,
                                                    uint32_t cmdCode,
                                                    transfer_callback_t callbackFn,
                                                    void *callbackParam);

/*!
 * @brief usb audio endpoint request.
 *
 * This function implements usb audio endpoint request.
 *
 * @param classHandle      The class handle.
 * @param buf                    The feature unit buffer pointer.
 * @param cmdCode          The feature unit command code, for example USB_AUDIO_GET_CUR_PITCH etc .
 * @param callbackFn         This callback is called after this function completes.
 * @param callbackParam   The first parameter in the callback function.
 *
 * @retval kStatus_USB_Success          Endpoint request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy               There is no idle transfer.
 * @retval kStatus_USB_Error               Send transfer fail, please reference to USB_HostSendSetup.
 *
 */
extern usb_status_t USB_HostAudioEndpointRequest(usb_host_class_handle classHandle,
                                                 void *buf,
                                                 uint32_t cmdCode,
                                                 transfer_callback_t callbackFn,
                                                 void *callbackParam);

/*! @}*/
#ifdef __cplusplus
}
#endif
/*! @}*/
#endif /* __USB_HOST_AUDIO_H__ */
