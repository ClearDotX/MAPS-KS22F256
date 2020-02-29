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

#ifndef __USB_DEVICE_CLASS_H__
#define __USB_DEVICE_CLASS_H__

/*!
 * @addtogroup usb_device_class_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Macro to define class handle */
#define class_handle_t uint32_t

/*! @brief Available class types. */
typedef enum _usb_usb_device_class_type
{
    kUSB_DeviceClassTypeHid = 1U,
    kUSB_DeviceClassTypeCdc,
    kUSB_DeviceClassTypeMsc,
    kUSB_DeviceClassTypeAudio,
    kUSB_DeviceClassTypePhdc,
    kUSB_DeviceClassTypeVideo,
    kUSB_DeviceClassTypePrinter,
    kUSB_DeviceClassTypeDfu,
    kUSB_DeviceClassTypeCcid,
} usb_device_class_type_t;

/*! @brief Available common class events. */
typedef enum _usb_device_class_event
{
    kUSB_DeviceClassEventClassRequest = 1U,
    kUSB_DeviceClassEventDeviceReset,
    kUSB_DeviceClassEventSetConfiguration,
    kUSB_DeviceClassEventSetInterface,
    kUSB_DeviceClassEventSetEndpointHalt,
    kUSB_DeviceClassEventClearEndpointHalt,
} usb_device_class_event_t;

/*!
 * @brief Obtains the endpoint data structure.
 *
 * Define the endpoint data structure.
 *
 */
typedef struct _usb_device_endpoint_struct
{
    uint8_t endpointAddress; /*!< endpoint address*/
    uint8_t transferType;    /*!< endpoint transfer type*/
    uint16_t maxPacketSize;  /*!< endpoint max packet size */
} usb_device_endpoint_struct_t;

/*!
* @brief Obtains the endpoint group.
*
* Structure Representing Endpoints and number of endpoints user want.
*
*/
typedef struct _usb_device_endpoint_list
{
    uint8_t count;                          /*!< how many endpoints in current interface*/
    usb_device_endpoint_struct_t *endpoint; /*!< endpoint structure list*/
} usb_device_endpoint_list_t;

/*!
 * @brief Obtains the interface list data structure.
 *
 * Structure Representing interface.
 *
 */
typedef struct _usb_device_interface_struct
{
    uint8_t alternateSetting;                /*!< alternate setting number*/
    usb_device_endpoint_list_t endpointList; /*!< endpoints of the interface*/
    void *classSpecific;                     /*!< class specific structure handle*/
} usb_device_interface_struct_t;

/*!
 * @brief Obtains the interface data structure.
 *
 * Structure Representing interface.
 *
 */
typedef struct _usb_device_interfaces_struct
{
    uint8_t classCode;                        /*!< class code of the interface*/
    uint8_t subclassCode;                     /*!< subclass code of the interface*/
    uint8_t protocolCode;                     /*!< protocol code of the interface*/
    uint8_t interfaceNumber;                  /*!< interface number*/
    usb_device_interface_struct_t *interface; /*!< interface structure list*/
    uint8_t count;                            /*!< how many interfaces in current interface*/
} usb_device_interfaces_struct_t;

/*!
 * @brief Obtains the interface group.
 *
 * Structure Representing how many interfaces in one class type.
 *
 */
typedef struct _usb_device_interface_list
{
    uint8_t count;                              /*!< how many interfaces of the class*/
    usb_device_interfaces_struct_t *interfaces; /*!< all interfaces*/
} usb_device_interface_list_t;

/*!
 * @brief Obtains the class data structure.
 *
 * Structure Representing how many configurations in one class type.
 *
 */
typedef struct _usb_device_class_struct
{
    usb_device_interface_list_t *interfaceList; /*!< interfaces of the class*/
    usb_device_class_type_t type;               /*!< class type*/
    uint8_t configurations;                     /*!< how many configurations of the class*/
} usb_device_class_struct_t;

/*callback function pointer structure for application to provide class params*/
typedef usb_status_t (*usb_device_class_callback_t)(class_handle_t classHandle,
                                                    uint32_t callbackEvent,
                                                    void *eventParam);

/*!
 * @brief Obtains the device class information structure.
 *
 * Structure Representing the device class information. This structure only can be stored in RAM space.
 *
 */
typedef struct _usb_device_class_config_struct
{
    usb_device_class_callback_t classCallback;  /*!< class callback function to handle the Device status related event
                                                   for the specified type of class*/
    class_handle_t classHandle;                 /*!< the class handle of the class, filled by the common driver.*/
    usb_device_class_struct_t *classInfomation; /*!< detail information of the class*/
} usb_device_class_config_struct_t;

/*!
 * @brief Obtains the device class configuration structure.
 *
 * Structure Representing the device class configuration information.
 *
 */
typedef struct _usb_device_class_config_list_struct
{
    usb_device_class_config_struct_t *config; /*!< Array of class configuration structures */
    usb_device_callback_t deviceCallback;     /*!< Device callback function */
    uint8_t count;                            /*!< Number of class supported */
} usb_device_class_config_list_struct_t;

