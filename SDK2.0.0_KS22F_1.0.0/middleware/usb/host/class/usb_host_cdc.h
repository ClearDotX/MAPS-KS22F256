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

#ifndef __USB_HOST_CDC_H__
#define __USB_HOST_CDC_H__

/*!
 * @addtogroup host_cdc
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* class-specific request  PSTN*/
/*! @brief CDC class-specific request (SET_LINE_CODING) */
#define USB_HOST_CDC_SET_LINE_CODING 0x20U
/*! @brief CDC class-specific request (GET_LINE_CODING) */
#define USB_HOST_CDC_GET_LINE_CODING 0x21U
/*! @brief CDC class-specific request (SET_CONTROL_LINE_STATE) */
#define USB_HOST_CDC_SET_CONTROL_LINE_STATE 0x22U

/*! @brief CDC class-specific notifications(SerialState) bitmap*/
#define USB_HOST_ACM_UART_STATE_BITMAP_BTXCARRITER 0x01U
/*! @brief CDC class-specific notifications(SerialState) bitmap*/
#define USB_HOST_ACM_UART_STATE_BITMAP_BRXCARRITER 0x02U
/*! @brief CDC class-specific notifications(SerialState) bitmap*/
#define USB_HOST_ACM_UART_STATE_BITMAP_BBREAK 0x04U
/*! @brief CDC class-specific notifications(SerialState) bitmap*/
#define USB_HOST_ACM_UART_STATE_BITMAP_BBRINGSIGNAL 0x10U

/*! @brief CDC class-specific request (SET_CONTROL_LINE_STATE) bitmap*/
#define USB_HOST_CDC_CONTROL_LINE_STATE_DTR 0x01U
/*! @brief CDC class-specific request (SET_CONTROL_LINE_STATE) bitmap*/
#define USB_HOST_CDC_CONTROL_LINE_STATE_RTS 0x02U

/*CDC SPEC*/
/*! @brief CDC class-specific bDescriptor SubType in functional descriptors*/
#define USB_HOST_DESC_SUBTYPE_HEADER 0x00U
/*! @brief CDC class-specific bDescriptor SubType in functional descriptors*/
#define USB_HOST_DESC_SUBTYPE_CM 0x01U
/*! @brief CDC class-specific bDescriptor SubType in functional descriptors*/
#define USB_HOST_DESC_SUBTYPE_ACM 0x02U
/*! @brief CDC class-specific bDescriptor SubType in functional descriptors*/
#define USB_HOST_DESC_SUBTYPE_DLM 0x03U
/*! @brief CDC class-specific bDescriptor SubType in functional descriptors*/
#define USB_HOST_DESC_SUBTYPE_TR 0x04U
/*! @brief CDC class-specific bDescriptor SubType in functional descriptors*/
#define USB_HOST_DESC_SUBTYPE_TC_LSR 0x05U
/*! @brief CDC class-specific bDescriptor SubType in functional descriptors*/
#define USB_HOST_DESC_SUBTYPE_UNION 0x06U
/*! @brief CDC class-specific bDescriptor SubType in functional descriptors*/
#define USB_HOST_DESC_SUBTYPE_CS 0x07U
/*! @brief CDC class-specific bDescriptor SubType in functional descriptors*/
#define USB_HOST_DESC_SUBTYPE_TOM 0x08U

/*referance cdc spec page20*/
/*! @brief CDC class-specific code, Communications Interface Class Code*/
#define USB_HOST_CDC_COMMUNICATIONS_CLASS_CODE 0x02U
/*! @brief CDC class-specific code,Communications Class Subclass Codes*/
#define USB_HOST_CDC_SUBCLASS_ACM_CODE 0x02U
/*No class specific protocol required ,referance cdc spec page22*/
#define USB_HOST_CDC_PROTOCOL_CODE 0x00U
/*! @brief CDC class-specific code,Data Class Interface Codes*/
#define USB_HOST_CDC_DATA_CLASS_CODE 0x0AU
/*this field is un-used for Data Class interfaces and should have a value of 00h.*/
#define USB_HOST_CDC_DATA_SUBCLASS_CODE 0x00U
/*No class specific protocol required ,referance cdc spec page22*/
#define USB_HOST_CDC_DATA_PROTOCOL_CODE 0x00U

