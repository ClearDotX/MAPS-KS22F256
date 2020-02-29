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

#ifndef __USB_DEVICE_KHCI_H__
#define __USB_DEVICE_KHCI_H__

/*!
 * @addtogroup usb_device_controller_khci_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief The max value of ISO max packet size for FS in USB specification 2.0 */
#define USB_DEVICE_MAX_FS_ISO_MAX_PACKET_SIZE (1023U)

/*! @brief The max value of non-ISO max packet size for FS in USB specification 2.0 */
#define USB_DEVICE_MAX_FS_NONE_ISO_MAX_PACKET_SIZE (64U)

/*! @brief Set BDT buffer address */
#define USB_KHCI_BDT_SET_ADDRESS(bdt_base, ep, direction, odd, address)                          \
    *((volatile uint32_t *)((bdt_base & 0xfffffe00U) | (((uint32_t)ep & 0x0fU) << 5U) |          \
                            (((uint32_t)direction & 1U) << 4U) | (((uint32_t)odd & 1U) << 3U)) + \
      1U) = address

/*! @brief Set BDT control fields*/
#define USB_KHCI_BDT_SET_CONTROL(bdt_base, ep, direction, odd, control)                \
    *(volatile uint32_t *)((bdt_base & 0xfffffe00U) | (((uint32_t)ep & 0x0fU) << 5U) | \
                           (((uint32_t)direction & 1U) << 4U) | (((uint32_t)odd & 1U) << 3U)) = control

/*! @brief Get BDT buffer address*/
#define USB_KHCI_BDT_GET_ADDRESS(bdt_base, ep, direction, odd)                                    \
    (*((volatile uint32_t *)((bdt_base & 0xfffffe00U) | (((uint32_t)ep & 0x0fU) << 5U) |          \
                             (((uint32_t)direction & 1U) << 4U) | (((uint32_t)odd & 1U) << 3U)) + \
       1U))

/*! @brief Get BDT control fields*/
#define USB_KHCI_BDT_GET_CONTROL(bdt_base, ep, direction, odd)                          \
    (*(volatile uint32_t *)((bdt_base & 0xfffffe00U) | (((uint32_t)ep & 0x0fU) << 5U) | \
                            (((uint32_t)direction & 1U) << 4U) | (((uint32_t)odd & 1U) << 3U)))

/*! @brief Endpoint state structure */
typedef struct _usb_device_khci_endpoint_state_struct
{
    uint8_t *transferBuffer; /*!< Address of buffer containing the data to be transmitted */
    uint32_t transferLength; /*!< Length of data to transmit. */
    uint32_t transferDone;   /*!< The data length has been transferred*/
    union
    {
        uint32_t state; /*!< The state of the endpoint */
        struct
        {
            uint32_t maxPacketSize : 10U; /*!< The max packet size of the endpoint */
            uint32_t stalled : 1U;        /*!< The endpoint is stalled or not */
            uint32_t data0 : 1U;          /*!< The data toggle of the trascation */
            uint32_t bdtOdd : 1U;         /*!< the BDT toggle of the endpoint */
            uint32_t dmaAlign : 1U;       /*!< the transferBuffer is dma align or not */
            uint32_t transferring : 1U;   /*!< The endpoint is transferring */
            uint32_t resersed : 1U;       /*!< Resersed */
        } stateBitField;
    } stateUnion;
} usb_device_khci_endpoint_state_struct_t;