/*!
 * @brief Obtains the control request structure.
 *
 * This structure is used to pass the control request iniformation.
 * The structure is used in following two cases.
 * Case one, the host wants to send data to the deice in control data stage: @n
 *         1. If a setup packet received, the structure is used to pass the setup packet data and wants to get the
 * buffer to recevie data sent from the host.
 *            In this way, the field isSetup is 1.
 *            The length is requested buffer length.
 *            The buffer is filled by the class or application by using the valid buffer address.
 *            The setup is setup packet address.
 *         2. If the data received sent by host, the structure is used to pass the data buffer address and the data
 * length sent by the host.
 *            In this way, the fidle isSetup is 0.
 *            The buffer is the address of the data sent from the host.
 *            The length is the received data length.
 *            The setup is setup packet address. @n
 * Case two, the host wants to get data from the device in control data stage: @n
 *         1. If the setup packet received, the structure is used to pass the setup packet data and wants to get the
 * data buffer address to sent data to the host.
 *            In this way, the field isSetup is 1.
 *            The length is requested data length.
 *            The buffer is filled by the class or application by using the valid buffer address.
 *            The setup is setup packet address.
 *
 */
typedef struct _usb_device_control_request_struct
{
    usb_setup_struct_t *setup; /*!< The pointer of the setup packet data. */
    uint8_t *buffer;           /*!< Pass the buffer address. */
    uint32_t length;           /*!< Pass the buffer length or requested length. */
    uint8_t isSetup;           /*!< Is a setup packet received? */
} usb_device_control_request_struct_t;

/*! @brief Obtains the control get descriptor request common structure. */
typedef struct _usb_device_get_descriptor_common_struct
{
    uint8_t *buffer; /*!< Pass the buffer address. */
    uint32_t length; /*!< Pass the buffer length. */
} usb_device_get_descriptor_common_struct_t;

/*! @brief Obtains the control get device descriptor request structure. */
typedef struct _usb_device_get_device_descriptor_struct
{
    uint8_t *buffer; /*!< Pass the buffer address. */
    uint32_t length; /*!< Pass the buffer length. */
} usb_device_get_device_descriptor_struct_t;

/*! @brief Obtains the control get configuration descriptor request structure. */
typedef struct _usb_device_get_configuration_descriptor_struct
{
    uint8_t *buffer;       /*!< Pass the buffer address. */
    uint32_t length;       /*!< Pass the buffer length. */
    uint8_t configuration; /*!< The configuration number. */
} usb_device_get_configuration_descriptor_struct_t;

/*! @brief Obtains the control get string descriptor request structure. */
typedef struct _usb_device_get_string_descriptor_struct
{
    uint8_t *buffer;     /*!< Pass the buffer address. */
    uint32_t length;     /*!< Pass the buffer length. */
    uint16_t languageId; /*!< Language ID. */
    uint8_t stringIndex; /*!< String index. */
} usb_device_get_string_descriptor_struct_t;

/*! @brief Obtains the control get hid descriptor request structure. */
typedef struct _usb_device_get_hid_descriptor_struct
{
    uint8_t *buffer;         /*!< Pass the buffer address. */
    uint32_t length;         /*!< Pass the buffer length. */
    uint8_t interfaceNumber; /*!< The interface number. */
} usb_device_get_hid_descriptor_struct_t;

/*! @brief Obtains the control get hid report descriptor request structure. */
typedef struct _usb_device_get_hid_report_descriptor_struct
{
    uint8_t *buffer;         /*!< Pass the buffer address. */
    uint32_t length;         /*!< Pass the buffer length. */
    uint8_t interfaceNumber; /*!< The interface number. */
} usb_device_get_hid_report_descriptor_struct_t;

/*! @brief Obtains the control get hid physical descriptor request structure. */
typedef struct _usb_device_get_hid_physical_descriptor_struct
{
    uint8_t *buffer;         /*!< Pass the buffer address. */
    uint32_t length;         /*!< Pass the buffer length. */
    uint8_t index;           /*!< Phyical index */
    uint8_t interfaceNumber; /*!< The interface number. */
} usb_device_get_hid_physical_descriptor_struct_t;

/*! @brief Obtains the control get descriptor request common union. */
typedef union _usb_device_get_descriptor_common_union
{
    usb_device_get_descriptor_common_struct_t commonDescriptor; /*!< Common structure. */
    usb_device_get_device_descriptor_struct_t deviceDescriptor; /*!< The structure to get device descriptor. */
    usb_device_get_configuration_descriptor_struct_t
        configurationDescriptor;                                /*!< The structure to get configuration descriptor. */
    usb_device_get_string_descriptor_struct_t stringDescriptor; /*!< The structure to get string descriptor. */
    usb_device_get_hid_descriptor_struct_t hidDescriptor;       /*!< The structure to get hid descriptor. */
    usb_device_get_hid_report_descriptor_struct_t
        hidReportDescriptor; /*!< The structure to get hid report descriptor. */
    usb_device_get_hid_physical_descriptor_struct_t
        hidPhysicalDescriptor; /*!< The structure to get hid physical descriptor. */
} usb_device_get_descriptor_common_union_t;