/*! @brief cdc GetLineCoding structure according to the 6.3 in PSTN spec */
typedef struct _usb_host_cdc_line_coding_struct
{
    uint32_t dwDTERate;  /*!< Data terminal rate, in bits per second*/
    uint8_t bCharFormat; /*!< Stop bits*/
    uint8_t bParityType; /*!< Parity*/
    uint8_t bDataBits;   /*!< Data bits (5, 6, 7, 8 or 16).*/
} usb_host_cdc_line_coding_struct_t;

/*! @brief cdc GetLineCoding structure according to the 6.3 in PSTN spec */
typedef struct _usb_host_cdc_control_line_state_struct
{
    uint16_t line_state; /*!< D1, This signal corresponds to V.24 signal 105 and RS-232 signal RTS*/
                         /*!< D0, This signal corresponds to V.24 signal 108/2 and RS-232 signal DTR*/
} usb_host_cdc_control_line_state_struct_t;

/*! @brief cdc SerialState structure according to the 6.5.4 in PSTN spec */
typedef struct _usb_host_cdc_acm_state_struct
{
    uint8_t reserved[8];  /*!<notify response by the device, this is used as notification header which is return by the
                             device */
    uint8_t bmstate;      /*!<UART State Bitmap Values*/
    uint8_t reserved1[1]; /*!<fix 4B align issue*/
    uint8_t reserved2[2]; /*!<fix 4B align issue*/
} usb_host_cdc_acm_state_struct_t;

/*! @brief cdc Header Functional Descriptor structure according to the 5.2.3 in cdc spec */
typedef struct _usb_host_cdc_head_function_desc_struct
{
    uint8_t bFunctionLength;    /*!<Size of this descriptor in bytes.*/
    uint8_t bDescriptorType;    /*!<CS_INTERFACE descriptor type.*/
    uint8_t bDescriptorSubtype; /*!<Header functional descriptor subtype.*/
    uint8_t bcdCDC[2]; /*!<USB Class Definitions for Communications Devices Specification release number in binary-coded
                          decimal.*/
} usb_host_cdc_head_function_desc_struct_t;

/*! @brief cdc Call Management Functional Descriptor structure according to the 5.3.1 in pstn spec */
typedef struct _usb_host_cdc_call_manage_desc_struct
{
    uint8_t bFunctionLength;    /*!<Size of this descriptor in bytes.*/
    uint8_t bDescriptorType;    /*!<CS_INTERFACE.*/
    uint8_t bDescriptorSubtype; /*!<Call Management functional descriptor subtype.*/
    uint8_t bmCapabilities;     /*!<The capabilities that this configuration supports.*/
    uint8_t bDataInterface;     /*!<Interface number of Data Class interface optionally used for call management..*/
} usb_host_cdc_call_manage_desc_struct_t;

/*! @brief cdc Abstract Control Management Functional Descriptor structure according to the 5.3.2 in pstn spec */
typedef struct _usb_host_cdc_abstract_control_desc_struct
{
    uint8_t bFunctionLength;    /*!<Size of this descriptor in bytes.*/
    uint8_t bDescriptorType;    /*!<CS_INTERFACE.*/
    uint8_t bDescriptorSubtype; /*!<Abstract Control Management functional descriptor subtype.*/
    uint8_t bmCapabilities;     /*!<The capabilities that this configuration supports.*/
} usb_host_cdc_abstract_control_desc_struct_t;

/*! @brief cdc Direct Line Management Functional Descriptor structure according to the 5.3.3 in pstn spec */
typedef struct _usb_host_cdc_direct_line_desc_struct
{
    uint8_t bFunctionLength;    /*!<Size of this descriptor in bytes.*/
    uint8_t bDescriptorType;    /*!<CS_INTERFACE.*/
    uint8_t bDescriptorSubtype; /*!<Direct Line Management functional descriptor subtype,.*/
    uint8_t bmCapabilities;     /*!<The capabilities that this configuration supports.*/
} usb_host_cdc_direct_line_desc_struct_t;

