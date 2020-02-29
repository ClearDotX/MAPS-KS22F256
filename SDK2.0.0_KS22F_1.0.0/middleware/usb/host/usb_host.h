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

#ifndef _USB_HOST_H_
#define _USB_HOST_H_

#include "usb.h"
#include "usb_misc.h"
#include "usb_spec.h"
#include "usb_host_framework.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines USB host stack version major*/
#define USB_HOST_STACK_VERSION_MAJOR (0x01U)
/*! @brief Defines USB host stack version minor*/
#define USB_HOST_STACK_VERSION_MINOR (0x00U)
/*! @brief Defines USB host stack version bugfix*/
#define USB_HOST_STACK_VERSION_BUGFIX (0x00U)

struct _usb_host_transfer; /* for cross reference */

/*!
 * @addtogroup usb_host_drv
 * @{
 */

/*! @brief USB host class handle type define */
typedef void *usb_host_class_handle;

/*! @brief USB host controller handle type define */
typedef void *usb_host_controller_handle;

/*! @brief USB host configuration handle type define */
typedef void *usb_host_configuration_handle;

/*! @brief USB host interface handle type define */
typedef void *usb_host_interface_handle;

/*! @brief USB host pipe handle type define */
typedef void *usb_host_pipe_handle;

/*! @brief event codes for device attach/detach */
typedef enum _usb_host_event
{
    kUSB_HostEventAttach = 1U,     /*!< device is attached */
    kUSB_HostEventDetach,          /*!< device is detached */
    kUSB_HostEventEnumerationDone, /*!< device's enumeration is done and the device is supported */
    kUSB_HostEventNotSupported     /*!< device's enumeration is done and the device is not supported */
} usb_host_event_t;

/*! @brief USB host device information code */
typedef enum _usb_host_dev_info
{
    kUSB_HostGetDeviceAddress = 1U, /*!< device's address */
    kUSB_HostGetDeviceHubNumber,    /*!< device's first hub address */
    kUSB_HostGetDevicePortNumber,   /*!< device's first hub port number */
    kUSB_HostGetDeviceSpeed,        /*!< device's speed */
    kUSB_HostGetDeviceHSHubNumber,  /*!< device's first high-speed hub address */
    kUSB_HostGetDeviceHSHubPort,    /*!< device's first high-speed hub number */
    kUSB_HostGetDeviceLevel,        /*!< device's hub level */
    kUSB_HostGetHostHandle,         /*!< device's host handle */
    kUSB_HostGetDeviceControlPipe,  /*!< device's control pipe handle */
    kUSB_HostGetDevicePID,          /*!< device's pid */
    kUSB_HostGetDeviceVID,          /*!< device's vid */
    kUSB_HostGetHubThinkTime        /*!< device's hub total think time */
} usb_host_dev_info_t;

/*!
 * @brief host callback function typedef.
 *
 * This callback function is used to notify application device attach/detach event.
 * This callback pointer is passed when initialize host.
 *
 * @param deviceHandle           The device handle, it means the attached device.
 * @param configurationHandle The configuration handle, it contains the attached device's configuration information.
 * @param event_code           The callback event code, please refer to the enumeration host_event_t.
 *
 * @return A USB error code or kStatus_USB_Success.
 * @retval kStatus_USB_Success       Application handles the attached device successfully.
 * @retval kStatus_USB_NotSupported  Application don't support the attached device.
 * @retval kStatus_USB_Error         Application handles the attached device falsely.
 */
typedef usb_status_t (*host_callback_t)(usb_device_handle deviceHandle,
                                        usb_host_configuration_handle configurationHandle,
                                        uint32_t eventCode);

/*!
 * @brief transfer callback function typedef.
 *
 * This callback function is used to notify up-layer the transfer result of a transfer.
 * This callback pointer is passed when calling the send/receive APIs.
 *
 * @param param     The parameter pointer, it is passed when calling the send/receive APIs.
 * @param data      The data buffer pointer.
 * @param data_len  The result data length.
 * @param status    A USB error code or kStatus_USB_Success.
 */
typedef void (*transfer_callback_t)(void *param, uint8_t *data, uint32_t dataLen, usb_status_t status);

/*!
 * @brief host stack inner transfer callback function typedef.
 *
 * This callback function is used to notify up-layer the transfer result of a transfer.
 * This callback pointer is passed when initialize structure usb_host_transfer_t.
 *
 * @param param     The parameter pointer, it is passed when calling the send/receive APIs.
 * @param transfer  The transfer information, please reference to structure usb_host_transfer_t.
 * @param status    A USB error code or kStatus_USB_Success.
 */
