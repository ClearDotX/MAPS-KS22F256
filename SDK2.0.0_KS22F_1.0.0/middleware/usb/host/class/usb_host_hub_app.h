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

#ifndef _USB_HOST_HUB_APP_H_
#define _USB_HOST_HUB_APP_H_

#if ((defined USB_HOST_CONFIG_HUB) && (USB_HOST_CONFIG_HUB))

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief hub reset times*/
#define USB_HOST_HUB_PORT_RESET_TIMES (1)

/*! @brief hub lock */
#define USB_HostHubLock() USB_OsaMutexLock(s_HubGlobal.hubMutex)
/*! @brief hub unlock */
#define USB_HostHubUnlock() USB_OsaMutexUnlock(s_HubGlobal.hubMutex)

/*! @brief hub application global structure */
typedef struct _usb_host_hub_global
{
    usb_host_handle hostHandle;          /*!< this hub list belong to this host*/
    usb_host_hub_instance_t *hubProcess; /*!< hub in processing*/
    usb_host_hub_instance_t *hubList;    /*!< host's hub list*/
    usb_osa_mutex_handle hubMutex;       /*!< hub mutex*/
} usb_host_hub_global_t;

/*! @brief hub application status */
typedef enum _usb_host_hub_app_status
{
    kHubRunIdle = 0,         /*!< idle */
    kHubRunInvalid,          /*!< invalid state */
    kHubRunWaitSetInterface, /*!< wait callback of set interface */
    kHubRunGetDescriptor7,   /*!< get 7 bytes hub descriptor */
    kHubRunGetDescriptor,    /*!< get all hub descriptor */
    kHubRunSetPortPower,     /*!< set hub's port power */
    kHubRunGetStatusDone,    /*!< hub status changed */
} usb_host_hub_app_status_t;

/*! @brief hub port application status */
typedef enum _usb_host_port_app_status
{
    kPortRunIdle = 0,             /*!< idle */
    kPortRunInvalid,              /*!< invalid state */
    kPortRunWaitPortChange,       /*!< wait port status change */
    kPortRunCheckCPortConnection, /*!< check C_PORT_CONNECTION */
    kPortRunGetPortConnection,    /*!< get port status data */
    kPortRunCheckPortConnection,  /*!< check PORT_CONNECTION   */
    kPortRunWaitPortResetDone,    /*!< wait port reset transfer done */
    kPortRunWaitCPortReset,       /*!< wait C_PORT_RESET */
    KPortRunCheckCPortReset,      /*!< check C_PORT_RESET */
    kPortRunResetAgain,           /*!< reset port again */
    kPortRunPortAttached,         /*!< device is attached on the port */
    kPortRunCheckPortDetach,      /*!< check port is detached */
    kPortRunGetConnectionBit,     /*!< get the port status data */
    kPortRunCheckConnectionBit,   /*!< check PORT_CONNECTION */
} usb_host_port_app_status_t;

/*! @brief hub data prime status */
typedef enum _usb_host_hub_prime_status
{
    kPrimeNone = 0,    /*!< don't prime data*/
    kPrimeHubControl,  /*!< prime hub control transfer*/
    kPrimePortControl, /*!< prime port control transfer*/
    kPrimeInterrupt,   /*!< prime interrupt transfer*/
} usb_host_hub_prime_status_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#endif /* USB_HOST_CONFIG_HUB */

#endif /* _USB_HOST_HUB_APP_H_ */
