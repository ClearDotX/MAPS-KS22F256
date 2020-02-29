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

#ifndef _USB_HOST_HID_H_
#define _USB_HOST_HID_H_

/*******************************************************************************
 * hid class public structure, enumeration, macro, function
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @addtogroup usb_host_hid_drv
 * @{
 */

/*! @brief HID class-specific request (get report) */
#define USB_HOST_HID_GET_REPORT (0x01U)
/*! @brief HID class-specific request (get idle) */
#define USB_HOST_HID_GET_IDLE (0x02U)
/*! @brief HID class-specific request (get protocol) */
#define USB_HOST_HID_GET_PROTOCOL (0x03U)
/*! @brief HID class-specific request (set report) */
#define USB_HOST_HID_SET_REPORT (0x09U)
/*! @brief HID class-specific request (set idle) */
#define USB_HOST_HID_SET_IDLE (0x0AU)
/*! @brief HID class-specific request (set protocol) */
#define USB_HOST_HID_SET_PROTOCOL (0x0BU)

/*! @brief HID class code */
#define USB_HOST_HID_CLASS_CODE (3U)
/*! @brief HID sub-class code */
#define USB_HOST_HID_SUBCLASS_CODE_NONE (0U)
/*! @brief HID sub-class code */
#define USB_HOST_HID_SUBCLASS_CODE_BOOT (1U)
/*! @brief HID class protocol code */
#define USB_HOST_HID_PROTOCOL_KEYBOARD (1U)
/*! @brief HID class protocol code */
#define USB_HOST_HID_PROTOCOL_MOUSE (2U)
/*! @brief HID class protocol code */
#define USB_HOST_HID_PROTOCOL_NONE (0U)

/*! @brief HID get/set protocol request data code */
#define USB_HOST_HID_REQUEST_PROTOCOL_BOOT (0U)
/*! @brief HID get/set protocol request data code */
#define USB_HOST_HID_REQUEST_PROTOCOL_REPORT (1U)

/*! @brief hid instance structure, hid usb_host_class_handle pointer to this structure */
typedef struct _usb_host_hid_instance
{
    usb_host_handle hostHandle;                /*!< this instance's related host handle*/
    usb_device_handle deviceHandle;            /*!< this instance's related device handle*/
    usb_host_interface_handle interfaceHandle; /*!< this instance's related interface handle*/
    usb_host_pipe_handle controlPipe;          /*!< this instance's related device control pipe*/
    usb_host_pipe_handle inPipe;               /*!< hid interrupt in pipe*/
    usb_host_pipe_handle outPipe;              /*!< hid interrupt out pipe*/
    transfer_callback_t inCallbackFn;          /*!< hid interrupt in transfer callback function pointer*/
    void *inCallbackParam;                     /*!< hid interrupt in transfer callback parameter*/
    transfer_callback_t outCallbackFn;         /*!< hid interrupt out transfer callback function pointer*/
    void *outCallbackParam;                    /*!< hid interrupt out transfer callback parameter*/
    transfer_callback_t controlCallbackFn;     /*!< hid control transfer callback function pointer*/
    void *controlCallbackParam;                /*!< hid control transfer callback parameter*/
    usb_host_transfer_t *controlTransfer;      /*!< on-going control transfer*/

    uint16_t inPacketSize;  /*!< hid interrupt in max packet size*/
    uint16_t outPacketSize; /*!< hid interrupt out max packet size*/
} usb_host_hid_instance_t;

/*! @brief hid descriptor structure according to the 6.2.1 in HID spec */
typedef struct _usb_host_hid_descriptor
{
    uint8_t bLength;              /*!< total size of the HID descriptor*/
    uint8_t bDescriptorType;      /*!< Constant name specifying type of HID descriptor*/
    uint8_t bcdHID[2];            /*!< Numeric expression identifying the HID Class Specification release*/
    uint8_t bCountryCode;         /*!< Numeric expression identifying country code of the localized hardware*/
    uint8_t bNumDescriptors;      /*!< Numeric expression specifying the number of class descriptors*/
    uint8_t bHidDescriptorType;   /*!< Constant name identifying type of class descriptor*/
    uint8_t wDescriptorLength[2]; /*!< Numeric expression that is the total size of the Report descriptor*/
} usb_host_hid_descriptor_t;