/*! @brief Define function type for class device instance initialization */
typedef usb_status_t (*usb_device_class_init_call_t)(uint8_t controllerId,
                                                     usb_device_class_config_struct_t *classConfig,
                                                     class_handle_t *classHandle);
/*! @brief Define function type for class device instance deinitialization, internal */
typedef usb_status_t (*usb_device_class_deinit_call_t)(class_handle_t handle);
/*! @brief Define function type for class device instance Event change */
typedef usb_status_t (*usb_device_class_event_callback_t)(void *classHandle, uint32_t event, void *param);

/*! @brief Define class driver interface structure. */
typedef struct _usb_device_class_map
{
    usb_device_class_init_call_t classInit;     /*!< class driver initialization- entry  of the class driver */
    usb_device_class_deinit_call_t classDeinit; /*!< class driver de-initialization*/
    usb_device_class_event_callback_t classEventCallback; /*!< class driver event callback*/
    usb_device_class_type_t type;                         /*!< class type*/
} usb_device_class_map_t;

/*! @brief Structure holding common class state information */
typedef struct _usb_device_common_class_struct
{
    usb_device_handle handle;                          /*!< usb device handle*/
    usb_device_class_config_list_struct_t *configList; /*!< usb device configure list*/
    uint8_t setupBuffer[USB_SETUP_PACKET_SIZE];        /*!< Setup packet data buffer*/
    uint16_t standardTranscationBuffer;                /*!<
                                                        * This variable is used in:
                                                        *           get status request
                                                        *           get configuration request
                                                        *           get interface request
                                                        *           set interface request
                                                        *           get sync frame request
                                                        */
    uint8_t controllerId;                              /*!< controller id*/
} usb_device_common_class_struct_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the common class and the supported classes.
 *
 * This function is used to initialize the common class and the supported classes.
 *
 * @param[in] controllerId   The controller id of the USB IP. Please refer to the enumeration #usb_controller_index_t.
 * @param[in] configList     The class configurations. The pointer must point to the goblal variable.
 *                           Please refer to the structure #usb_device_class_config_list_struct_t.
 * @param[out] handle        It is out parameter, is used to return pointer of the device handle to the caller.
 *                           The value of parameter is a pointer points the device handle, and this design is uesd to
 *                           make simple device align with composite device. For composite device, there are many
 *                           kinds of class handle, but there is only one device handle. So the handle points to
 *                           a device instead of a class. And the class handle can be got from the
 *                           #usb_device_class_config_struct_t::classHandle after the the function successfully.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceClassInit(uint8_t controllerId,
                                 usb_device_class_config_list_struct_t *configList,
                                 usb_device_handle *handle);

/*!
 * @brief De-initialize the common class and the supported classes.
 *
 * This function is used to de-initialize the common class and the supported classes.
 *
 * @param[in] controllerId   The controller id of the USB IP. Please refer to the enumeration #usb_controller_index_t.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceClassDeinit(uint8_t controllerId);

/*!
 * @brief Get the USB bus speed.
 *
 * This function is used to get the USB bus speed.
 *
 * @param[in] controllerId   The controller id of the USB IP. Please refer to the enumeration #usb_controller_index_t.
 * @param[out] speed           It is an OUT parameter, return current speed of the controller.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceClassGetSpeed(uint8_t controllerId, uint8_t *speed);

/*!
 * @brief Handle the event passed to the class drivers.
 *
 * This function handles the event passed to the class drivers.
 *
 * @param[in] handle          The device handle, got from the #USB_DeviceInit.
 * @param[in] event           The event codes. Please refer to the enumeration #usb_device_class_event_t.
 * @param[in,out] param           The param type is determined by the event code.
 *
 * @return A USB error code or kStatus_USB_Success.
 * @retval kStatus_USB_Success              Free device hanlde successfully.
 * @retval kStatus_USB_InvalidParameter     The device handle not be found.
 * @retval kStatus_USB_InvalidRequest       The request is invalid, and the control pipe will be stalled by the caller.
 */
usb_status_t USB_DeviceClassEvent(usb_device_handle handle, usb_device_class_event_t event, void *param);

/*!
 * @brief Handle the common class callback.
 *
 * This function handles the common class callback.
 *
 * @param[in] handle          The device handle, got from the #USB_DeviceInit.
 * @param[in] event           The event codes. Please refer to the enumeration #usb_device_event_t.
 * @param[in,out] param           The param type is determined by the event code.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceClassCallback(usb_device_handle handle, uint32_t event, void *param);

/*!
 * @brief Get the device handle according to the controller id.
 *
 * This function gets the device handle according to the controller id.
 *
 * @param[in] controllerId   The controller id of the USB IP. Please refer to the enumeration #usb_controller_index_t.
 * @param[out] handle          It is out parameter, is used to return pointer of the device handle to the caller.
 *
 * @retval kStatus_USB_Success              Get device hanlde successfully.
 * @retval kStatus_USB_InvalidParameter     The device handle can not be found.
 */
usb_status_t USB_DeviceClassGetDeviceHandle(uint8_t controllerId, usb_device_handle *handle);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __USB_DEVICE_CLASS_H__ */
