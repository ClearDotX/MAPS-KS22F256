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

#ifndef __USB_HOST_PHDC_H__
#define __USB_HOST_PHDC_H__

/*******************************************************************************
 * phdc class public structure, enumeration, macro, function
 ******************************************************************************/
/*!
 * @addtogroup usb_host_phdc_drv
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief PHDC class code */
#define USB_HOST_PHDC_CLASS_CODE (0x0FU)
/*! @brief PHDC sub class code */
#define USB_HOST_PHDC_SUBCLASS_CODE (0x00U)
/*! @brief PHDC protocol */
#define USB_HOST_PHDC_PROTOCOL (0x00U)
/*! @brief PHDC get status request */
#define USB_HOST_PHDC_GET_STATUS_REQUEST (0x00U)
/*! @brief PHDC set feature request */
#define USB_HOST_PHDC_SET_FEATURE_REQUEST (0x03U)
/*! @brief PHDC clear feature request */
#define USB_HOST_PHDC_CLEAR_FEATURE_REQUEST (0x01U)
/*! @brief PHDC meta-data feature */
#define USB_HOST_PHDC_FEATURE_METADATA (0x01U)
/*! @brief PHDC QoS information encoding feature */
#define USB_HOST_PHDC_QOS_ENCODING_VERSION (0x01U)

/*! @brief meta-data message preamble signature size */
#define USB_HOST_PHDC_MESSAGE_PREAMBLE_SIGNATURE_SIZE (0x10U)

/*! @brief PHDC class function descriptor type */
#define USB_HOST_PHDC_CLASSFUNCTION_DESCRIPTOR (0x20U)
/*! @brief PHDC QoS descriptor type */
#define USB_HOST_PHDC_QOS_DESCRIPTOR (0x21U)
/*! @brief PHDC function extension descriptor type */
#define USB_HOST_PHDC_11073PHD_FUNCTION_DESCRIPTOR (0x30U)
/*! @brief PHDC meta-data descriptor type */
#define USB_HOST_PHDC_METADATA_DESCRIPTOR (0x22U)

/*! @brief PHDC class function descriptor structure as defined by the PHDC class specification */
typedef struct _usb_host_phdc_class_function_desciptor
{
    uint8_t bLength;         /*!< class function descriptor length */
    uint8_t bDescriptortype; /*!< PHDC_CLASSFUNCTION_DESCRIPTOR type */
    uint8_t bPhdcDataCode;   /*!< Data/Messaging format code */
    uint8_t bmCapability;    /*!< If bit 0 is 1 the meta-data message preamble is implemented and 0 if it is not */
} usb_host_phdc_class_function_desciptor_t;

/*! @brief Function extension descriptor (device specialization) structure as defined by the PHDC class specification.
 */
typedef struct _usb_host_phdc_function_extension_descriptor
{
    uint8_t bLength;               /*!< function extension descriptor length */
    uint8_t bDescriptortype;       /*!< PHDC_CLASSFUNCTION_DESCRIPTOR type */
    uint8_t bReserved;             /*!< reserved for future use */
    uint8_t bNumDevSpecs;          /*!< number of wDevSpecializations */
    uint16_t *wDevSpecializations; /*!< variable length list that defines the device specialization */
} usb_host_phdc_function_extension_descriptor_t;

/*! @brief QoS descriptor structure as defined by the PHDC class specification. */
typedef struct _usb_host_phdc_qos_descriptor
{
    uint8_t bLength;              /*!< QoS descriptor length */
    uint8_t bDescriptortype;      /*!< PHDC_QOS_DESCRIPTOR type */
    uint8_t bQosEncodingVersion;  /*!< version of QoS information encoding */
    uint8_t bmLatencyReliability; /*!< latency/reliability bin for the QoS data */
} usb_host_phdc_qos_descriptor_t;

/*! @brief Meta-data descriptor structure as defined by the PHDC class specification. */
typedef struct _usb_host_phdc_metadata_descriptor
{
    uint8_t bLength;         /*!< meta-data descriptor length */
    uint8_t bDescriptortype; /*!< Descriptor type */
    uint8_t *bOpaqueData;    /*!< opaque meta-data */
} usb_host_phdc_metadata_descriptor_t;

/*! @brief Meta-data message preamble structure as defined by the PHDC class specification. */
typedef struct _usb_host_phdc_metadata_preamble
{
    uint8_t
        aSignature[USB_HOST_PHDC_MESSAGE_PREAMBLE_SIGNATURE_SIZE]; /*!< constant used to give preamble verifiability */
    uint8_t bNumberTransfers;     /*!< count of following transfer to which the QoS setting applies */
    uint8_t bQosEncodingVersion;  /*!< version of QoS information encoding */
    uint8_t bmLatencyReliability; /*!< refer to latency/reliability bin for the QoS data */
    uint8_t bOpaqueDataSize;      /*!< opaque QoS data or meta-data size */
    uint8_t *bOpaqueData;         /*!< opaque meta-data */
} usb_host_phdc_metadata_preamble_t;