/*! @brief cdc Telephone Ringer Functional Descriptor structure according to the 5.3.4 in pstn spec */
typedef struct _usb_host_cdc_telephone_ringer_desc_struct
{
    uint8_t bFunctionLength;    /*!<Size of this descriptor in bytes.*/
    uint8_t bDescriptorType;    /*!<CS_INTERFACE.*/
    uint8_t bDescriptorSubtype; /*!<Telephone Ringer functional descriptor subtype*/
    uint8_t bRingerVolSteps;    /*!<Number of discrete steps in volume supported by the ringer,.*/
    uint8_t bNumRingerPatterns; /*!<Number of ringer patterns supported.*/
} usb_host_cdc_telephone_ringer_desc_struct_t;

/*! @brief cdc Telephone Call and Line State Reporting Capabilities Descriptor structure according to the 5.3.6 in pstn
 * spec */
typedef struct _usb_host_cdc_tcLsr_desc_struct
{
    uint8_t bFunctionLength;    /*!<Size of this descriptor in bytes.*/
    uint8_t bDescriptorType;    /*!<CS_INTERFACE.*/
    uint8_t bDescriptorSubtype; /*!<Telephone Call State Reporting Capabilities descriptor subtype.*/
    uint8_t bmCapabilities[4];  /*!<Call and line state reporting capabilities of the device.*/
} usb_host_cdc_tcLsr_desc_struct_t;

/*! @brief cdc Header Functional Descriptor structure according to the 5.2.3 in cdc spec */
typedef struct _usb_host_cdc_union_interface_desc_struct
{
    uint8_t bFunctionLength;    /*!<Size of this descriptor in bytes.*/
    uint8_t bDescriptorType;    /*!<CS_INTERFACE descriptor type.*/
    uint8_t bDescriptorSubtype; /*!<Union Functional Descriptor SubType.*/
    uint8_t bControlInterface;  /*!<USB Class Definitions for Communications Devices Specification release number in
                                   binary-coded decimal.*/
} usb_host_cdc_union_interface_desc_struct_t;

/*! @brief cdc Telephone Operational Modes Functional Descriptor structure according to the 5.3.5 in pstn spec */
typedef struct _usb_host_cdc_tom_desc_struct
{
    uint8_t bFunctionLength;    /*!<Size of this descriptor in bytes.*/
    uint8_t bDescriptorType;    /*!<CS_INTERFACE.*/
    uint8_t bDescriptorSubtype; /*!<Telephone Operational Modes functional descriptor subtype.*/
    uint8_t bmCapabilities;     /*!<operational modes:.*/
} usb_host_cdc_tom_desc_struct_t;

/*! @brief cdc common Functional Descriptor structure */
typedef struct _usb_host_cdc_common_desc_struct
{
    uint8_t bFunctionLength;    /*!<Size of this descriptor in bytes.*/
    uint8_t bDescriptorType;    /*!<CS_INTERFACE descriptor type.*/
    uint8_t bDescriptorSubtype; /*!<Header functional descriptor subtype.*/
} usb_host_cdc_common_desc_struct_t;

/*! @brief cdc union Functional Descriptor structure for analyse class specific descriptor */
typedef union _usb_cdc_func_desc_struct
{
    usb_host_cdc_common_desc_struct_t common;
    usb_host_cdc_head_function_desc_struct_t head;
    usb_host_cdc_union_interface_desc_struct_t unionDesc;
    usb_host_cdc_call_manage_desc_struct_t callManage;
    usb_host_cdc_abstract_control_desc_struct_t acm;
    usb_host_cdc_direct_line_desc_struct_t dl;
    usb_host_cdc_telephone_ringer_desc_struct_t tr;
    usb_host_cdc_tom_desc_struct_t tom;
    usb_host_cdc_tcLsr_desc_struct_t tcLsr;
} usb_cdc_func_desc_struct_t;