typedef void (*host_inner_transfer_callback_t)(void *param, struct _usb_host_transfer *transfer, usb_status_t status);

/*! @brief USB host endpoint information structure */
typedef struct _usb_host_ep
{
    usb_descriptor_endpoint_t *epDesc; /*!< endpoint descriptor pointer*/
    uint8_t *epExtension;              /*!< endpoint extended descriptor pointer*/
    uint16_t epExtensionLength;        /*!< extended descriptor length*/
} usb_host_ep_t;

/*! @brief USB host interface information structure */
typedef struct _usb_host_interface
{
    usb_host_ep_t epList[USB_HOST_CONFIG_INTERFACE_MAX_EP]; /*!< endpoint array*/
    usb_descriptor_interface_t *interfaceDesc;              /*!< interface descriptor pointer*/
    uint8_t *interfaceExtension;                            /*!< interface extended descriptor pointer*/
    uint16_t interfaceExtensionLength;                      /*!< extended descriptor length*/
    uint8_t interfaceIndex;                                 /*!< the interface index*/
    uint8_t alternateSettingNumber;                         /*!< the interface alternate setting value*/
    uint8_t epCount;                                        /*!< interface's endpoint number*/
} usb_host_interface_t;

/*! @brief USB host configuration information structure */
typedef struct _usb_host_configuration
{
    usb_host_interface_t interfaceList[USB_HOST_CONFIG_CONFIGURATION_MAX_INTERFACE]; /*!< interface array*/
    usb_descriptor_configuration_t *configurationDesc; /*!< configuration descriptor pointer*/
    uint8_t *configurationExtension;                   /*!< configuration extended descriptor pointer*/
    uint16_t configurationExtensionLength;             /*!< extended descriptor length*/
    uint8_t interfaceCount;                            /*!< the configuration's interface number*/
} usb_host_configuration_t;

/*! @brief USB host pipe common structure */
typedef struct _usb_host_pipe
{
    struct _usb_host_pipe *next;    /*!< link the idle pipes*/
    usb_device_handle deviceHandle; /*!< this pipe's device's handle*/
    uint16_t currentCount;          /*!< for khci transfer*/
    uint16_t nakCount;              /*!< max nak count*/
    uint16_t maxPacketSize;         /*!< max packet size*/
    uint16_t interval;              /*!< FS/LS: frame unit; HS: micro-frame unit*/
    uint8_t open;                   /*!< 0 - closed, 1 - open*/
    uint8_t nextdata01;             /*!< data toggle*/
    uint8_t endpointAddress;        /*!< endpoint address*/
    uint8_t direction;              /*!< pipe direction*/
    uint8_t pipeType;               /*!< pipe type, for example USB_ENDPOINT_BULK*/
    uint8_t numberPerUframe;        /*!< transaction number per micro-frame*/
} usb_host_pipe_t;

/*! @brief USB host transfer structure */
typedef struct _usb_host_transfer
{
    struct _usb_host_transfer *next; /*!< the next transfer structure*/
#if USB_HOST_CONFIG_EHCI
    uint32_t ehciUnitHead; /*!< qtd/itd/sitd head for this transfer*/
    uint32_t ehciUnitTail; /*!< qtd/itd/sitd tail for this transfer*/
#endif
    uint8_t *transferBuffer;                   /*!< transfer data buffer*/
    uint32_t transferLength;                   /*!< transfer data length*/
    uint32_t transferSofar;                    /*!< have transfered length*/
    host_inner_transfer_callback_t callbackFn; /*!< transfer callback function*/
    void *callbackParam;                       /*!< transfer callback parameter*/
    usb_host_pipe_t *transferPipe;             /*!< transfer pipe pointer*/
    usb_setup_struct_t setupPacket;            /*!< setup packet buffer*/
    uint8_t direction;                         /*!< transfer direction, it's values are USB_OUT or USB_IN*/
    uint8_t setupStatus;                       /*!< setup transfer status*/
#if USB_HOST_CONFIG_KHCI
    int32_t transferResult; /*!< khci transfer result */
    uint32_t frame;         /*!< khci transfer frame number */
    uint16_t nakTimeout;    /*!< khci transfer NAK timeout */
    uint16_t retry;         /*!< khci transfer retry */
#endif
} usb_host_transfer_t;

