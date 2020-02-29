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

#ifndef __USB_DEVICE_H__
#define __USB_DEVICE_H__

/*!
 * @addtogroup usb_device_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines USB device stack version major*/
#define USB_DEVICE_STACK_VERSION_MAJOR (0x01U)
/*! @brief Defines USB device stack version minor*/
#define USB_DEVICE_STACK_VERSION_MINOR (0x00U)
/*! @brief Defines USB device stack version bugfix*/
#define USB_DEVICE_STACK_VERSION_BUGFIX (0x00U)

/*! @brief Defines Get/Set status Types */
typedef enum _usb_device_status
{
    kUSB_DeviceStatusTestMode = 1U, /*!< Test mode */
    kUSB_DeviceStatusSpeed,         /*!< Current speed */
    kUSB_DeviceStatusOtg,           /*!< OTG status */
    kUSB_DeviceStatusDevice,        /*!< Device status */
    kUSB_DeviceStatusEndpoint,      /*!< Endpoint state usb_device_endpoint_status_t */
    kUSB_DeviceStatusDeviceState,   /*!< Device state */
    kUSB_DeviceStatusAddress,       /*!< Device address */
    kUSB_DeviceStatusSynchFrame,    /*!< Current frame */
    kUSB_DeviceStatusBus,           /*!< Bus status */
} usb_device_status_t;

/*! @brief Defines USB 2.0 device state */
typedef enum _usb_device_state
{
    kUSB_DeviceStateConfigured = 0U, /*!< Device state, Configured*/
    kUSB_DeviceStateAddress,         /*!< Device state, Address*/
    kUSB_DeviceStateDefault,         /*!< Device state, Default*/
    kUSB_DeviceStateAddressing,      /*!< Device state, Address setting*/
    kUSB_DeviceStateTestMode,        /*!< Device state, Test mode*/
} usb_device_state_t;

/*! @brief Defines endpoint state */
typedef enum _usb_endpoint_status
{
    kUSB_DeviceEndpointStateIdle = 0U, /*!< Endpoint state, idle*/
    kUSB_DeviceEndpointStateStalled,   /*!< Endpoint state, stalled*/
} usb_device_endpoint_status_t;

/*! @brief Control endpoint index */
#define USB_CONTROL_ENDPOINT (0U)
/*! @brief Control endpoint maxPacketSize */
#define USB_CONTROL_MAX_PACKET_SIZE (64U)

#if (USB_DEVICE_CONFIG_EHCI && (USB_CONTROL_MAX_PACKET_SIZE != (64U)))
#error For high speed, USB_CONTROL_MAX_PACKET_SIZE must be 64!!!
#endif

/*! @brief The setup packet size of USB control transfer. */
#define USB_SETUP_PACKET_SIZE (8U)
/*! @brief  USB endpoint mask */
#define USB_ENDPOINT_NUMBER_MASK (0x0FU)

/*! @brief Default invalid value or the endpoint callback length of canceled transfer */
#define USB_UNINITIALIZED_VAL_32 (0xFFFFFFFFU)

/*! @brief Available common EVENT types in device callback */
typedef enum _usb_device_event
{
    kUSB_DeviceEventBusReset = 1U, /*!< USB bus reset signal detected */
    kUSB_DeviceEventSuspend,       /*!< USB bus suspend signal detected */
    kUSB_DeviceEventResume, /*!< USB bus resume signal detected. The resume signal is drived by itself or a host */
    kUSB_DeviceEventError,  /*!< An error is happened in the bus. */
    kUSB_DeviceEventDetach, /*!< USB device is disconnected from a host. */
    kUSB_DeviceEventAttach, /*!< USB device is connected to a host. */
    kUSB_DeviceEventSetConfiguration, /*!< Set configuration. */
    kUSB_DeviceEventSetInterface,     /*!< Set interface. */

    kUSB_DeviceEventGetDeviceDescriptor,        /*!<Get device descriptor. */
    kUSB_DeviceEventGetConfigurationDescriptor, /*!< Get configuration descriptor. */
    kUSB_DeviceEventGetStringDescriptor,        /*!< Get string descriptor. */
    kUSB_DeviceEventGetHidDescriptor,           /*!< Get hid descriptor. */
    kUSB_DeviceEventGetHidReportDescriptor,     /*!< Get hid report descriptor. */
    kUSB_DeviceEventGetHidPhysicalDescriptor,   /*!< Get hid physical descriptor. */
    kUSB_DeviceEventVendorRequest,              /*!< Vendor request. */
    kUSB_DeviceEventSetRemoteWakeup,            /*!< Enable or disable remote wakeup function. */
    kUSB_DeviceEventGetConfiguration,           /*!< Get current configuration index */
    kUSB_DeviceEventGetInterface,               /*!< Get current interface alternate setting value */
} usb_device_event_t;