/*! @brief KHCI state structure */
typedef struct _usb_device_khci_state_struct
{
    usb_device_struct_t *deviceHandle; /*!< Device handle, used to identify the device object is belonged to */
    uint8_t *bdt;                      /*!< BDT buffer address */
    USB_Type *registerBase;            /*!< The base address of the register */
    uint8_t setupPacketBuffer[USB_SETUP_PACKET_SIZE * 2]; /*!< The setup request buffer */
    uint8_t
        *dmaAlignBuffer; /*!< This buffer is used to fix transferBuffer or transferLength does
                           not align to 4-byte when the function USB_DeviceKhciRecv is called.
                           The marco USB_DEVICE_CONFIG_KHCI_DMA_ALIGN is used to enable or disable this feature.
                           If the feature is enabled, when the transferBuffer or transferLength do not 4-byte alignment
                           and the transferLength is not more than USB_DEVICE_CONFIG_KHCI_DMA_ALIGN_BUFFER_LENGTH and
                           the flag isDmaAlignBufferInusing is zero, the dmaAlignBuffer is used to receive data
                           and the flag isDmaAlignBufferInusing will be set to 1.
                           When the transfer is done, the received data, kept in dmaAlignBuffer, will be copied
                           to transferBuffer, and the flag isDmaAlignBufferInusing will be cleared.
                            */
    usb_device_khci_endpoint_state_struct_t
        endpointState[USB_DEVICE_CONFIG_ENDPOINTS * 2]; /*!< Endpoint state structures */
    uint8_t isDmaAlignBufferInusing;                    /*!< The dmaAlignBuffer is used or not */
    uint8_t isResetting;                                /*!< Is doing device reset or not */
    uint8_t controllerId;                               /*!< Controller id */
    uint8_t setupBufferIndex;                           /*!< Valid setup buffer flag */
} usb_device_khci_state_struct_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name USB device khci functions
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Initialize the USB device KHCI instance.
 *
 * This function initizlizes the USB device KHCI module specified by the controllerId.
 *
 * @param[in] controllerId The controller id of the USB IP. Please refer to enumeration type usb_controller_index_t.
 * @param[in] handle        Pointer of the device handle, used to identify the device object is belonged to.
 * @param[out] khciHandle   It is out parameter, is used to return pointer of the device KHCI handle to the caller.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceKhciInit(uint8_t controllerId,
                                usb_device_handle handle,
                                usb_device_controller_handle *khciHandle);

/*!
 * @brief De-initialize the USB device KHCI instance.
 *
 * This function de-initizlizes the USB device KHCI module.
 *
 * @param[in] khciHandle   Pointer of the device KHCI handle.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceKhciDeinit(usb_device_controller_handle khciHandle);

/*!
 * @brief Send data through a specified endpoint.
 *
 * This function sends data through a specified endpoint.
 *
 * @param[in] khciHandle      Pointer of the device KHCI handle.
 * @param[in] endpointAddress Endpoint index.
 * @param[in] buffer           The memory address to hold the data need to be sent.
 * @param[in] length           The data length need to be sent.
 *
 * @return A USB error code or kStatus_USB_Success.
 *
 * @note The return value just means if the sending request is successful or not; the transfer done is notified by the
 * corresponding callback function.
 * Currently, only one transfer request can be supported for one specific endpoint.
 * If there is a specific requirement to support multiple transfer requests for one specific endpoint, the application
 * should implement a queue in the application level.
 * The subsequent transfer could begin only when the previous transfer is done (get notification through the endpoint
 * callback).
 */
usb_status_t USB_DeviceKhciSend(usb_device_controller_handle khciHandle,
                                uint8_t endpointAddress,
                                uint8_t *buffer,
                                uint32_t length);

/*!
 * @brief Receive data through a specified endpoint.
 *
 * This function Receives data through a specified endpoint.
 *
 * @param[in] khciHandle      Pointer of the device KHCI handle.
 * @param[in] endpointAddress Endpoint index.
 * @param[in] buffer           The memory address to save the received data.
 * @param[in] length           The data length want to be received.
 *
 * @return A USB error code or kStatus_USB_Success.
 *
 * @note The return value just means if the receiving request is successful or not; the transfer done is notified by the
 * corresponding callback function.
 * Currently, only one transfer request can be supported for one specific endpoint.
 * If there is a specific requirement to support multiple transfer requests for one specific endpoint, the application
 * should implement a queue in the application level.
 * The subsequent transfer could begin only when the previous transfer is done (get notification through the endpoint
 * callback).
 */
usb_status_t USB_DeviceKhciRecv(usb_device_controller_handle khciHandle,
                                uint8_t endpointAddress,
                                uint8_t *buffer,
                                uint32_t length);

/*!
 * @brief Cancel the pending transfer in a specified endpoint.
 *
 * The function is used to cancel the pending transfer in a specified endpoint.
 *
 * @param[in] khciHandle      Pointer of the device KHCI handle.
 * @param[in] ep               Endpoint address, bit7 is the direction of endpoint, 1U - IN, abd 0U - OUT.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceKhciCancel(usb_device_controller_handle khciHandle, uint8_t ep);

/*!
 * @brief Control the status of the selected item.
 *
 * The function is used to control the status of the selected item.
 *
 * @param[in] khciHandle      Pointer of the device KHCI handle.
 * @param[in] type             The selected item. Please refer to enumeration type usb_device_control_type_t.
 * @param[in,out] param            The param type is determined by the selected item.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceKhciControl(usb_device_controller_handle khciHandle,
                                   usb_device_control_type_t type,
                                   void *param);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* __USB_DEVICE_KHCI_H__ */