/*! @brief USB host pipe information structure for opening pipe */
typedef struct _usb_host_pipe_init
{
    void *devInstance;       /*!< device instance handle*/
    uint16_t nakCount;       /*!< Max NAK retry count. MUST be zero for interrupt*/
    uint16_t maxPacketSize;  /*!< pipe's max packet size*/
    uint8_t interval;        /*!< pipe's interval*/
    uint8_t endpointAddress; /*!< endpoint address*/
    uint8_t direction;       /*!< endpoint direction*/
    uint8_t pipeType;        /*!< endpoint type, the value is USB_ENDPOINT_INTERRUPT, USB_ENDPOINT_CONTROL,
                                USB_ENDPOINT_ISOCHRONOUS, USB_ENDPOINT_BULK*/
    uint8_t numberPerUframe; /*!< transaction number for each micro-frame*/
} usb_host_pipe_init_t;

/*! @brief cancel transfer parameter structure */
typedef struct _usb_host_cancel_param
{
    usb_host_pipe_handle pipeHandle; /*!< canceling pipe handle*/
    usb_host_transfer_t *transfer;   /*!< canceling transfer*/
} usb_host_cancel_param_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @name USB host APIs part1
 * The follow APIs are recommend to be used in application.
 * @{
 */

/*!
 * @brief Initialize the USB host stack.
 *
 * This function initializes the USB host module specified by the controllerId.
 *
 * @param[in] controllerId    The controller id of the USB IP. Please refer to the enumeration usb_controller_index_t.
 * @param[out] hostHandle     return the host handle.
 * @param[in] callbackFn      host callback function, it notify device attach/detach.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_InvalidHandle        The host_handle_ptr is a NULL pointer.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller according to the controller id.
 * @retval kStatus_USB_AllocFail            allocate memory fail.
 * @retval kStatus_USB_Error                host mutex create fail, KHCI/EHCI mutex or KHCI/EHCI event create fail.
 *                                          Or, KHCI/EHCI IP initialize fail.
 */
extern usb_status_t USB_HostInit(uint8_t controllerId, usb_host_handle *hostHandle, host_callback_t callbackFn);

/*!
 * @brief De-initialize the USB host stack.
 *
 * This function de-initialize the USB host module specified by the hostHandle.
 *
 * @param[in] hostHandle  the host handle.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_InvalidHandle        The hostHandle is a NULL pointer.
 * @retval kStatus_USB_Error                controller de-initialize fail.
 */
extern usb_status_t USB_HostDeinit(usb_host_handle hostHandle);

/*!
 * @brief get device information.
 *
 * This function get device information.
 *
 * @param[in] deviceHandle      the removing device handle.
 * @param[in] infoCode       please reference to enumeration host_dev_info_t.
 * @param[iout] infoValue    return the information value.
 *
 * @retval kStatus_USB_Success              close successfully.
 * @retval kStatus_USB_InvalidParameter     The deviceHandle or info_value is a NULL pointer.
 * @retval kStatus_USB_Error                The info_code is not host_dev_info_t value.
 */
extern usb_status_t USB_HostHelperGetPeripheralInformation(usb_device_handle deviceHandle,
                                                           uint32_t infoCode,
                                                           uint32_t *infoValue);

/*!
 * @brief parse alternate interface descriptor.
 *
 * This function parse the alternate interface descriptor and return interface information through structure
 * usb_host_interface_t.
 *
 * @param[in] interfaceHandle     the whole interface handle.
 * @param[in] alternateSetting    alternate setting value.
 * @param[out] interface          return interface information.
 *
 * @retval kStatus_USB_Success              close successfully.
 * @retval kStatus_USB_InvalidHandle        The interfaceHandle is a NULL pointer.
 * @retval kStatus_USB_InvalidParameter     The alternateSetting is 0.
 * @retval kStatus_USB_Error                The interface descriptor is wrong.
 */
extern usb_status_t USB_HostHelperParseAlternateSetting(usb_host_interface_handle interfaceHandle,
                                                        uint8_t alternateSetting,
                                                        usb_host_interface_t *interface);

/*!
 * @brief remove attached device.
 *
 * This function remove the attached device.
 * This function should not be used in normal situation, user need to be clear about the function when calling it.
 *
 * @param[in] hostHandle     the host handle.
 * @param[in] deviceHandle   the removing device handle.
 *
 * @retval kStatus_USB_Success              remove successfully.
 * @retval kStatus_USB_InvalidHandle        The hostHandle or deviceHandle is a NULL pointer.
 * @retval kStatus_USB_InvalidParameter     The deviceHandle instance don't belong to hostHandle instance.
 */
extern usb_status_t USB_HostRemoveDevice(usb_host_handle hostHandle, usb_device_handle deviceHandle);

/*!
 * @brief khci task function.
 *
 * The function is used to handle KHCI controller message.
 * In the BM environment, this function should be called periodically in the main function.
 * And in the RTOS environment, this function should be used as a function entry to create a task.
 *
 * @param[in] hostHandle The host handle.
 */
extern void USB_HostKhciTaskFunction(void *hostHandle);

/*!
 * @brief ehci task function.
 *
 * The function is used to handle EHCI controller message.
 * In the BM environment, this function should be called periodically in the main function.
 * And in the RTOS environment, this function should be used as a function entry to create a task.
 *
 * @param[in] hostHandle The host handle.
 */
extern void USB_HostEhciTaskFunction(void *hostHandle);

/*!
 * @brief Device KHCI isr function.
 *
 * The function is KHCI interrupt service routine.
 *
 * @param[in] hostHandle The host handle.
 */
extern void USB_HostKhciIsrFunction(void *hostHandle);

/*!
 * @brief Device EHCI isr function.
 *
 * The function is EHCI interrupt service routine.
 *
 * @param[in] hostHandle The host handle.
 */
extern void USB_HostEhciIsrFunction(void *hostHandle);
/*! @}*/

/*!
 * @name USB host APIs part2.
 * The follow APIs are not recommend to be used in application, they are mainly used in class driver.
 * @{
 */

/*!
 * @brief open USB host pipe.
 *
 * This function open one pipe according to the pipe_init_ptr parameter.
 *
 * @param[in] hostHandle  the host handle.
 * @param[out] pipeHandle the pipe handle pointer, it is used to return the pipe handle.
 * @param[in] pipeInit    it is used to initialize the pipe.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_InvalidHandle        The hostHandle or pipe_handle_ptr is a NULL pointer.
 * @retval kStatus_USB_Error                there is no idle pipe.
 *                                          Or, there is no idle QH for EHCI.
 *                                          Or, bandwidth allocate fail for EHCI.
 */
extern usb_status_t USB_HostOpenPipe(usb_host_handle hostHandle,
                                     usb_host_pipe_handle *pipeHandle,
                                     usb_host_pipe_init_t *pipeInit);

/*!
 * @brief close USB host pipe.
 *
 * This function close one pipe and release the related resources.
 *
 * @param[in] hostHandle     the host handle.
 * @param[in] pipeHandle     the closing pipe handle.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_InvalidHandle        The hostHandle or pipeHandle is a NULL pointer.
 */
extern usb_status_t USB_HostClosePipe(usb_host_handle hostHandle, usb_host_pipe_handle pipeHandle);

/*!
 * @brief send data to pipe.
 *
 * This function request to send the transfer to the specified pipe.
 *
 * @param[in] hostHandle     the host handle.
 * @param[in] pipeHandle     the sending pipe handle.
 * @param[in] transfer        the transfer information.
 *
 * @retval kStatus_USB_Success              send successfully.
 * @retval kStatus_USB_InvalidHandle        The hostHandle, pipeHandle or transfer is a NULL pointer.
 * @retval kStatus_USB_LackSwapBuffer       there is no swap buffer for KHCI.
 * @retval kStatus_USB_Error                There is no idle qtd/itd/sitd for EHCI.
 */
extern usb_status_t USB_HostSend(usb_host_handle hostHandle,
                                 usb_host_pipe_handle pipeHandle,
                                 usb_host_transfer_t *transfer);

/*!
 * @brief send setup transfer to pipe.
 *
 * This function request to send the setup transfer to the specified pipe.
 *
 * @param[in] hostHandle     the host handle.
 * @param[in] pipeHandle     the sending pipe handle.
 * @param[in] transfer        the transfer information.
 *
 * @retval kStatus_USB_Success              send successfully.
 * @retval kStatus_USB_InvalidHandle        The hostHandle, pipeHandle or transfer is a NULL pointer.
 * @retval kStatus_USB_LackSwapBuffer       there is no swap buffer for KHCI.
 * @retval kStatus_USB_Error                There is no idle qtd/itd/sitd for EHCI.
 */
extern usb_status_t USB_HostSendSetup(usb_host_handle hostHandle,
                                      usb_host_pipe_handle pipeHandle,
                                      usb_host_transfer_t *transfer);

/*!
 * @brief receive data from pipe.
 *
 * This function request to receive the transfer from the specified pipe.
 *
 * @param[in] hostHandle     the host handle.
 * @param[in] pipeHandle     the receiving pipe handle.
 * @param[in] transfer        the transfer information.
 *
 * @retval kStatus_USB_Success              receive successfully.
 * @retval kStatus_USB_InvalidHandle        The hostHandle, pipeHandle or transfer is a NULL pointer.
 * @retval kStatus_USB_LackSwapBuffer       there is no swap buffer for KHCI.
 * @retval kStatus_USB_Error                There is no idle qtd/itd/sitd for EHCI.
 */
extern usb_status_t USB_HostRecv(usb_host_handle hostHandle,
                                 usb_host_pipe_handle pipeHandle,
                                 usb_host_transfer_t *transfer);

/*!
 * @brief cancel pipe's transfers.
 *
 * This function cancel pipe's all transfers when the parameter transfer is NULL, or cancel the transfer.
 *
 * @param[in] hostHandle     the host handle.
 * @param[in] pipeHandle     the receiving pipe handle.
 * @param[in] transfer        the transfer information.
 *
 * @retval kStatus_USB_Success              cancel successfully.
 * @retval kStatus_USB_InvalidHandle        The hostHandle or pipeHandle is a NULL pointer.
 */
extern usb_status_t USB_HostCancelTransfer(usb_host_handle hostHandle,
                                           usb_host_pipe_handle pipeHandle,
                                           usb_host_transfer_t *transfer);

/*!
 * @brief allocate a transfer resource.
 *
 * This function allocate a transfer, this transfer is used to pass data information to low level stack.
 *
 * @param[in] hostHandle     the host handle.
 * @param[out] transfer       return the transfer.
 *
 * @retval kStatus_USB_Success              allocate successfully.
 * @retval kStatus_USB_InvalidHandle        The hostHandle or transfer is a NULL pointer.
 * @retval kStatus_USB_Error                there is no idle transfer.
 */
extern usb_status_t USB_HostMallocTransfer(usb_host_handle hostHandle, usb_host_transfer_t **transfer);

/*!
 * @brief free a transfer resource.
 *
 * This function free a transfer, this transfer is used to pass data information to low level stack.
 *
 * @param[in] hostHandle     the host handle.
 * @param[in] transfer        release the transfer.
 *
 * @retval kStatus_USB_Success              free successfully.
 * @retval kStatus_USB_InvalidHandle        The hostHandle or transfer is a NULL pointer.
 */
extern usb_status_t USB_HostFreeTransfer(usb_host_handle hostHandle, usb_host_transfer_t *transfer);

/*!
 * @brief request usb standard request.
 *
 * This function send the usb standard request packet.
 *
 * @param[in] deviceHandle    the device handle for control transfer.
 * @param[in] usbRequest      it is the usb standard request code, please reference to usb_spec.h.
 * @param[in] transfer        the used transfer.
 * @param[in] param           the param structure is different for different request, please reference to
 * usb_host_framework.h.
 *
 * @retval kStatus_USB_Success              send successfully.
 * @retval kStatus_USB_InvalidHandle        The deviceHandle is a NULL pointer.
 * @retval kStatus_USB_LackSwapBuffer       there is no swap buffer for KHCI.
 * @retval kStatus_USB_Error                There is no idle qtd/itd/sitd for EHCI,
 *                                          Or, the request is not standard request.
 */
extern usb_status_t USB_HostRequestControl(usb_device_handle deviceHandle,
                                           uint8_t usbRequest,
                                           usb_host_transfer_t *transfer,
                                           void *param);

/*!
 * @brief open interface.
 *
 * This function open interface. It is used to notify the host driver the interface is used by APP or class driver.
 *
 * @param[in] deviceHandle      the removing device handle.
 * @param[in] interfaceHandle     the opening interface handle.
 *
 * @retval kStatus_USB_Success              open successfully.
 * @retval kStatus_USB_InvalidHandle        The deviceHandle or interfaceHandle is a NULL pointer.
 */
extern usb_status_t USB_HostOpenDeviceInterface(usb_device_handle deviceHandle,
                                                usb_host_interface_handle interfaceHandle);

/*!
 * @brief close interface.
 *
 * This function open interface. It is used to notify the host driver the interface will not be used by APP or class
 * driver.
 *
 * @param[in] deviceHandle      the removing device handle.
 * @param[in] interfaceHandle     the opening interface handle.
 *
 * @retval kStatus_USB_Success              close successfully.
 * @retval kStatus_USB_InvalidHandle        The deviceHandle is a NULL pointer.
 */
extern usb_status_t USB_HostCloseDeviceInterface(usb_device_handle deviceHandle,
                                                 usb_host_interface_handle interfaceHandle);

/*!
 * @brief Get host stack version function.
 *
 * The function is used to get host stack version.
 *
 * @param[out] version The version structure pointer to keep the host stack version.
 *
 */
extern void USB_HostGetVersion(uint32_t *version);

/*! @}*/

#ifdef __cplusplus
}
#endif

/*! @}*/

#endif /* _USB_HOST_H_ */
