/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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

#include <stdio.h>
#include "fsl_flexcan_hal.h"
#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "fsl_clock_configs.h"
#include "fsl_clock_manager.h"

#if defined(CPU_MK70FN1M0VMJ12) || defined(CPU_MK70FN1M0VMJ15)
#define CAN_DEVICE  1
#else
#define CAN_DEVICE  0
#endif

/* Global variables*/

/* Set NODE to 1 or 2 depending on which endpoint you are running. See notes in test.c*/
#define NODE 1
uint32_t TX_identifier;
uint32_t RX_identifier;
uint32_t TX_remote_identifier;
uint32_t RX_remote_identifier;
uint32_t TX_mailbox_num;
uint32_t RX_mailbox_num;
uint32_t TX_remote_mailbox_num;
uint32_t RX_remote_mailbox_num;
flexcan_user_config_t flexcan1_data;
flexcan_id_table_t id_table;
flexcan_mb_t rx_fifo;
flexcan_mb_t rx_mb;
uint8_t instance;
bool int_mb;
bool int_fifo;
uint32_t rx_mb_idx;
semaphore_t irqSync;
bool int_mb;
bool int_fifo;
uint32_t rx_mb_idx;
flexcan_mb_code_status_t tx_cs1;
uint32_t error_number;
uint32_t check_errors;

/*!
 * @brief Interrupt handler for a FlexCAN instance.
 *
 * @param   can_ptr                   point to a FlexCAN instance.
 */
void flexcan_irq_handler(uint8_t instance)
{
    volatile uint32_t                      tmp_reg;

    /* get the interrupt flag */
    tmp_reg = (flexcan_hal_get_all_mb_int_flags(instance)) & CAN_IMASK1_BUFLM_MASK;

    /* check Tx/Rx interrupt flag and clear the interrupt*/
    if(tmp_reg){
        if (tmp_reg & 0x20)
        {
            int_fifo = true;
        }

        if (tmp_reg & (1 << rx_mb_idx))
        {
            int_mb = true;
        }

        /* clear the interrupt and unlock message buffer */
        OSA_SemaPost(&irqSync);
        flexcan_hal_clear_mb_int_flag(instance, tmp_reg);
        flexcan_hal_unlock_rx_mb(instance);
   }

    /* Clear all other interrupts in ERRSTAT register (Error, Busoff, Wakeup)*/
    flexcan_hal_clear_err_interrupt_status(instance);

    return;
}

/*! @brief Implementation of CAN0 handler named in startup code. */
/*!*/
/*! Passes instance to generic FlexCAN IRQ handler.*/
void CAN0_ORed_Message_buffer_IRQHandler(void)
{
    flexcan_irq_handler(0);
}

/*! @brief Implementation of CAN1 handler named in startup code. */
/*!*/
/*! Passes instance to generic FlexCAN IRQ handler.*/
void CAN1_ORed_Message_buffer_IRQHandler(void)
{
    flexcan_irq_handler(1);
}

/*! @brief Implementation of CAN0 handler named in startup code. */
/*!*/
/*! Passes instance to generic FlexCAN IRQ handler.*/
void CAN0_Bus_Off_IRQHandler(void)
{
    flexcan_irq_handler(0);
}

/*! @brief Implementation of CAN1 handler named in startup code. */
/*!*/
/*! Passes instance to generic FlexCAN IRQ handler.*/
void CAN1_Bus_Off_IRQHandler(void)
{
    flexcan_irq_handler(1);
}

/*! @brief Implementation of CAN0 handler named in startup code. */
/*!*/
/*! Passes instance to generic FlexCAN IRQ handler.*/
void CAN0_Error_IRQHandler(void)
{
    flexcan_irq_handler(0);
}

/*! @brief Implementation of CAN1 handler named in startup code. */
/*!*/
/*! Passes instance to generic FlexCAN IRQ handler.*/
void CAN1_Error_IRQHandler(void)
{
    flexcan_irq_handler(1);
}

/*! @brief Implementation of CAN0 handler named in startup code. */
/*!*/
/*! Passes instance to generic FlexCAN IRQ handler.*/
void CAN0_Wake_Up_IRQHandler(void)
{
    flexcan_irq_handler(0);
}

/*! @brief Implementation of CAN1 handler named in startup code. */
/*!*/
/*! Passes instance to generic FlexCAN IRQ handler.*/
void CAN1_Wake_Up_IRQHandler(void)
{
    flexcan_irq_handler(1);
}

/*!
 * @brief Send configuration and transfer FlexCAN messages
 */
