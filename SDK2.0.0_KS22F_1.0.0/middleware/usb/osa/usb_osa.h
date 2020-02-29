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

#ifndef __USB_OSA_H__
#define __USB_OSA_H__

/*!
 * @addtogroup usb_os_abstraction
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Define big endian */
#define BIG_ENDIAN (0U)
/*! @brief Define little endian */
#define LITTLE_ENDIAN (1U)

/*! @brief Define current endian */
#define ENDIANNESS LITTLE_ENDIAN

/*! @brief Define usb osa event handle */
typedef void *usb_osa_event_handle;

/*! @brief Define usb osa semaphore handle */
typedef void *usb_osa_sem_handle;

/*! @brief Define usb osa mutex handle */
typedef void *usb_osa_mutex_handle;

/*! @brief Define usb osa message queue handle */
typedef void *usb_osa_msgq_handle;

/*! @brief USB OSA error code */
typedef enum _usb_osa_status
{
    kStatus_USB_OSA_Success = 0x00U, /*!< Success */
    kStatus_USB_OSA_Error,           /*!< Failed */
    kStatus_USB_OSA_TimeOut,         /*!< Timeout occurs while waiting */
} usb_osa_status_t;

/*! @brief The event flags are cleared automatically or manually.*/
typedef enum _usb_osa_event_mode
{
    kUSB_OsaEventManualClear = 0U, /*!< The flags of the event will be cleared manually. */
    kUSB_OsaEventAutoClear = 1U,   /*!< The flags of the event will be cleared automatically. */
} usb_osa_event_mode_t;

/* Include required header file based on RTOS selection */
#if defined(USB_STACK_BM)

#include "usb_osa_bm.h"

#elif defined(USB_STACK_FREERTOS)

#include "usb_osa_freertos.h"

#elif defined(USB_STACK_UCOSII)

#include "usb_osa_ucosii.h"

#elif defined(USB_STACK_UCOSIII)

#include "usb_osa_ucosiii.h"

#else

#error Not define RTOS in file "usb_osa.h".

