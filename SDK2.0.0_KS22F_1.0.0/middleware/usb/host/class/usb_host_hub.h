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

#ifndef _USB_HOST_HUB_H_
#define _USB_HSOT_HUB_H_

#if ((defined USB_HOST_CONFIG_HUB) && (USB_HOST_CONFIG_HUB))

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief USB host hub max port count */
#define USB_HOST_HUB_MAX_PORT (7U)

/*! @brief HUB class code */
#define USB_HOST_HUB_CLASS_CODE (9U)
/*! @brief HUB sub-class code */
#define USB_HOST_HUB_SUBCLASS_CODE_NONE (0U)

/* HUB and PORT status according to Table 11-17 in chapter 11.*/
/*! @brief Local Power Status Change: This field indicates that a change has occurred in the hub's Local Power Source */
#define C_HUB_LOCAL_POWER (0U)
/*! @brief Over-Current Change: This field indicates if a change has occurred in the Over-Current field*/
#define C_HUB_OVER_CURRENT (1U)
/*! @brief Current Connect Status: This field reflects whether or not a device is currently connected to this port*/
#define PORT_CONNECTION (0U)
/*! @brief Port Enabled/Disabled: Ports can be enabled by the USB System Software only. Ports
can be disabled by either a fault condition (disconnect event or other fault condition) or by the USB System
Software*/
#define PORT_ENABLE (1U)
/*! @brief Suspend: This field indicates whether or not the device on this port is suspended */
#define PORT_SUSPEND (2U)
/*! @brief this field indicate that the current drain on the port exceeds the specified maximum. */
#define PORT_OVER_CURRENT (3U)
/*! @brief This field is set when the host wishes to reset the attached device */
#define PORT_RESET (4U)
/*! @brief This field reflects a port's logical, power control state */
#define PORT_POWER (8U)
/*! @brief Low- Speed Device Attached: This is relevant only if a device is attached */
#define PORT_LOW_SPEED (9U)
/*! @brief High-speed Device Attached: This is relevant only if a device is attached */
#define PORT_HIGH_SPEED (10U)
/*! @brief Connect Status Change: Indicates a change has occurred in the port's Current Connect Status */
#define C_PORT_CONNECTION (16U)
/*! @brief Port Enable/Disable Change: This field is set to one when a port is disabled because of a Port_Error
 * condition */
#define C_PORT_ENABLE (17U)
/*! @brief Suspend Change: This field indicates a change in the host-visible suspend state of the attached device */
#define C_PORT_SUSPEND (18U)
/*! @brief Over-Current Indicator Change: This field applies only to hubs that report over-current conditions on a
 * per-port basis */
#define C_PORT_OVER_CURRENT (19U)
/*! @brief Reset Change: This field is set when reset processing on this port is complete */
#define C_PORT_RESET (20U)

/*! @brief get hub think time value */
#define USB_HOST_HUB_DESCRIPTOR_CHARACTERISTICS_THINK_TIME_MASK (0x60U)
/*! @brief get hub think time value */
#define USB_HOST_HUB_DESCRIPTOR_CHARACTERISTICS_THINK_TIME_SHIFT (5U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief hub descriptor structure */
typedef struct _usb_host_hub_descriptor
{
    uint8_t blength;                /*!< Number of bytes in this descriptor*/
    uint8_t bdescriptortype;        /*!< Descriptor Type*/
    uint8_t bnrports;               /*!< Number of downstream facing ports that this hub supports*/
    uint8_t whubcharacteristics[2]; /*!< hub characteristics please reference to Table 11-13 in usb2.0 spec*/
    uint8_t bpwron2pwrgood;   /*!< Time (in 2 ms intervals) from the time the power-on sequence begins on a port until
                                 power is good on that port.*/
    uint8_t bhubcontrcurrent; /*!< Maximum current requirements of the Hub Controller electronics in mA*/
    uint8_t deviceremovable;  /*!< Indicates if a port has a removable device attached*/
} usb_host_hub_descriptor_t;