/*! @brief Endpoint callback message structure */
typedef struct _usb_device_endpoint_callback_message_struct
{
    uint8_t *buffer; /*!< transferred buffer */
    uint32_t length; /*!< transferred data length */
    uint8_t isSetup; /*!< is in setup phase */
} usb_device_endpoint_callback_message_struct_t;

/*!
 * @brief Endpoint callback function typedef.
 *
 * This callback function is used to notify uplayer the tranfser result of a transfer.
 * This callback pointer is passed when a sepcified endpoint initialied by calling API #USB_DeviceInitEndpoint.
 *
 * @param handle          The device handle. It equals the value returned from #USB_DeviceInit.
 * @param message         The result of a transfer, includes transfer buffer, transfer length and whether is in setup
 * phase for control pipe.
 * @param callbackParam  The paramter for this callback. It is same with
 * usb_device_endpoint_callback_struct_t::callbackParam.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
typedef usb_status_t (*usb_device_endpoint_callback_t)(usb_device_handle handle,
                                                       usb_device_endpoint_callback_message_struct_t *message,
                                                       void *callbackParam);

/*!
 * @brief Device callback function typedef.
 *
 * This callback function is used to notify uplayer the device status changed.
 * This callback pointer is passed by calling API #USB_DeviceInit.
 *
 * @param handle          The device handle. It equals the value returned from #USB_DeviceInit.
 * @param callbackEvent  The callback event type. please refer to the enumeration #usb_device_event_t.
 * @param eventParam     The event paramter for this callback. The parameter type is determined by the callback event.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
typedef usb_status_t (*usb_device_callback_t)(usb_device_handle handle, uint32_t callbackEvent, void *eventParam);

/*! @brief Endpoint callback structure */
typedef struct _usb_device_endpoint_callback_struct
{
    usb_device_endpoint_callback_t callbackFn; /*!< endpoint callback function*/
    void *callbackParam;                       /*!< parameter for callback function*/
} usb_device_endpoint_callback_struct_t;

/*! @brief Endpoint initialization structure */
typedef struct _usb_device_endpoint_init_struct
{
    uint16_t maxPacketSize;  /*!< endpoint max packet size */
    uint8_t endpointAddress; /*!< endpoint address*/
    uint8_t transferType;    /*!< endpoint transfer type*/
    uint8_t zlt;             /*!< zlt flag*/
} usb_device_endpoint_init_struct_t;

/*! @brief Endpoint status structure */
typedef struct _usb_device_endpoint_status_struct
{
    uint8_t endpointAddress; /*!< Endpoint address */
    uint16_t endpointStatus; /*!< Endpoint status : idle or stalled */
} usb_device_endpoint_status_struct_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @name USB device APIs
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Initialize the USB device stack.
 *
 * This function initizlizes the USB device module specified by the controllerId.
 *
 * @param[in] controllerId   The controller id of the USB IP. Please refer to the enumeration #usb_controller_index_t.
 * @param[in] deviceCallback Fucntion pointer of the device callback.
 * @param[out] handle          It is an out parameter, is used to return pointer of the device handle to the caller.
 *
 * @retval kStatus_USB_Success              The device is initialized successfully.
 * @retval kStatus_USB_InvalidHandle        The handle is a NULL pointer.
 * @retval kStatus_USB_Busy                 Cannot allocate a device handle.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller according to the controller id.
 * @retval kStatus_USB_InvalidControllerInterface  The controller driver interfaces is invaild, There is an empty
 * interface entity.
 * @retval kStatus_USB_Error                The macro USB_DEVICE_CONFIG_ENDPOINTS is more than IP's endpoint number.
 *                                          Or, the device has been initialized.
 *                                          Or, the mutex or message queue is created failed.
 */