/*! @brief PHDC instance structure */
typedef struct _usb_host_phdc_instance
{
    usb_host_handle hostHandle;                /*!< the host handle */
    usb_device_handle deviceHandle;            /*!< the device handle */
    usb_host_interface_handle interfaceHandle; /*!< the interface handle */
    usb_host_pipe_handle controlPipe;          /*!< the control pipe */
    usb_host_pipe_handle interruptPipe;        /*!< the interrupt pipe */
    usb_host_pipe_handle bulkInPipe;           /*!< the bulk in pipe */
    usb_host_pipe_handle bulkOutPipe;          /*!< the bulk out pipe */
    transfer_callback_t inCallbackFn;          /*!< the callback function is called when the PHDC receives complete */
    void *inCallbackParam;                     /*!< the first param of the in callback function */
    transfer_callback_t outCallbackFn;         /*!< the callback function is called when the PHDC sends complete */
    void *outCallbackParam;                    /*!< the first param of the out callback function */
    transfer_callback_t controlCallbackFn;     /*!< the control callback function */
    void *controlCallbackParam;                /*!< the first param of the control callback function */
    usb_host_transfer_t *controlTransfer;      /*!< the control transfer pointer */
    usb_host_ep_t interruptInEndpointInformation; /*!< the interrupt in information */
    usb_host_ep_t bulkInEndpointInformation;      /*!< the bulk in information */
    usb_host_ep_t bulkOutEndpointInformation;     /*!< the bulk out information */
    uint8_t isMessagePreambleEnabled; /*!< the flag is used to check the message preamble feature is enabled or not */
    uint8_t numberTransferBulkOut;    /*!< the number of transfer that follow Meta-data Message Preamble */
    uint8_t numberTransferBulkIn;     /*!< the number of transfer that follow Meta-data Message Preamble */
} usb_host_phdc_instance_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @name USB host phdc class APIs
 * @{
 */

/*!
 * @brief initialize the phdc instance.
 *
 * This function allocate the resource for phdc instance.
 *
 * @param deviceHandle       the device handle.
 * @param classHandle        return class handle.
 *
 * @retval kStatus_USB_Success        The device is initialized successfully.
 * @retval kStatus_USB_AllocFail      Allocate memory fail.
 */
extern usb_status_t USB_HostPhdcInit(usb_host_handle deviceHandle, usb_host_class_handle *classHandle);

/*!
 * @brief set interface.
 *
 * This function bind the interface with the phdc instance.
 *
 * @param classHandle      the class handle.
 * @param interfaceHandle  the interface handle.
 * @param alternateSetting the alternate setting value.
 * @param callbackFn       this callback is called after this function completes.
 * @param callbackParam    the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        The device is initialized successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 * @retval kStatus_USB_Busy           callback return status, there is no idle pipe.
 * @retval kStatus_USB_TransferStall  callback return status, the transfer is stall by device.
 * @retval kStatus_USB_Error          callback return status, open pipe fail, please reference to USB_HostOpenPipe.
 */
extern usb_status_t USB_HostPhdcSetInterface(usb_host_class_handle classHandle,
                                             usb_host_interface_handle interfaceHandle,
                                             uint8_t alternateSetting,
                                             transfer_callback_t callbackFn,
                                             void *callbackParam);

/*!
 * @brief de-initialize the phdc instance.
 *
 * This function release the resource for phdc instance.
 *
 * @param deviceHandle   the device handle.
 * @param classHandle the class handle.
 *
 * @retval kStatus_USB_Success        The device is de-initialized successfully.
 */
extern usb_status_t USB_HostPhdcDeinit(usb_host_handle deviceHandle, usb_host_class_handle classHandle);

/*!
 * @brief receive data.
 *
 * This function implements phdc receiving data.
 *
 * @param classHandle   the class handle.
 * @param qos           QoS of the data being received.
 * @param buffer        the buffer pointer.
 * @param bufferLength  the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        receive request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostRecv.
 */
extern usb_status_t USB_HostPhdcRecv(usb_host_class_handle classHandle,
                                     uint8_t qos,
                                     uint8_t *buffer,
                                     uint32_t bufferLength,
                                     transfer_callback_t callbackFn,
                                     void *callbackParam);

/*!
 * @brief send data.
 *
 * This function implements phdc sending data.
 *
 * @param classHandle   the class handle.
 * @param buffer        the buffer pointer.
 * @param bufferLength  the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        send request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSend.
 */
extern usb_status_t USB_HostPhdcSend(usb_host_class_handle classHandle,
                                     uint8_t *buffer,
                                     uint32_t bufferLength,
                                     transfer_callback_t callbackFn,
                                     void *callbackParam);

/*!
 * @brief phdc sends control request.
 *
 * @param classHandle   the class handle.
 * @param request_type  setup packet request.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        send request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSend.
 */
extern usb_status_t USB_HostPhdcSendControlRequest(usb_host_class_handle classHandle,
                                                   uint8_t request,
                                                   transfer_callback_t callbackFn,
                                                   void *callbackParam);

/*!
 * @brief phdc set and clear feature endpoint halt request.
 *
 * @param classHandle   the class handle.
 * @param request       setup packet request.
 * @param param         request parameter
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        send request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSend.
 */
extern usb_status_t USB_HostPhdcSetClearFeatureEndpointHalt(usb_host_class_handle classHandle,
                                                            uint8_t request,
                                                            void *param,
                                                            transfer_callback_t callbackFn,
                                                            void *callbackParam);

/*!
 * @brief USB_HostPhdcGetEndpointInformation.
 * This function returns the PHDC endpoint information structure contains endpoint
 * descriptor and endpoint extended descriptor.
 *
 * @param classHandle   the class handle.
 * @param pipeType      pipe type.
 * @param direction     pipe direction.
 *
 * @retval endpointReturn   All input parameters are valid.
 * @retval NULL             One or more input parameters are invalid.
 */
usb_host_ep_t *USB_HostPhdcGetEndpointInformation(usb_host_class_handle classHandle,
                                                  uint8_t pipeType,
                                                  uint8_t direction);

/*! @}*/

#ifdef __cplusplus
}
#endif

/*! @}*/

#endif /* _USB_HOST_PHDC_H_ */