#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name USB OSA Memeroy Management
 * @{
 */

/*!
 * @brief Reserve the requested amount of memory in bytes.
 *
 * The function is used to reserve the requested amount of memory in bytes and initializes it to 0.
 *
 * @param length Amount of bytes to reserve.
 *
 * @return Pointer to the reserved memory. NULL if memory could not be allocated.
 */
void *USB_OsaMemoryAllocate(uint32_t length);

/*!
 * @brief Release the memory previously reserved.
 *
 * The function is used to free the memory block previously reserved.
 *
 * @param p Pointer to the start of the memory block previously reserved.
 *
 */
extern void USB_OsaMemoryFree(void *p);

/* @} */

/*!
 * @name USB OSA Event
 * @{
 */

/*!
 * @brief Create an event object with all flags cleared.
 *
 * This function creates an event object and set its clear mode. If clear mode
 * is kUSB_OsaEventAutoClear, when a task gets the event flags, these flags will be
 * cleared automatically. If clear mode is kUSB_OsaEventManualClear, these flags must
 * be cleared manually.
 *
 * @param handle    It is out parameter, is used to return pointer of the event object.
 * @param flag      The event is auto-clear or manual-clear. Please refer to the enumeration #usb_osa_event_mode_t.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_event_handle eventHandle;
   usb_osa_status_t     usbOsaStatus;
   usbOsaStatus = USB_OsaEventCreate(&eventHandle, kUSB_OsaEventManualClear);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaEventCreate(usb_osa_event_handle *handle, uint32_t flag);

/*!
 * @brief Destroy a created event object.
 *
 * @param handle    Pointer to the event object.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_status_t     usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaEventDestroy(eventHandle);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaEventDestroy(usb_osa_event_handle handle);

/*!
 * @brief Set an event flags.
 *
 * Set specified flags for an event object.
 *
 * @param handle    Pointer to the event object.
 * @param bitMask  Event flags to be set.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_status_t     usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaEventSet(eventHandle, 0x01U);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaEventSet(usb_osa_event_handle handle, uint32_t bitMask);

/*!
 * @brief Wait an event flags.
 *
 * This function waits for a combination of flags to be set in an event object.
 * Applications can wait for any/all bits to be set. Also this function could
 * obtain the flags who wakeup the waiting task.
 *
 * @param handle    Pointer to the event object.
 * @param bitMask  Event flags to wait.
 * @param flag      Wait all flags or any flag to be set. 0U - wait any flag, others - wait all flags.
 * @param timeout   The maximum number of milliseconds to wait for the event.
 *                  If the wait condition is not met, passes 0U will
 *                  wait indefinitely when environment is RTOS. And returns kStatus_OSA_Timeout
 *                  immediately for BM whatever pass any value.
 * @param bitSet   Flags that wakeup the waiting task are obtained by this parameter.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_status_t     usbOsaStatus;
   uint32_t             bitSet;
   ...
   usbOsaStatus = USB_OsaEventWait(eventHandle, 0x01U, 0U, 0U, &bitSet);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaEventWait(
    usb_osa_event_handle handle, uint32_t bitMask, uint32_t flag, uint32_t timeout, uint32_t *bitSet);

/*!
 * @brief Check an event flags.
 *
 * This function checks for a combination of flags to be set in an event object.
 *
 * @param handle    Pointer to the event object.
 * @param bitMask  Event flags to check.
 * @param bitSet   Flags have been set.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_status_t     usbOsaStatus;
   uint32_t             bitSet;
   ...
   usbOsaStatus = USB_OsaEventCheck(eventHandle, 0x01U, &bitSet);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaEventCheck(usb_osa_event_handle handle, uint32_t bitMask, uint32_t *bitSet);

/*!
 * @brief Clear an event flags.
 *
 * This function clears flags of an event object.
 *
 * @param handle    Pointer to the event object
 * @param bitMask  Event flags to be cleared.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_status_t     usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaEventClear(eventHandle, 0x01U);
   @endcode
 */
extern usb_osa_status_t USB_OsaEventClear(usb_osa_event_handle handle, uint32_t bitMask);
/* @} */

/*!
 * @name USB OSA Semaphore
 * @{
 */

/*!
 * @brief Create a semaphore with a given value.
 *
 * This function creates a semaphore and sets the default count.
 *
 * @param handle    It is out parameter, is used to return pointer of the semaphore object.
 * @param count     Initialize value of the semaphore.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_sem_handle   semHandle;
   usb_osa_status_t     usbOsaStatus;
   usbOsaStatus = USB_OsaSemCreate(&semHandle, 1U);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaSemCreate(usb_osa_sem_handle *handle, uint32_t count);

/*!
 * @brief Destroy a semaphore object.
 *
 * This function destroys a semaphore object.
 *
 * @param handle    Pointer to the semaphore.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_sem_handle   semHandle;
   usb_osa_status_t     usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaSemDestroy(semHandle);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaSemDestroy(usb_osa_sem_handle handle);

/*!
 * @brief Post a semephore
 *
 * This function wakes up a task waiting on the semephore. If these is not task pending, increases the semephore's
 value.
 *
 * @param handle    Pointer to the semaphore.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_sem_handle   semHandle;
   usb_osa_status_t     usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaSemPost(semHandle);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaSemPost(usb_osa_sem_handle handle);

/*!
 * @brief Wait a semephore.
 *
 * This function check semephore's value. If it is positive, decreases the semephore's value and return
 kStatus_OSA_Success.
 *
 * @param handle    Pointer to the semaphore.
 * @param timeout   The maximum number of milliseconds to wait for the semephore.
 *                  If the wait condition is not met, pass 0U will
 *                  wait indefinitely when environment is RTOS. And return kStatus_OSA_Timeout
 *                  immediately for BM whatever pass any value.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_sem_handle   semHandle;
   usb_osa_status_t     usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaSemWait(semHandle, 0U);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaSemWait(usb_osa_sem_handle handle, uint32_t timeout);
/* @} */

/*!
 * @name USB OSA Mutex
 * @{
 */

/*!
 * @brief Create a mutex.
 *
 * This function creates a mutex and sets it to unlocked status.
 *
 * @param handle    It is out parameter, is used to return pointer of the mutex object.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_mutex_handle mutexHandle;
   usb_osa_status_t     usbOsaStatus;
   usbOsaStatus = USB_OsaMutexCreate(&mutexHandle);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaMutexCreate(usb_osa_mutex_handle *handle);

/*!
 * @brief Destroy a mutex.
 *
 * This function destroys a mutex and sets it to unlocked status.
 *
 * @param handle    Pointer to the mutex.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_mutex_handle mutexHandle;
   usb_osa_status_t     usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaMutexDestroy(mutexHandle);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaMutexDestroy(usb_osa_mutex_handle handle);

/*!
 * @brief Wait for a mutex and lock it.
 *
 * This function checks the mutex's status. If it is unlocked, locks it and returns the
 * kStatus_OSA_Success. Otherwise, waits forever to lock in RTOS, and return
 * kStatus_OSA_Success immediately for BM.
 *
 * @param handle    Pointer to the mutex.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_mutex_handle mutexHandle;
   usb_osa_status_t     usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaMutexLock(mutexHandle);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaMutexLock(usb_osa_mutex_handle handle);

/*!
 * @brief unlock lock a mutex.
 *
 * This function unlock a mutex.
 *
 * @param handle    Pointer to the mutex.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_mutex_handle mutexHandle;
   usb_osa_status_t     usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaMutexUnlock(mutexHandle);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaMutexUnlock(usb_osa_mutex_handle handle);
/* @} */

/*!
 * @name USB OSA Message Queue
 * @{
 */

/*!
 * @brief Create a message queue.
 *
 * This function creates a message queue.
 *
 * @param handle    It is out parameter, is used to return pointer of the message queue object.
 * @param count     The count of elements in the queue.
 * @param size      Size of every elements in words.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_msgq_handle  msgqHandle;
   usb_osa_status_t     usbOsaStatus;
   usbOsaStatus = USB_OsaMsgqCreate(msgqHandle, 8U, 4U);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaMsgqCreate(usb_osa_msgq_handle *handle, uint32_t count, uint32_t size);

/*!
 * @brief Destroy a message queue.
 *
 * This function destroys a message queue.
 *
 * @param handle    Pointer to a message queue.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_msgq_handle  msgqHandle;
   usb_osa_status_t     usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaMsgqDestroy(msgqHandle);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaMsgqDestroy(usb_osa_msgq_handle handle);

/*!
 * @brief Send a message.
 *
 * This function sends a message to the tail of the message queue.
 *
 * @param handle    Pointer to a message queue.
 * @param msg       The pointer to a message to be put into the queue.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_msgq_handle      msgqHandle;
   message_struct_t         message;
   usb_osa_status_t         usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaMsgqSend(msgqHandle, &message);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaMsgqSend(usb_osa_msgq_handle handle, void *msg);

/*!
 * @brief Receive a message.
 *
 * This function receives a message from the head of the message queue.
 *
 * @param handle    Pointer to a message queue.
 * @param msg       The pointer to save a received message.
 * @param timeout   The maximum number of milliseconds to wait for a message.
 *                  If the wait condition is not met, passes 0U will
 *                  wait indefinitely when environment is RTOS. And returns kStatus_OSA_Timeout
 *                  immediately for BM whatever pass any value.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_msgq_handle      msgqHandle;
   message_struct_t         message;
   usb_osa_status_t         usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaMsgqRecv(msgqHandle, &message, 0U);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaMsgqRecv(usb_osa_msgq_handle handle, void *msg, uint32_t timeout);

/*!
 * @brief Check a message queue, and receive a message if the queue is not empty.
 *
 * This function checkes a message queue, and receives a message if the queue is not empty.
 *
 * @param handle    Pointer to a message queue.
 * @param msg       The pointer to save a received message.
 *
 * @return A USB OSA error code or kStatus_OSA_Success.
 *
 * Example:
   @code
   usb_osa_msgq_handle      msgqHandle;
   message_struct_t         message;
   usb_osa_status_t         usbOsaStatus;
   ...
   usbOsaStatus = USB_OsaMsgqCheck(msgqHandle, &message);
   @endcode
 *
 */
extern usb_osa_status_t USB_OsaMsgqCheck(usb_osa_msgq_handle handle, void *msg);

/* @} */

#if defined(__cplusplus)
}
#endif

/* @} */

#endif /* __USB_OSA_H__ */