extern usb_status_t USB_DeviceInit(uint8_t controllerId,
                                   usb_device_callback_t deviceCallback,
                                   usb_device_handle *handle);

/*!
 * @brief Enable the device functionality.
 *
 * The function enables the device functionality, so that the device can be recognized by the host when the device
 * detects that it has been connected to a host.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 *
 * @retval kStatus_USB_Success              The device is run successfully.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller.
 * @retval kStatus_USB_InvalidHandle        The device handle is a NULL pointer. Or the controller handle is invalid.
 *
 */
extern usb_status_t USB_DeviceRun(usb_device_handle handle);

/*!
 * @brief Disable the device functionality.
 *
 * The function disables the device functionality, after this function called, even the device is detached to the host,
 * and the device can't work.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 *
 * @retval kStatus_USB_Success              The device is stopped successfully.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller.
 * @retval kStatus_USB_InvalidHandle        The device handle is a NULL pointer. Or the controller handle is invalid.
 */
extern usb_status_t USB_DeviceStop(usb_device_handle handle);

/*!
 * @brief De-initialize the device controller.
 *
 * The function de-initializes the device controller specified by the handle.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 *
 * @retval kStatus_USB_Success              The device is stopped successfully.
 * @retval kStatus_USB_InvalidHandle        The device handle is a NULL pointer. Or the controller handle is invalid.
 */
extern usb_status_t USB_DeviceDeinit(usb_device_handle handle);

/*!
 * @brief Send data through a specified endpoint.
 *
 * The function is used to send data through a specified endpoint.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 * @param[in] endpointAddress Endpoint index.
 * @param[in] buffer The memory address to hold the data need to be sent.
 * @param[in] length The data length need to be sent.
 *
 * @retval kStatus_USB_Success              The send request is sent successfully.
 * @retval kStatus_USB_InvalidHandle        The handle is a NULL pointer. Or the controller handle is invalid.
 * @retval kStatus_USB_Busy                 Cannot allocate dtds for current tansfer in EHCI driver.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller.
 * @retval kStatus_USB_Error                The device is doing reset.
 *
 * @note The return value just means if the sending request is successful or not; the transfer done is notified by the
 * corresponding callback function.
 * Currently, only one transfer request can be supported for one specific endpoint.
 * If there is a specific requirement to support multiple transfer requests for one specific endpoint, the application
 * should implement a queue in the application level.
 * The subsequent transfer could begin only when the previous transfer is done (get notification through the endpoint
 * callback).
 */
extern usb_status_t USB_DeviceSendRequest(usb_device_handle handle,
                                          uint8_t endpointAddress,
                                          uint8_t *buffer,
                                          uint32_t length);

/*!
 * @brief Receive data through a specified endpoint.
 *
 * The function is used to receive data through a specified endpoint.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 * @param[in] endpointAddress Endpoint index.
 * @param[in] buffer The memory address to save the received data.
 * @param[in] length The data length want to be received.
 *
 * @retval kStatus_USB_Success              The receive request is sent successfully.
 * @retval kStatus_USB_InvalidHandle        The handle is a NULL pointer. Or the controller handle is invalid.
 * @retval kStatus_USB_Busy                 Cannot allocate dtds for current tansfer in EHCI driver.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller.
 * @retval kStatus_USB_Error                The device is doing reset.
 *
 * @note The return value just means if the receiving request is successful or not; the transfer done is notified by the
 * corresponding callback function.
 * Currently, only one transfer request can be supported for one specific endpoint.
 * If there is a specific requirement to support multiple transfer requests for one specific endpoint, the application
 * should implement a queue in the application level.
 * The subsequent transfer could begin only when the previous transfer is done (get notification through the endpoint
 * callback).
 */