typedef struct _usb_host_cdc_instance_struct
{
    usb_host_handle hostHandle;                             /*!< The handle of the usb host. */
    usb_device_handle deviceHandle;                         /*!< The handle of the usb device structure. */
    usb_host_interface_handle controlInterfaceHandle;       /*!< The handle of the cdc device control interface. */
    usb_host_interface_handle dataInterfaceHandle;          /*!< The handle of the cdc device data interface. */
    usb_host_pipe_handle interruptPipe;                     /*!< Interrupt pipe for the cdc device data interface. */
    usb_host_pipe_handle controlPipe;                       /*!< control pipe*/
    usb_host_pipe_handle inPipe;                            /*!< cdc bulk in pipe*/
    usb_host_pipe_handle outPipe;                           /*!< cdc bulk out pipe*/
    usb_host_cdc_head_function_desc_struct_t *headDesc;     /*!< cdc class-specific head function descriptor*/
    usb_host_cdc_call_manage_desc_struct_t *callManageDesc; /*!< cdc class-specific call manage descriptor*/
    usb_host_cdc_abstract_control_desc_struct_t
        *abstractControlDesc; /*!< cdc class-specific abstract control descriptor*/
    usb_host_cdc_union_interface_desc_struct_t *unionInterfaceDesc; /*!< cdc class-specific union function descriptor*/
    void *interruptCallbackParam;                                   /*!< cdc interrupt in transfer callback parameter*/
    void *controlCallbackParam;                                     /*!< cdc control transfer callback parameter*/
    void *inCallbackParam;                                          /*!< cdc bulk in transfer callback parameter*/
    void *outCallbackParam;                                         /*!< cdc bulk out transfer callback parameter*/
    usb_host_transfer_t *controlTransfer;                           /*!< on-going control transfer*/
    transfer_callback_t controlCallbackFn;   /*!< cdc control transfer callback function pointer*/
    transfer_callback_t interruptCallbackFn; /*!< cdc interrupt transfer callback function pointer*/
    transfer_callback_t outCallbackFn;       /*!< cdc bulk out transfer callback function pointer*/
    transfer_callback_t inCallbackFn;        /*!< cdc bulk in transfer callback function pointer*/
    uint16_t packetSize;                     /*!< cdc control pipe max packet size*/
    uint16_t bulkOutPacketSize;              /*!< cdc bulk out max packet size*/
    uint16_t bulkInPacketSize;               /*!< cdc bulk in max packet size*/
} usb_host_cdc_instance_struct_t;

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @name USB CDC host class driver
 * @{
 */

/*!
 * @brief initialize the cdc instance.
 *
 * This function allocate the resource for cdc instance.
 *
 * @param deviceHandle       the device handle.
 * @param classHandle return class handle.
 *
 * @retval kStatus_USB_Success        The device is initialized successfully.
 * @retval kStatus_USB_AllocFail      Allocate memory fail.
 */
extern usb_status_t USB_HostCdcInit(usb_device_handle deviceHandle, usb_host_class_handle *classHandle);

/*!
 * @brief cdc set data interface callback, open pipes.
 *
 * @param param      callback parameter.
 * @param transfer    callback transfer.
 * @param status      transfer status.
 */
extern usb_status_t USB_HostCdcSetDataInterface(usb_host_class_handle classHandle,
                                                usb_host_interface_handle interfaceHandle,
                                                uint8_t alternateSetting,
                                                transfer_callback_t callbackFn,
                                                void *callbackParam);

/*!
 * @brief cdc set control interface callback, open pipes.
 *
 * @param param      callback parameter.
 * @param transfer    callback transfer.
 * @param status      transfer status.
 */
extern usb_status_t USB_HostCdcSetControlInterface(usb_host_class_handle classHandle,
                                                   usb_host_interface_handle interfaceHandle,
                                                   uint8_t alternateSetting,
                                                   transfer_callback_t callbackFn,
                                                   void *callbackParam);

/*!
 * @brief de-initialize the cdc instance.
 *
 * This function release the resource for cdc instance.
 *
 * @param deviceHandle   the device handle.
 * @param classHandle the class handle.
 *
 * @retval kStatus_USB_Success        The device is de-initialized successfully.
 */
extern usb_status_t USB_HostCdcDeinit(usb_device_handle deviceHandle, usb_host_class_handle classHandle);

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
extern uint16_t USB_HostCdcGetPacketsize(usb_host_class_handle classHandle, uint8_t pipeType, uint8_t direction);

/*!
 * @brief receive data.
 *
 * This function implements cdc receiving data.
 *
 * @param classHandle   the class handle.
 * @param buffer         the buffer pointer.
 * @param bufferLength     the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        receive request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostRecv.
 */
extern usb_status_t USB_HostCdcDataRecv(usb_host_class_handle classHandle,
                                        uint8_t *buffer,
                                        uint32_t bufferLength,
                                        transfer_callback_t callbackFn,
                                        void *callbackParam);

/*!
 * @brief send data.
 *
 * This function implements cdc sending data.
 *
 * @param classHandle   the class handle.
 * @param buffer         the buffer pointer.
 * @param bufferLength     the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success          receive request successfully.
 * @retval kStatus_USB_InvalidHandle    The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy             There is no idle transfer.
 * @retval kStatus_USB_Error            pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSend.
 */
extern usb_status_t USB_HostCdcDataSend(usb_host_class_handle classHandle,
                                        uint8_t *buffer,
                                        uint32_t bufferLength,
                                        transfer_callback_t callbackFn,
                                        void *callbackParam);

/*!
 * @brief interrupt receive data.
 *
 * This function implements interrupt receiving data.
 *
 * @param classHandle    the class handle.
 * @param buffer          the buffer pointer.
 * @param bufferLength      the buffer length.
 * @param callbackFn     this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success         receive request successfully.
 * @retval kStatus_USB_InvalidHandle   The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy            There is no idle transfer.
 * @retval kStatus_USB_Error           pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostRecv.
 */
extern usb_status_t USB_HostCdcInterruptRecv(usb_host_class_handle classHandle,
                                             uint8_t *buffer,
                                             uint32_t bufferLength,
                                             transfer_callback_t callbackFn,
                                             void *callbackParam);

/*!
 * @brief cdc get line coding.
 *
 * This function implements cdc GetLineCoding request.refer to pstn spec.
 *
 * @param classHandle   the class handle.
 * @param buffer         the buffer pointer.
 * @param bufferLength     the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostCdcGetAcmLineCoding(usb_host_class_handle classHandle,
                                                usb_host_cdc_line_coding_struct_t *uartLineCoding,
                                                transfer_callback_t callbackFn,
                                                void *callbackParam);

/*!
 * @brief cdc setControlLineState.
 *
 * This function implements cdc etControlLineState request.refer to pstn spec.
 *
 * @param classHandle   the class handle.
 * @param buffer         the buffer pointer.
 * @param bufferLength     the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        request successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostCdcSetAcmCtrlState(
    usb_host_class_handle classHandle, uint8_t dtr, uint8_t rts, transfer_callback_t callbackFn, void *callbackParam);

/*!
 * @brief cdc get acm descriptor.
 *
 * This function is hunting for class specific acm decriptor in the configuration ,get the corresponding
 * descriptor .
 *
 * @param classHandle          the class handle.
 * @param headDesc             the head function descriptor pointer.
 * @param callManageDesc       the call management functional descriptor pointer.
 * @param abstractControlDesc  the abstract control management functional pointer.
 * @param unionInterfaceDesc   the union functional descriptor pointer.
 *
 * @retval kStatus_USB_Error          analyse descriptor error.
 */
extern usb_status_t USB_HostCdcGetAcmDescriptor(usb_host_class_handle classHandle,
                                                usb_host_cdc_head_function_desc_struct_t **headDesc,
                                                usb_host_cdc_call_manage_desc_struct_t **callManageDesc,
                                                usb_host_cdc_abstract_control_desc_struct_t **abstractControlDesc,
                                                usb_host_cdc_union_interface_desc_struct_t **unionInterfaceDesc);

/*!
 * @brief cdc send control transfer common code.
 *
 * @param classHandle   the class handle.
 * @param request_type   setup packet request type.
 * @param request        setup packet request value.
 * @param wvalue_l       setup packet wvalue low byte.
 * @param wvalue_h       setup packet wvalue high byte.
 * @param wlength        setup packet wlength value.
 * @param data           data buffer pointer
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @return An error code or kStatus_USB_Success.
 */
extern usb_status_t USB_HostCdcControl(usb_host_class_handle classHandle,
                                       uint8_t request_type,
                                       uint8_t request,
                                       uint8_t wvalue_l,
                                       uint8_t wvalue_h,
                                       uint16_t wlength,
                                       uint8_t *data,
                                       transfer_callback_t callbackFn,
                                       void *callbackParam);
/*@}*/

#ifdef __cplusplus
}
#endif
/*@}*/

#endif /*__USB_HOST_CDC_H__*/
