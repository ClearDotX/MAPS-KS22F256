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


#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include <assert.h>
#include "spi_hal_test.h"
#include "board.h"
#include <stdio.h>

uint32_t byteCount;
uint32_t timeoutValue = 1;

/*! @name SPI hal test*/
/*@{*/


/*! @brief Calls the spi hal test*/
/*!*/
/*! This function initializes the SPI module as slave and waits for SPI master */
/*! to start transfering data.*/
/*!*/
/*! @return kStatus_SPI_Success*/
spi_status_t spi_test(void);

/*! @brief Table of base pointers for SPI instances. */
SPI_Type * const g_spiBase[SPI_INSTANCE_COUNT] = SPI_BASE_PTRS;

const static char s_spi_hal_test_error[HW_SPI_HAL_TEST_COUNT][100] =
{
    {"SPI_HAL_Init() test failed\n\r"},
    {"SPI_HAL_Enable() test failed\n\r"},
    {"SPI_HAL_Disable() test failed\n\r"},
    {"SPI_HAL_SetBaud() test failed\n\r"},
    {"SPI_HAL_SetBaudDivisors() test failed\n\r"},
    {"SPI_HAL_SetMasterSlave() test failed\n\r"},
    {"SPI_HAL_SetSlaveSelectOutputMode() test failed\n\r"},
    {"SPI_HAL_SetDataFormat() test failed\n\r"},
    {"SPI_HAL_SetPinMode() test failed\n\r"},
    {"SPI_HAL_ConfigureDma() test failed\n\r"},
    {"SPI_HAL_ConfigureStopInWaitMode() test failed\n\r"},
    {"SPI_HAL_Enable_receive_and_fault_interrupt() test failed\n\r"},
    {"SPI_HAL_Disable_receive_and_fault_interrupt() test failed\n\r"},
    {"SPI_HAL_Enable_transmit_interrupt test failed\n\r"},
    {"SPI_HAL_Disable_transmit_interrupt() test failed\n\r"},
    {"SPI_HAL_Enable_match_interrupt() test failed\n\r"},
    {"SPI_HAL_Disable_match_interrupt() test failed\n\r"},
    {"SPI_HAL_ClearModeFault() test failed\n\r"},
    {"SPI_HAL_ClearMatch() test failed\n\r"},
    {"SPI_HAL_SetMatchVal() test failed\n\r"},
    {"SPI_HAL_SPIbitMODE() test failed\n\r"},
    {"SPI_HAL_INTCLR() test failed\n\r"},
    {"SPI_HAL_FIFOMODE() test failed\n\r"},
    {"SPI_HAL_FIFOINTMODE() test failed\n\r"},
    {"SPI_HAL_STATUSFLAG() test failed\n\r"},
};
/*! @brief unit test main function, sets up hardware then jumps to test.*/
int main()
{
#if 1  /* Add later when platform supports this */
    /* Initialize standard SDK demo application pins */
    hardware_init();

    /* Call this function to initialize the console UART.  This function
       enables the use of STDIO functions (printf, scanf, etc.) */
    dbg_uart_init();
#endif

    if (spi_test() == kStatus_SPI_Success)
    {
        printf("\nSPI HAL tests passed \n");
    }
    else
    {
        printf("\n**One or more SPI HAL tests failed \n");
    }

    return 0;
}
void print_msg(uint32_t msgID)
{
    printf(s_spi_hal_test_error[msgID]);
}