extern usb_status_t USB_DeviceRecvRequest(usb_device_handle handle,
                                          uint8_t endpointAddress,
                                          uint8_t *buffer,
                                          uint32_t length);

/*!
 * @brief Cancel the pending transfer in a specified endpoint.
 *
 * The function is used to cancel the pending transfer in a specified endpoint.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 * @param[in] endpointAddress Endpoint address, bit7 is the direction of endpoint, 1U - IN, abd 0U - OUT.
 *
 * @retval kStatus_USB_Success              The transfer is cancelled.
 * @retval kStatus_USB_InvalidHandle        The handle is a NULL pointer. Or the controller handle is invalid.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller.
 */
extern usb_status_t USB_DeviceCancel(usb_device_handle handle, uint8_t endpointAddress);

/*!
 * @brief Initialize a specified endpoint.
 *
 * The function is used to initialize a specified endpoint and the corresponding endpoint callback is also initialized.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 * @param[in] epInit Endpoint initizlization structure. Please refer to the structure usb_device_endpoint_init_struct_t.
 * @param[in] endpointCallback Endpoint callback structure. Please refer to the structure
 * usb_device_endpoint_callback_struct_t.
 *
 * @retval kStatus_USB_Success              The endpoint is initialized successfully.
 * @retval kStatus_USB_InvalidHandle        The handle is a NULL pointer. Or the controller handle is invalid.
 * @retval kStatus_USB_InvalidParameter     The epInit or endpointCallback is NULL pointer. Or the endpoint number is
 * more than USB_DEVICE_CONFIG_ENDPOINTS.
 * @retval kStatus_USB_Busy                 The endpoint is busy in EHCI driver.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller.
 */
extern usb_status_t USB_DeviceInitEndpoint(usb_device_handle handle,
                                           usb_device_endpoint_init_struct_t *epInit,
                                           usb_device_endpoint_callback_struct_t *endpointCallback);

/*!
 * @brief De-initizlize a specified endpoint.
 *
 * The function is used to de-initizlize a specified endpoint.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 * @param[in] endpointAddress Endpoint address, bit7 is the direction of endpoint, 1U - IN, abd 0U - OUT.
 *
 * @retval kStatus_USB_Success              The endpoint is de-initialized successfully.
 * @retval kStatus_USB_InvalidHandle        The handle is a NULL pointer. Or the controller handle is invalid.
 * @retval kStatus_USB_InvalidParameter     The endpoint number is more than USB_DEVICE_CONFIG_ENDPOINTS.
 * @retval kStatus_USB_Busy                 The endpoint is busy in EHCI driver.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller.
 */
extern usb_status_t USB_DeviceDeinitEndpoint(usb_device_handle handle, uint8_t endpointAddress);

/*!
 * @brief Stall a specified endpoint.
 *
 * The function is used to stall a specified endpoint.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 * @param[in] endpointAddress Endpoint address, bit7 is the direction of endpoint, 1U - IN, abd 0U - OUT.
 *
 * @retval kStatus_USB_Success              The endpoint is stalled successfully.
 * @retval kStatus_USB_InvalidHandle        The handle is a NULL pointer. Or the controller handle is invalid.
 * @retval kStatus_USB_InvalidParameter     The endpoint number is more than USB_DEVICE_CONFIG_ENDPOINTS.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller.
 */
extern usb_status_t USB_DeviceStallEndpoint(usb_device_handle handle, uint8_t endpointAddress);

/*!
 * @brief Un-stall a specified endpoint.
 *
 * The function is used to un-stall a specified endpoint.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 * @param[in] endpointAddress Endpoint address, bit7 is the direction of endpoint, 1U - IN, abd 0U - OUT.
 *
 * @retval kStatus_USB_Success              The endpoint is un-stalled successfully.
 * @retval kStatus_USB_InvalidHandle        The handle is a NULL pointer. Or the controller handle is invalid.
 * @retval kStatus_USB_InvalidParameter     The endpoint number is more than USB_DEVICE_CONFIG_ENDPOINTS.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller.
 */