void transfer(void)
{
    uint8_t   data = 45;
    uint32_t result, temp, i, transmitDataCount;
    uint32_t receiveDataCount;

    /*Standard ID*/
    tx_cs1.code = kFlexCanTX_Data;
    tx_cs1.msg_id_type = kFlexCanMbId_Std;
    tx_cs1.data_length = 1;
    tx_cs1.substitute_remote = 0;

    transmitDataCount = 1;
    receiveDataCount = 20;

    printf("\r\nFlexCAN send config");
    /* Initialize transmit mb*/
    tx_cs1.code = kFlexCanTX_Inactive;
    flexcan_hal_set_mb_tx(instance, &flexcan1_data, TX_mailbox_num, &tx_cs1, TX_identifier, NULL);

    /* Enable message buffer interrupt*/
    flexcan_hal_enable_mb_interrupt(instance, &flexcan1_data, TX_mailbox_num);

    i = transmitDataCount;
    while (i)
    {
        data++;

        /* Set up FlexCAN message buffer for transmitting data*/
        tx_cs1.code = kFlexCanTX_Data;
        result = flexcan_hal_set_mb_tx(instance, &flexcan1_data, TX_mailbox_num, &tx_cs1,
                                       TX_identifier, &data);
        if(result != kStatus_FLEXCAN_Success)
            printf("\r\nTransmit error. Error Code: 0x%lx", result);
        else
            printf("\r\nData transmit: 0x%x", data);

        i--;
    }

    while (receiveDataCount)
    {
        int_mb = false;
        int_fifo = false;

        /* Wait for the event*/
        osa_status_t syncStatus;
        
        do
        {
            syncStatus = OSA_SemaWait(&irqSync, 1);
        }while(syncStatus == kStatus_OSA_Idle);
        if (syncStatus != kStatus_OSA_Success)
        {
            printf("\r\nEvent Wait failed");
        }

        if (int_mb)
        {
            /* Lock RX MB*/
            flexcan_hal_lock_rx_mb(instance, &flexcan1_data, RX_mailbox_num);

            /* Get RX MB field values*/
            result = flexcan_hal_get_mb(instance, &flexcan1_data, RX_mailbox_num, &rx_mb);
            if(result)
                return;

            temp = ((rx_mb.cs) >> 16) & 0xF;
            printf("\r\nDLC=%d, mb_idx=%d", temp, RX_mailbox_num);
            printf("\r\nRX MB data: ");
            for (result = 0; result < temp; result++)
                printf ("0x%x ", rx_mb.data[result]);

            printf("\r\nID: 0x%x", rx_mb.msg_id);
        }

        if (int_fifo)
        {
            /* Lock RX FIFO*/
            flexcan_hal_lock_rx_mb(instance, &flexcan1_data, 0);

            /* Get RX FIFO field values*/
            result = flexcan_hal_read_fifo(instance, &rx_fifo);
            if(result)
                return;

            temp = ((rx_fifo.cs) >> 16) & 0xF;
            printf("\r\nDLC: 0x%x", temp);
            printf("\r\nRX FIFO data: ");
            for (result = 0; result < temp; result++)
                printf ("0x%x ", rx_fifo.data[result]);

            printf("\r\nID: 0x%x", rx_fifo.msg_id);
        }

        /* Unlock RX message buffer and RX FIFO*/
        flexcan_hal_unlock_rx_mb(instance);

        receiveDataCount--;
    }
}

/*!
 * @brief FlexCAN receive configuration
 */
void receive_config(void)
{
    flexcan_mb_code_status_t rx_cs1;
    uint32_t bit_mask = 0;

    rx_cs1.code = kFlexCanRX_Ranswer;
    rx_cs1.msg_id_type = kFlexCanMbId_Std;
    rx_cs1.data_length = 1;
    rx_cs1.substitute_remote = 0;

    printf("\r\nFlexCAN receive config");
    int_mb = false;
    int_fifo = false;

    if (flexcan1_data.is_rx_fifo_needed)
    {
        /* Configure RX FIFO fields*/
        /* Initialize rx fifo */
        flexcan_hal_set_rx_fifo(instance, &flexcan1_data, kFlexCanRxFifoIdElementFormat_A, &id_table);

        /* Enable RX FIFO interrupts*/
        flexcan_hal_enable_mb_interrupt(instance, &flexcan1_data, 5);

        flexcan_hal_enable_mb_interrupt(instance, &flexcan1_data, 6);

        flexcan_hal_enable_mb_interrupt(instance, &flexcan1_data, 7);
    }

    if (flexcan1_data.is_rx_mb_needed)
    {
        /* Configure RX MB fields*/
        rx_mb_idx = RX_mailbox_num;

        /* Initialize rx mb*/
        rx_cs1.code = kFlexCanRX_NotUsed;
        flexcan_hal_set_mb_rx(instance, &flexcan1_data, RX_mailbox_num, &rx_cs1, RX_identifier);

        /* Enable MB interrupt*/
        flexcan_hal_enable_mb_interrupt(instance, &flexcan1_data, RX_mailbox_num);

        /* Initialize receive MB*/
        rx_cs1.code = kFlexCanRX_Inactive;
        flexcan_hal_set_mb_rx(instance, &flexcan1_data, RX_mailbox_num, &rx_cs1, RX_identifier);

        /* Set up FlexCAN message buffer fields for receiving data*/
        rx_cs1.code = kFlexCanRX_Empty;
        flexcan_hal_set_mb_rx(instance, &flexcan1_data, RX_mailbox_num, &rx_cs1, RX_identifier);
    }

    /* Start receiving data*/
    /* Find out bit mask. Each set bit represents an event bit to wait for*/
    if (flexcan1_data.is_rx_mb_needed)
    {
        bit_mask |= 1 << RX_mailbox_num;
    }

    if (flexcan1_data.is_rx_fifo_needed)
    {
        bit_mask |= 1 << 5;
    }
    
    if (bit_mask == 0)
        return;
}