/*! @brief hub port instance structure */
typedef struct _usb_host_hub_port_instance
{
    usb_device_handle deviceHandle; /*!< device handle*/
    uint8_t portStatus;             /*!< port running status*/
    uint8_t resetCount;             /*!< port reset time*/
    uint8_t speed;                  /*!< port's device speed*/
} usb_host_hub_port_instance_t;

/*! @brief hub instance structure */
typedef struct _usb_host_hub_instance
{
    struct _usb_host_hub_instance *next;       /*!< next hub instance*/
    usb_host_handle hostHandle;                /*!< host handle*/
    usb_device_handle deviceHandle;            /*!< device handle*/
    usb_host_interface_handle interfaceHandle; /*!< interface handle*/
    usb_host_pipe_handle controlPipe;          /*!< control pipe handle*/
    usb_host_pipe_handle interruptPipe;        /*!< hub interrupt in pipe handle*/
    usb_host_hub_port_instance_t *portList;    /*!< hub's port instance list*/
    usb_host_transfer_t *controlTransfer;      /*!< control transfer in progress*/
    transfer_callback_t inCallbackFn;          /*!< interrupt in callback*/
    void *inCallbackParam;                     /*!< interrupt in callback parameter*/
    transfer_callback_t controlCallbackFn;     /*!< control callback*/
    void *controlCallbackParam;                /*!< control callback parameter*/
                                               /* hub property */
    uint16_t totalThinktime;                   /*!< hub total think time*/
    uint8_t hubLevel;                          /*!< hub level, the root hub's level is 1*/

    /* hub app parameter */
    uint8_t hubDescriptor[7 + (USB_HOST_HUB_MAX_PORT >> 3) + 1]; /*!< hub descriptor buffer*/
    uint8_t hubBitmapBuffer[(USB_HOST_HUB_MAX_PORT >> 3) + 1];   /*!< hub receiving bitmap data buffer*/
    uint8_t hubStatusBuffer[4];                                  /*!< hub status buffer*/
    uint8_t portStatusBuffer[4];                                 /*!< port status buffer*/
    uint8_t hubStatus;                                           /*!< hub instance running status*/
    uint8_t portCount;                                           /*!< hub port count*/
    uint8_t portIndex;                                           /*!< record the index when processing ports in turn*/
    uint8_t portProcess;                                         /*!< the port that is processing*/
    uint8_t primeStatus;                                         /*!< data prime transfer status*/
    uint8_t invalid; /*!< 0/1, when invalid, cannot send transfer to the class*/
} usb_host_hub_instance_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief initialize the hub instance.
 *
 * This function allocate the resource for hub instance.
 *
 * @param deviceHandle  the device handle.
 * @param classHandle   return class handle.
 *
 * @retval kStatus_USB_Success        The device is initialized successfully.
 * @retval kStatus_USB_AllocFail      Allocate memory fail.
 */
extern usb_status_t USB_HostHubInit(usb_device_handle deviceHandle, usb_host_class_handle *classHandle);

/*!
 * @brief set interface.
 *
 * This function bind the interface with the hub instance.
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
 * @retval kStatus_USB_Error          Open pipe fail, please reference to USB_HostOpenPipe.
 *                                    Or send transfer fail, please reference to USB_HostSendSetup,
 */
extern usb_status_t USB_HostHubSetInterface(usb_host_class_handle classHandle,
                                            usb_host_interface_handle interfaceHandle,
                                            uint8_t alternateSetting,
                                            transfer_callback_t callbackFn,
                                            void *callbackParam);

/*!
 * @brief de-initialize the hub instance.
 *
 * This function release the resource for hub instance.
 *
 * @param deviceHandle   the device handle.
 * @param classHandle    the class handle.
 *
 * @retval kStatus_USB_Success        The device is de-initialized successfully.
 */
extern usb_status_t USB_HostHubDeinit(usb_device_handle deviceHandle, usb_host_class_handle classHandle);