/*! @brief hid descriptor structure according to the 6.2.1 in HID spec */
typedef struct _usb_host_hid_class_descriptor
{
    uint8_t bHidDescriptorType;   /*!< Constant name specifying type of optional descriptor*/
    uint8_t wDescriptorLength[2]; /*!< Numeric expression that is the total size of the optional descriptor*/
} usb_host_hid_class_descriptor_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @name USB host hid class APIs
 * @{
 */

/*!
 * @brief initialize the hid instance.
 *
 * This function allocate the resource for hid instance.
 *
 * @param[in] deviceHandle   the device handle.
 * @param[out] classHandle   return class handle.
 *
 * @retval kStatus_USB_Success        The device is initialized successfully.
 * @retval kStatus_USB_AllocFail      Allocate memory fail.
 */
extern usb_status_t USB_HostHidInit(usb_device_handle deviceHandle, usb_host_class_handle *classHandle);

/*!
 * @brief set interface.
 *
 * This function bind the interface with the hid instance.
 *
 * @param[in] classHandle      the class handle.
 * @param[in] interfaceHandle  the interface handle.
 * @param[in] alternateSetting the alternate setting value.
 * @param[in] callbackFn       this callback is called after this function completes.
 * @param[in] callbackParam    the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        The device is initialized successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 * @retval kStatus_USB_Busy           callback return status, there is no idle pipe.
 * @retval kStatus_USB_TransferStall  callback return status, the transfer is stall by device.
 * @retval kStatus_USB_Error          callback return status, open pipe fail, please reference to USB_HostOpenPipe.
 */
extern usb_status_t USB_HostHidSetInterface(usb_host_class_handle classHandle,
                                            usb_host_interface_handle interfaceHandle,
                                            uint8_t alternateSetting,
                                            transfer_callback_t callbackFn,
                                            void *callbackParam);

/*!
 * @brief de-initialize the hid instance.
 *
 * This function release the resource for hid instance.
 *
 * @param[in] deviceHandle   the device handle.
 * @param[in] classHandle    the class handle.
 *
 * @retval kStatus_USB_Success        The device is de-initialized successfully.
 */
extern usb_status_t USB_HostHidDeinit(usb_device_handle deviceHandle, usb_host_class_handle classHandle);

/*!
 * @brief get pipe max packet size.
 *
 * @param[in] classHandle the class handle.
 * @param[in] pipeType    It's value is USB_ENDPOINT_CONTROL, USB_ENDPOINT_ISOCHRONOUS, USB_ENDPOINT_BULK or
 * USB_ENDPOINT_INTERRUPT.
 *                        Please reference to usb_spec.h
 * @param[in] direction   pipe direction.
 *
 * @retval 0        The classHandle is NULL.
 * @retval max packet size.
 */
extern uint16_t USB_HostHidGetPacketsize(usb_host_class_handle classHandle, uint8_t pipeType, uint8_t direction);

/*!
 * @brief hid get report descriptor.
 *
 * This function implements hid report descriptor request.
 *
 * @param[in] classHandle   the class handle.
 * @param[out] buffer        the buffer pointer.
 * @param[in] buffer_len     the buffer length.
 * @param[in] callbackFn    this callback is called after this function completes.
 * @param[in] callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHidGetReportDescriptor(usb_host_class_handle classHandle,
                                                   uint8_t *buffer,
                                                   uint16_t buffer_len,
                                                   transfer_callback_t callbackFn,
                                                   void *callbackParam);

/*!
 * @brief receive data.
 *
 * This function implements hid receiving data.
 *
 * @param[in] classHandle   the class handle.
 * @param[out] buffer       the buffer pointer.
 * @param[in] bufferLength  the buffer length.
 * @param[in] callbackFn    this callback is called after this function completes.
 * @param[in] callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        receive request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostRecv.
 */
extern usb_status_t USB_HostHidRecv(usb_host_class_handle classHandle,
                                    uint8_t *buffer,
                                    uint32_t bufferLength,
                                    transfer_callback_t callbackFn,
                                    void *callbackParam);

/*!
 * @brief send data.
 *
 * This function implements hid sending data.
 *
 * @param[in] classHandle   the class handle.
 * @param[in] buffer        the buffer pointer.
 * @param[in] bufferLength  the buffer length.
 * @param[in] callbackFn    this callback is called after this function completes.
 * @param[in] callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        send request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSend.
 */
extern usb_status_t USB_HostHidSend(usb_host_class_handle classHandle,
                                    uint8_t *buffer,
                                    uint32_t bufferLength,
                                    transfer_callback_t callbackFn,
                                    void *callbackParam);

/*!
 * @brief hid get idle.
 *
 * This function implements hid class specific request (get idle).
 *
 * @param[in] classHandle   the class handle.
 * @param[in] reportId      report id.
 * @param[out] idleRate     return idle rate value.
 * @param[in] callbackFn    this callback is called after this function completes.
 * @param[in] callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHidGetIdle(usb_host_class_handle classHandle,
                                       uint8_t reportId,
                                       uint8_t *idleRate,
                                       transfer_callback_t callbackFn,
                                       void *callbackParam);

/*!
 * @brief hid set idle.
 *
 * This function implements hid class specific request (set idle).
 *
 * @param[in] classHandle   the class handle.
 * @param[in] reportId      report id.
 * @param[in] idleRate      idle rate value.
 * @param[in] callbackFn    this callback is called after this function completes.
 * @param[in] callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHidSetIdle(usb_host_class_handle classHandle,
                                       uint8_t reportId,
                                       uint8_t idleRate,
                                       transfer_callback_t callbackFn,
                                       void *callbackParam);

/*!
 * @brief hid get protocol.
 *
 * This function implements hid class specific request (get protocol).
 *
 * @param[in] classHandle   the class handle.
 * @param[out] protocol      return protocol value.
 * @param[in] callbackFn    this callback is called after this function completes.
 * @param[in] callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHidGetProtocol(usb_host_class_handle classHandle,
                                           uint8_t *protocol,
                                           transfer_callback_t callbackFn,
                                           void *callbackParam);

/*!
 * @brief hid set protocol.
 *
 * This function implements hid class specific request (set protocol).
 *
 * @param[in] classHandle   the class handle.
 * @param[in] protocol      protocol value.
 * @param[in] callbackFn    this callback is called after this function completes.
 * @param[in] callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHidSetProtocol(usb_host_class_handle classHandle,
                                           uint8_t protocol,
                                           transfer_callback_t callbackFn,
                                           void *callbackParam);

/*!
 * @brief hid get report.
 *
 * This function implements hid class specific request (get report).
 *
 * @param[in] classHandle   the class handle.
 * @param[in] reportId      report id.
 * @param[in] reportType    report type.
 * @param[out] buffer       the buffer pointer.
 * @param[in] bufferLength  the buffer length.
 * @param[in] callbackFn    this callback is called after this function completes.
 * @param[in] callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHidGetReport(usb_host_class_handle classHandle,
                                         uint8_t reportId,
                                         uint8_t reportType,
                                         uint8_t *buffer,
                                         uint32_t bufferLength,
                                         transfer_callback_t callbackFn,
                                         void *callbackParam);

/*!
 * @brief hid set report.
 *
 * This function implements hid class specific request (set report).
 *
 * @param[in] classHandle   the class handle.
 * @param[in] reportId      report id.
 * @param[in] reportType    report type.
 * @param[in] buffer        the buffer pointer.
 * @param[in] bufferLength  the buffer length.
 * @param[in] callbackFn    this callback is called after this function completes.
 * @param[in] callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHidSetReport(usb_host_class_handle classHandle,
                                         uint8_t reportId,
                                         uint8_t reportType,
                                         uint8_t *buffer,
                                         uint32_t bufferLength,
                                         transfer_callback_t callbackFn,
                                         void *callbackParam);

/*! @}*/

#ifdef __cplusplus
}
#endif

/*! @}*/

#endif /* _USB_HOST_HID_H_ */