/*!
 * @brief FlexCAN HAL driver unit test
 * @return  0 if successful; non-zero failed.
 */
flexcan_status_t flexcan_hal_test(void)
{
   uint32_t result;
   flexcan_time_segment_t flexcan1_time_seg, flexcan1_time_seg2;
   uint32_t rx_fifo_id[8];
   uint32_t i;
#if !defined(CPU_MK64FN1M0VMD12)
   uint32_t clk, tq, bitrate;
#endif
   bool enable_err_interrupts = 1;
   uint32_t number_errors = 0;

   instance = CAN_DEVICE;
   flexcan1_data.num_mb = 16;
   flexcan1_data.max_num_mb = 16;
   flexcan1_data.num_id_filters = kFlexCanRxFifoIDFilters_8;
   flexcan1_data.is_rx_fifo_needed = 1;
   flexcan1_data.is_rx_mb_needed = 1;

   flexcan1_time_seg.pre_divider = 19;
   flexcan1_time_seg.propseg = 6;
   flexcan1_time_seg.pseg1 = 7;
   flexcan1_time_seg.pseg2 = 7;
   flexcan1_time_seg.rjw = 3;

   printf("\r\n*********FLEXCAN TEST PROGRAM.*********");
   printf("\r\n   Message format: Standard (11 bit id)");
   printf("\r\n   Message buffer 8 used for Rx.");
   printf("\r\n   Message buffer 9 used for Tx.");
   printf("\r\n   Interrupt Mode: Enabled");
   printf("\r\n   Operation Mode: TX and RX --> Normal");
   printf("\r\n***************************************\n");
  
    id_table.is_extended_mb = 0;
    id_table.is_remote_mb = 0;
    rx_fifo_id[0] = 0x666;
    rx_fifo_id[1] = 0x667;
    rx_fifo_id[2] = 0x676;
    rx_fifo_id[3] = 0x66E;
    rx_fifo_id[4] = 0x66F;
    for (i = 5; i < 8; i++)
        rx_fifo_id[i] = 0x6E6;
    id_table.id_filter = rx_fifo_id;

    /* Select mailbox number */
    RX_mailbox_num = 8;
    TX_mailbox_num = 9;
    RX_remote_mailbox_num = 10;
    TX_remote_mailbox_num = 11;

#if NODE==1
   RX_identifier = 0x123;
   TX_identifier = 0x321;
   RX_remote_identifier = 0x0F0;
   TX_remote_identifier = 0x00F;
#else
   RX_identifier = 0x321; 
   TX_identifier = 0x123;
   RX_remote_identifier = 0x00F;
   TX_remote_identifier = 0x0F0;
#endif  

    /* Select a source clock*/
    result = flexcan_hal_select_clk(instance, kFlexCanClkSource_Ipbus);
    if (result)
    {
        number_errors++;
        printf("\r\nFlexCAN select source clock failed");
    }

	if (!BR_CAN_CTRL1_CLKSRC(instance))
		printf("\r\nFlexCAN BUS clock is not selected");

	/* Disable FLEXCAN device*/
    result = flexcan_hal_disable(instance);
    if (result)
    {
        number_errors++;
        printf("\r\nFlexCAN disable failed");
    }

	if (!flexcan_hal_is_enabled(instance))
		printf("\r\nFlexCAN is not disabled");

    /* Enable FLEXCAN device*/
    result = flexcan_hal_enable(instance);
    if (result)
    {
        number_errors++;
        printf("\r\nFlexCAN enable failed");
    }

	if (flexcan_hal_is_enabled(instance))
		printf("\r\nFlexCAN is not enabled");

    /* Enter freeze mode*/
    flexcan_hal_enter_freeze_mode(instance);

    /* Exit freeze mode*/
    flexcan_hal_exit_freeze_mode(instance);
#if 1
    /* Reset FLEXCAN device*/
    result = flexcan_hal_sw_reset(instance);
    if (result)
    {
        number_errors++;
        printf("\r\nFlexCAN SW reset failed");
    }
#endif
    /* Initialize FLEXCAN device*/
    result = flexcan_hal_init(instance, &flexcan1_data);
    if (result)
    {
        number_errors++;
        printf("\r\nFlexCAN init failed, 0x%x", result);
    }

    /* Disable Normal mode*/
    result = flexcan_hal_disable_operation_mode(instance, kFlexCanNormalMode);
    if (result)
    {
        number_errors++;
        printf("\r\nFlexCAN enable operation mode failed");
    }

	if (!BR_CAN_MCR_SUPV(instance))
		printf("\r\nFlexCAN is in normal mode");

    /* Enable Normal mode*/
    result = flexcan_hal_enable_operation_mode(instance, kFlexCanNormalMode);
    if (result)
    {
        number_errors++;
        printf("\r\nFlexCAN enable operation mode failed");
    }

	if (BR_CAN_MCR_SUPV(instance))
		printf("\r\nFlexCAN is not in normal mode");

    /* Enable error interrupts*/
    if(enable_err_interrupts)
    {
        flexcan_hal_enable_error_interrupt(instance);
		if (!BR_CAN_CTRL1_BOFFMSK(instance))
		    printf("\r\nFlexCAN bus off interrupt is not enabled");

		if (!BR_CAN_CTRL1_ERRMSK(instance))
		    printf("\r\nFlexCAN error interrupt is not enabled");

		if (!BR_CAN_MCR_WAKMSK(instance))
		    printf("\r\nFlexCAN wakeup interrupt is not enabled");

        /* Init the interrupt sync object.*/
        OSA_SemaCreate(&irqSync, 0);

        /* configure and enable the uart instance IRQ */
        IRQn_Type canInstanceIrq[4];
        if(instance == 0)
        {
            canInstanceIrq[0] = CAN0_ORed_Message_buffer_IRQn;
            canInstanceIrq[1] = CAN0_Bus_Off_IRQn;
            canInstanceIrq[2] = CAN0_Error_IRQn;
            canInstanceIrq[3] = CAN0_Wake_Up_IRQn;
        }
#if !defined(CPU_MK64FN1M0VMD12)
        else 
        {
            canInstanceIrq[0] = CAN1_ORed_Message_buffer_IRQn;
            canInstanceIrq[1] = CAN1_Bus_Off_IRQn;
            canInstanceIrq[2] = CAN1_Error_IRQn;
            canInstanceIrq[3] = CAN1_Wake_Up_IRQn;
        }
#endif
        for (i = 0; i < 4; i++)
        {
            /* Set interrupt priority to medium.*/
            NVIC_SetPriority(canInstanceIrq[i], 2);

            /* Enable FlexCAN interrupt.*/
            NVIC_EnableIRQ(canInstanceIrq[i]);
        }
    }

    flexcan_hal_set_time_segments(instance, &flexcan1_time_seg);

    /* Get the time segments*/
    flexcan_hal_get_time_segments(instance, &flexcan1_time_seg2);

    if (!((flexcan1_time_seg2.pre_divider == flexcan1_time_seg.pre_divider) &&
        (flexcan1_time_seg2.propseg == flexcan1_time_seg.propseg) &&
        (flexcan1_time_seg2.pseg1 == flexcan1_time_seg.pseg1) &&
        (flexcan1_time_seg2.pseg2 == flexcan1_time_seg.pseg2) &&
        (flexcan1_time_seg2.rjw == flexcan1_time_seg.rjw)))
        printf("\r\nFlexCAN time segments are not correct");
#if !defined(CPU_MK64FN1M0VMD12)
    CLOCK_SYS_GetFreq(kBusClock, &clk);
    tq = 1;
    tq = (1+(1+flexcan1_time_seg2.propseg)+(1+flexcan1_time_seg2.pseg1)+(1+flexcan1_time_seg2.pseg2));
    bitrate = clk / tq;
    bitrate /= (flexcan1_time_seg2.pre_divider + 1);
    printf("\r\nFLEXCAN get bitrate: %d Hz", bitrate);
#endif
    flexcan_hal_set_mask_type(instance, kFlexCanRxMask_Global);

    if (flexcan1_data.is_rx_fifo_needed)
    {
        flexcan_hal_set_rx_fifo_global_std_mask(instance, 0x666);
    }

    if (flexcan1_data.is_rx_mb_needed)
    {
        flexcan_hal_set_rx_mb_global_std_mask(instance, 0x123);
    }

    receive_config();
    transfer();

    if (number_errors == 0)
        return kStatus_FLEXCAN_Success;
    else
        return kStatus_FLEXCAN_Fail;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