extern usb_status_t USB_DeviceUnstallEndpoint(usb_device_handle handle, uint8_t endpointAddress);

/*!
 * @brief Get the status of the selected item.
 *
 * The function is used to get the status of the selected item.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 * @param[in] type   The selected item. Please refer to the structure #usb_device_status_t.
 * @param[out] param  The param type is determined by the selected item.
 *
 * @retval kStatus_USB_Success              Get status successfully.
 * @retval kStatus_USB_InvalidHandle        The handle is a NULL pointer. Or the controller handle is invalid.
 * @retval kStatus_USB_InvalidParameter     The param is NULL pointer.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller.
 * @retval kStatus_USB_Error                Unsupported type.
 */
extern usb_status_t USB_DeviceGetStatus(usb_device_handle handle, usb_device_status_t type, void *param);

/*!
 * @brief Set the status of the selected item.
 *
 * The function is used to set the status of the selected item.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 * @param[in] type The selected item. Please refer to the structure #usb_device_status_t.
 * @param[in] param The param type is determined by the selected item.
 *
 * @retval kStatus_USB_Success              Set status successfully.
 * @retval kStatus_USB_InvalidHandle        The handle is a NULL pointer. Or the controller handle is invalid.
 * @retval kStatus_USB_ControllerNotFound   Cannot find the controller.
 * @retval kStatus_USB_Error                Unsupported type, or the param is NULL pointer.
 */
extern usb_status_t USB_DeviceSetStatus(usb_device_handle handle, usb_device_status_t type, void *param);

/*!
 * @brief Device task function.
 *
 * The function is used to handle controller message.
 * This function should not be called in applicartion directly.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 */
extern void USB_DeviceTaskFunction(void *deviceHandle);

#if ((defined(USB_DEVICE_CONFIG_KHCI)) && (USB_DEVICE_CONFIG_KHCI > 0U))
/*!
 * @brief Device KHCI task function.
 *
 * The function is used to handle KHCI controller message.
 * In the BM environment, this function should be called periodically in the main function.
 * And in the RTOS environment, this function should be used as a function entry to create a task.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 */
#define USB_DeviceKhciTaskFunction(deviceHandle) USB_DeviceTaskFunction(deviceHandle)
#endif

#if ((defined(USB_DEVICE_CONFIG_EHCI)) && (USB_DEVICE_CONFIG_EHCI > 0U))
/*!
 * @brief Device EHCI task function.
 *
 * The function is used to handle EHCI controller message.
 * In the BM environment, this function should be called periodically in the main function.
 * And in the RTOS environment, this function should be used as a function entry to create a task.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 */
#define USB_DeviceEhciTaskFunction(deviceHandle) USB_DeviceTaskFunction(deviceHandle)
#endif

#if ((defined(USB_DEVICE_CONFIG_KHCI)) && (USB_DEVICE_CONFIG_KHCI > 0U))
/*!
 * @brief Device KHCI isr function.
 *
 * The function is KHCI interrupt service routine.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 */
extern void USB_DeviceKhciIsrFunction(void *deviceHandle);
#endif

#if ((defined(USB_DEVICE_CONFIG_EHCI)) && (USB_DEVICE_CONFIG_EHCI > 0U))
/*!
 * @brief Device EHCI isr function.
 *
 * The function is EHCI interrupt service routine.
 *
 * @param[in] handle The device handle got from #USB_DeviceInit.
 */
extern void USB_DeviceEhciIsrFunction(void *deviceHandle);
#endif

/*!
 * @brief Get dvice stack version function.
 *
 * The function is used to get dvice stack version.
 *
 * @param[out] version The version structure pointer to keep the device stack version.
 *
 */
extern void USB_DeviceGetVersion(uint32_t *version);

/*! @}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* __USB_DEVICE_H__ */