/* ! @bhal hal api test.*/
spi_status_t spi_test()
{
    uint32_t instance, i;
    uint32_t bitsPerSec;
    uint32_t sourceClock;
    uint32_t failCount = 0;
    uint32_t prescaler, scaler;

    for(instance = 0; instance < SPI_INSTANCE_COUNT; instance++)
    {
    	/* Typecast printf arguement to unsigned int to work around KDS warning */
        printf("\r\n <Testing SPI%u>\n", (unsigned int)instance);

        SPI_Type *base = g_spiBase[instance];

        /* ungate the clocks to the spi module*/
        for (i = 0; i < SPI_INSTANCE_COUNT; i++)
        {
            CLOCK_SYS_EnableSpiClock(i);
        }

        sourceClock = CLOCK_SYS_GetSpiFreq(instance);
        /* Typecast printf arguement to unsigned int to work around KDS warning */
        printf("\r    SPI sourceClock = %u\n\r", (unsigned int)sourceClock);

        /*hal init test*/
        if(test_SPI_HAL_Init(base))
        {
            print_msg(HAL_INIT);
            failCount++;
        }

        /*hal enable test*/
        if(test_SPI_HAL_Enable(base))
        {
            print_msg(HAL_ENABLE);
            failCount++;
        }

        /*hal disable test*/
        if(test_SPI_HAL_Disable(base))
        {
            print_msg(HAL_DISABLE);
            failCount++;
        }

        /*hal set baud rate test*/
        /* test several values, ranging from 20kHz to 12MHz */
        for (bitsPerSec = 20000; bitsPerSec <= 12000000; bitsPerSec+=20000)
        {
            if(test_SPI_HAL_SetBaud(base, bitsPerSec, sourceClock))
            {
                print_msg(HAL_SET_BAUD);
                failCount++;
            }
        }

        /*hal set baud divisor test*/
        for (prescaler = 0; prescaler <= 7; prescaler++)
        {
            for (scaler = 0; scaler <= 8; scaler++)
            {
                if(test_SPI_HAL_SetBaudDivisors(base, prescaler, scaler))
                {
                    print_msg(HAL_SET_BAUD_DIVISOR);
                    failCount++;
                }
            }
        }

        /*hal set master and slave test*/
        if(test_SPI_HAL_SetMasterSlave(base, kSpiMaster))
        {
            print_msg(HAL_SET_MASTER_SLAVE);
            failCount++;
        }
        if(test_SPI_HAL_SetMasterSlave(base, kSpiSlave))
        {
            print_msg(HAL_SET_MASTER_SLAVE);
            failCount++;
        }

        /*hal set slave output mode test*/
        if(test_SPI_HAL_SetSlaveSelectOutputMode(base, kSpiSlaveSelect_AutomaticOutput))
        {
            print_msg(HAL_SET_SLAVE_OUTPUT_MODE);
            failCount++;
        }

        /*set output data format test*/
        if(test_SPI_HAL_SetDataFormat(base, kSpiClockPolarity_ActiveHigh, kSpiClockPhase_SecondEdge, kSpiLsbFirst))
        {
            print_msg(HAL_SET_DATA_FORMAT);
            failCount++;
        }

        /*hal set pin mode test*/
        if(test_SPI_HAL_SetPinMode(base, kSpiPinMode_Output))
        {
            print_msg(HAL_SET_PIN_MODE);
            failCount++;
        }

#if FSL_FEATURE_SPI_HAS_DMA_SUPPORT
        /*hal configure dma test*/
        if(test_SPI_HAL_ConfigureDma(base, true, true))
        {
            print_msg(HAL_CONFIGURE_DMA);
            failCount++;
        }
        if(test_SPI_HAL_ConfigureDma(base, false, false))
        {
            print_msg(HAL_CONFIGURE_DMA);
            failCount++;
        }
#endif

        /*hal configure stop and wait mode test*/
        if(test_SPI_HAL_ConfigureStopInWaitMode(base, true))
        {
            print_msg(HAL_SET_STOP_WAIT_MODE);
            failCount++;
        }
        /*hal set interrupt tests*/
        if(test_SPI_HAL_Enable_receive_and_fault_interrupt(base))
        {
            print_msg(HAL_ENABLE_RF_INTERRUPT);
            failCount++;
        }
        if(test_SPI_HAL_Disable_receive_and_fault_interrupt(base))
        {
            print_msg(HAL_DISABLE_RF_INTERRUPT);
            failCount++;
        }
        if(test_SPI_HAL_Enable_transmit_interrupt(base))
        {
            print_msg(HAL_ENABLE_TM_INTERRUPT);
            failCount++;
        }
        if(test_SPI_HAL_Disable_transmit_interrupt(base))
        {
            print_msg(HAL_DISABLE_TM_INTERRUPT);
            failCount++;
        }
        if(test_SPI_HAL_Enable_match_interrupt(base))
        {
            print_msg(HAL_ENABLE_MATCH_INTERRUPT);
            failCount++;
        }
        if(test_SPI_HAL_Disable_match_interrupt(base))
        {
            print_msg(HAL_DISABLE_MATCH_INTERRUPT);
            failCount++;
        }

        /*hal clear mode fault test*/
        if(test_SPI_HAL_ClearModeFault(base))
        {
            print_msg(HAL_CLEAR_MODE_FAULT);
            failCount++;
        }

        /*hal clear match test*/
        if(test_SPI_HAL_ClearMatch(base))
        {
            print_msg(HAL_CLEAR_MATCH);
            failCount++;
        }

        /*hal set match test*/
        if(test_SPI_HAL_SetMatchVal(base, 0xab))
        {
            print_msg(HAL_SET_MATCH);
            failCount++;
        }

#if FSL_FEATURE_SPI_16BIT_TRANSFERS
        /*hal 8- or 16-bit mode test*/
        if(test_SPI_HAL_Set8or16BitMode(base))
        {
            print_msg(HAL_SPI8_SPI16_MODE);
            failCount++;
        }
#endif

#if FSL_FEATURE_SPI_FIFO_SIZE
        /* For FIFO related hal functions, only test if FIFO is supported in instance */
        if (FSL_FEATURE_SPI_FIFO_SIZEn(instance) != 0)
        {
            printf("\r    FIFO features being tested \n\r");
            /*hal interrupt clearing mechanism mode test*/
            if(test_SPI_HAL_SetIntClearCmd(base))
            {
                print_msg(HAL_SPI_INTCLR);
                failCount++;
            }
            /*hal FIFO mode test*/
            if(test_SPI_HAL_SetFifoMode(base))
            {
                print_msg(HAL_SPI_FIFOMODE);
                failCount++;
            }
            /*hal FIFO interrupt mode test*/
            if(test_SPI_HAL_SetFifoIntCmd(base))
            {
                print_msg(HAL_SPI_FIFOINTMODE);
                failCount++;
            }
            /*hal FIFO get status flag test*/
            if(test_SPI_HAL_GetFifoStatusFlag(base))
            {
                print_msg(HAL_SPI_STATUSFLAG);
                failCount++;
            }

        }
#endif

    }

    if (failCount)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    else
    {
        return kStatus_SPI_Success;
    }
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