/*!
 * @brief receive data.
 *
 * This function implements hub receiving data.
 *
 * @param classHandle   the class handle.
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
extern usb_status_t USB_HostHubInterruptRecv(usb_host_class_handle classHandle,
                                             uint8_t *buffer,
                                             uint16_t bufferLength,
                                             transfer_callback_t callbackFn,
                                             void *callbackParam);

/*!
 * @brief port reset setup.
 *
 * This function sends hub port reset transfer.
 *
 * @param classHandle   the class handle.
 * @param portNumber    port number.
 *
 * @retval kStatus_USB_Success        send successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHubSendPortReset(usb_host_class_handle classHandle, uint8_t portNumber);

/*!
 * @brief hub get descriptor.
 *
 * This function implements get hub descriptor specific request.
 *
 * @param classHandle   the class handle.
 * @param buffer        the buffer pointer.
 * @param bufferLength  the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        send successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHubGetDescriptor(usb_host_class_handle classHandle,
                                             uint8_t *buffer,
                                             uint16_t bufferLength,
                                             transfer_callback_t callbackFn,
                                             void *callbackParam);

/*!
 * @brief hub clear feature.
 *
 * This function implements clear hub feature specific request.
 *
 * @param classHandle   the class handle.
 * @param buffer        the buffer pointer.
 * @param bufferLength  the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        send successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHubClearFeature(usb_host_class_handle classHandle,
                                            uint8_t feature,
                                            transfer_callback_t callbackFn,
                                            void *callbackParam);

/*!
 * @brief hub get status.
 *
 * This function implements get hub status specific request.
 *
 * @param classHandle   the class handle.
 * @param buffer        the buffer pointer.
 * @param bufferLength  the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        send successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHubGetStatus(usb_host_class_handle classHandle,
                                         uint8_t *buffer,
                                         uint16_t bufferLength,
                                         transfer_callback_t callbackFn,
                                         void *callbackParam);

/*!
 * @brief hub set feature.
 *
 * This function implements set hub feature specific request.
 *
 * @param classHandle   the class handle.
 * @param buffer        the buffer pointer.
 * @param bufferLength  the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        send successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHubSetPortFeature(usb_host_class_handle classHandle,
                                              uint8_t portNumber,
                                              uint8_t feature,
                                              transfer_callback_t callbackFn,
                                              void *callbackParam);

/*!
 * @brief hub clear port feature.
 *
 * This function implements clear hub port feature specific request.
 *
 * @param classHandle   the class handle.
 * @param buffer        the buffer pointer.
 * @param bufferLength  the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        send successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHubClearPortFeature(usb_host_class_handle classHandle,
                                                uint8_t portNumber,
                                                uint8_t feature,
                                                transfer_callback_t callbackFn,
                                                void *callbackParam);

/*!
 * @brief hub port get status.
 *
 * This function implements get hub port status specific request.
 *
 * @param classHandle   the class handle.
 * @param buffer        the buffer pointer.
 * @param bufferLength  the buffer length.
 * @param callbackFn    this callback is called after this function completes.
 * @param callbackParam the first parameter in the callback function.
 *
 * @retval kStatus_USB_Success        send successfully.
 * @retval kStatus_USB_InvalidHandle  The classHandle is NULL pointer.
 * @retval kStatus_USB_Busy           There is no idle transfer.
 * @retval kStatus_USB_Error          pipe is not initialized.
 *                                    Or, send transfer fail, please reference to USB_HostSendSetup.
 */
extern usb_status_t USB_HostHubGetPortStatus(usb_host_class_handle classHandle,
                                             uint8_t portNumber,
                                             uint8_t *buffer,
                                             uint16_t bufferLength,
                                             transfer_callback_t callbackFn,
                                             void *callbackParam);

#ifdef __cplusplus
}
#endif

#endif /* USB_HOST_CONFIG_HUB */

#endif /* _USB_HSOT_HUB_H_ */
