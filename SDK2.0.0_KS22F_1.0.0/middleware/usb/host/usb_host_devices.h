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

#ifndef _USB_HOST_DEV_MNG_H_
#define _USB_HOST_DEV_MNG_H_

#include "usb_host.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief States of device instances enumeration */
typedef enum _usb_host_device_status
{
    kStatus_DEV_Notinit = 0, /*!< device is invalid */
    kStatus_DEV_Initial,     /*!< device has been processed by host driver */
    kStatus_DEV_GetDes8,     /*!< enumeration process: get 8 bytes' device descriptor */
    kStatus_DEV_SetAddress,  /*!< enumeration process: set device address */
    kStatus_DEV_GetDes,      /*!< enumeration process: get device descriptor */
    kStatus_DEV_GetCfg9,     /*!< enumeration process: get 9 bytes' configuration descriptor */
    kStatus_DEV_GetCfg,      /*!< enumeration process: get configuration descriptor */
    kStatus_DEV_SetCfg,      /*!< enumeration process: set configuration */
    kStatus_DEV_EnumDone,    /*!< enumeration is done */
    kStatus_DEV_AppUsed,     /*!< this device has been used by application */
} usb_host_device_status_t;

/*! @brief States of device's interface */
typedef enum _usb_host_interface_state
{
    kStatus_interface_Attached = 1, /*!< interface's default status */
    kStatus_interface_Opened,       /*!< interface is used by application */
    kStatus_interface_Detached,     /*!< interface is not used by application */
} usb_host_interface_state_t;

/*! @brief device instance */
typedef struct _usb_host_device_instance
{
    struct _usb_host_device_instance *next;   /*!< next device, or NULL */
    usb_host_handle hostHandle;               /*!< host handle */
    usb_host_configuration_t configuration;   /*!< parsed configuration information for the device */
    usb_descriptor_device_t deviceDescriptor; /*!< standard device descriptor */
    usb_host_pipe_handle controlPipe;         /*!< device's control pipe */
    uint8_t *configurationDesc;               /*!< configuration descriptor pointer */
    uint16_t configurationLen;                /*!< configuration descriptor length */
    uint16_t configurationValue;              /*!< configuration index */
    uint8_t interfaceStatus[USB_HOST_CONFIG_CONFIGURATION_MAX_INTERFACE]; /*!< interfaces' status, please reference to
                                                                             #usb_host_interface_state_t */
    uint8_t enumBuffer[9];                                                /*!< buffer for enumeration */
    uint8_t state;                                                        /*!< device state for enumeration */
    uint8_t enumRetries;       /*!< re-enumeration when error in control transfer */
    uint8_t stallRetries;      /*!< re-transfer when stall */
    uint8_t speed;             /*!< device speed */
    uint8_t allocatedAddress;  /*!< temporary address for the device. when set address request succed setAddress will be
                                  the value, 1 - 127 */
    uint8_t setAddress;        /*!< the address has been set to the device successfully, 1 - 127 */
    uint8_t deviceAttachState; /*!< 0 - the device is detached; 1 - the device is attached */
#if ((defined USB_HOST_CONFIG_HUB) && (USB_HOST_CONFIG_HUB))
    /* hub related */
    uint8_t hubNumber;   /*!< device's first connected hub address (root hub = 0) */
    uint8_t portNumber;  /*!< device's first connected hub's port no (1 - 8) */
    uint8_t hsHubNumber; /*!< device's first connected high-speed hub's address (1 - 8) */
    uint8_t hsHubPort;   /*!< device's first connected high-speed hub's port no (1 - 8) */
    uint8_t level;       /*!< device's level (root device = 0) */
#endif
} usb_host_device_instance_t;

typedef struct _usb_host_enum_process_entry
{
    uint8_t successState; /*!< when the last step is successful, the next state value */
    uint8_t retryState;   /*!< when the last step need retry, the next state value */
    usb_status_t (*process)(usb_host_device_instance_t *deviceInstance); /*!< when the last step transfer is done, the
                                                                            function is used to process the transfer
                                                                            data */
} usb_host_enum_process_entry_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief call this function when device attach.
 *
 * @param hostHandle    host instance handle.
 * @param speed         device speed.
 * @param hubNumber     device hub no, root device's hub no is 0.
 * @param portNumber    device port no, root device's port no is 0.
 * @param level         device level, root device's level is 1.
 * @param deviceHandle  return device handle.
 *
 * @return kStatus_USB_Success or error codes.
 */
extern usb_status_t USB_HostAttachDevice(usb_host_handle hostHandle,
                                         uint8_t speed,
                                         uint8_t hubNumber,
                                         uint8_t portNumber,
                                         uint8_t level,
                                         usb_device_handle *deviceHandle);

/*!
 * @brief call this function when device detach.
 *
 * @param hostHandle   host instance handle.
 * @param hubNumber    device hub no, root device's hub no is 0.
 * @param portNumber   device port no, root device's port no is 0.
 *
 * @return kStatus_USB_Success or error codes.
 */
extern usb_status_t USB_HostDetachDevice(usb_host_handle hostHandle, uint8_t hubNumber, uint8_t portNumber);

/*!
 * @brief call this function when device detach.
 *
 * @param hostHandle    host instance handle.
 * @param deviceHandle  device handle.
 *
 * @return kStatus_USB_Success or error codes.
 */
extern usb_status_t USB_HostDetachDeviceInternal(usb_host_handle hostHandle, usb_device_handle deviceHandle);

/*!
 * @brief get the device attach/detach state.
 *
 * @param deviceHandle    device handle.
 *
 * @return 0x01 - attached; 0x00 - detached.
 */
extern uint8_t USB_HostGetDeviceAttachState(usb_device_handle deviceHandle);

/*!
 * @brief judge whether the device is attached.
 *
 * @param hostInstance    host instance pointer.
 * @param deviceHandle    device handle.
 *
 * @return kStatus_USB_Success or error codes.
 */
extern usb_status_t USB_HostValidateDevice(usb_host_handle hostHandle, usb_device_handle deviceHandle);

#endif /* _USB_HOST_DEV_MNG_H_ */
