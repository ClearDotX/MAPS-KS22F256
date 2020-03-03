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

#include "fsl_gpio_hal_test.h"
#include "stdio.h"


/* This is ISR function which will toggle the LED status */
bool gpio_hal_coverage_test(void);

/* This is ISR function which will toggle the LED status */
void gpio_irq_callback(void);

/* Short delay */
void delay(int loops);


uint32_t isr_counter = 0;

int main(void)
{
    bool bRet = true;
    
    hardware_init();
    dbg_uart_init();
    
    printf("\r\nGPIO HAL Test: Start...\r\n");
    
    /**********************************************
     * GPIO HAL API coverage test 
     *********************************************/
    if(!gpio_hal_coverage_test())
    {
        bRet = false;
    }

    /**********************************************
     * PORT Initialization
     *********************************************/

    /* Active input port as interrupt on rising edge.*/
    PORT_HAL_SetPinIntMode(InputPortBaseAddr, InputPin, kPortIntRisingEdge);
    /* Set input port as gpio.*/
    PORT_HAL_SetMuxMode(InputPortBaseAddr, InputPin, kPortMuxAsGpio);
    
    /* Set output as gpio.*/
    PORT_HAL_SetMuxMode(OutputPortBaseAddr, OutputPin, kPortMuxAsGpio);
    /* Enable output port drive strength.*/
    PORT_HAL_SetDriveStrengthMode(OutputPortBaseAddr, OutputPin, kPortHighDriveStrength);
    
    /* Set BOARD_GPIO_LED_RED as gpio.*/
    PORT_HAL_SetMuxMode(LEDPortBaseAddr, LEDPin, kPortMuxAsGpio);
    /* Enable BOARD_GPIO_LED_RED drive strength.*/
    PORT_HAL_SetDriveStrengthMode(LEDPortBaseAddr, LEDPin, kPortHighDriveStrength);

    
    /**********************************************
     * GPIO Initialization
     *********************************************/
    
    /* Set GPIO direction.*/
    GPIO_HAL_SetPinDir(OutputGpioBaseAddr, OutputPin, kGpioDigitalOutput);
    GPIO_HAL_SetPinDir(LEDGpioBaseAddr, LEDPin, kGpioDigitalOutput);
    GPIO_HAL_SetPinDir(InputGpioBaseAddr, InputPin, kGpioDigitalInput);

    /* Set GPIO output*/
    GPIO_HAL_ClearPinOutput(OutputGpioBaseAddr, OutputPin);
    GPIO_HAL_ClearPinOutput(LEDGpioBaseAddr, LEDPin);
     
    /**********************************************
     * Enable PORTC Interruption
     *********************************************/

    INT_SYS_InstallHandler(Input_IRQn, gpio_irq_callback);
    INT_SYS_EnableIRQ(Input_IRQn);
    
    for (int i=0; i<5; ++i)
    {
        /* Create a rising edge.*/
        GPIO_HAL_ClearPinOutput(OutputGpioBaseAddr, OutputPin);      
        GPIO_HAL_SetPinOutput(OutputGpioBaseAddr, OutputPin);
        
        /* Short Delay*/
        delay(100);       
    }
    
    if (isr_counter != 5) 
    {
        bRet = false;
    }
    if (bRet) 
    {
        printf("GPIO HAL test success!");
    } 
    else 
    {
        printf("GPIO HAL test failed!");
    }
		
		return 1;
}

/* Coverage test: test all the GPIO HAL APIs. */
bool gpio_hal_coverage_test()
{
    bool bRet = true;
    
    /* Set input port and output port as gpio.*/
    PORT_HAL_SetMuxMode(InputPortBaseAddr, InputPin, kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(OutputPortBaseAddr, OutputPin, kPortMuxAsGpio);

    /* GPIO cover tests*/
    GPIO_HAL_SetPinDir(InputGpioBaseAddr, InputPin, kGpioDigitalInput);
    if ( kGpioDigitalInput != GPIO_HAL_GetPinDir(InputGpioBaseAddr, InputPin) )
    {
        bRet = false;
    }
    
    GPIO_HAL_SetPortDir(InputGpioBaseAddr, 0);
    if ( 0 != GPIO_HAL_GetPortDir(InputGpioBaseAddr))
    {
        bRet = false;
    }
    
    GPIO_HAL_SetPinDir(OutputGpioBaseAddr, OutputPin, kGpioDigitalOutput);
    GPIO_HAL_WritePinOutput(OutputGpioBaseAddr, OutputPin, 1);
    if (!(0x01 & (GPIO_HAL_ReadPortOutput(OutputGpioBaseAddr) >> OutputPin)))
    {
        bRet = false;
    }
    
    GPIO_HAL_TogglePinOutput(OutputGpioBaseAddr, OutputPin);
    if (0x01 & (GPIO_HAL_ReadPortOutput(OutputGpioBaseAddr) >> OutputPin))
    {
        bRet = false;
    }
    
    if (GPIO_HAL_ReadPinInput(InputGpioBaseAddr, InputPin))
    {
        bRet = false;
    }
    
    if ((1<<InputPin) & GPIO_HAL_ReadPortInput(InputGpioBaseAddr))
    {
        bRet = false;
    }
    
    /* FGPIO cover tests */
#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
    
    FGPIO_HAL_SetPinDir(InputFGpioBaseAddr, InputPin, kGpioDigitalInput);
    if ( kGpioDigitalInput != FGPIO_HAL_GetPinDir(InputFGpioBaseAddr, InputPin) )
    {
        bRet = false;
    }
    
    FGPIO_HAL_SetPortDir(InputFGpioBaseAddr, 0);
    if ( 0 != FGPIO_HAL_GetPortDir(InputFGpioBaseAddr))
    {
        bRet = false;
    }
    
    FGPIO_HAL_SetPinDir(OutputFGpioBaseAddr, OutputPin, kGpioDigitalOutput);
    FGPIO_HAL_WritePinOutput(OutputFGpioBaseAddr, OutputPin, 1);
    if (!(0x01 & (FGPIO_HAL_ReadPortOutput(OutputFGpioBaseAddr) >> OutputPin)))
    {
        bRet = false;
    }
    
    FGPIO_HAL_TogglePinOutput(OutputFGpioBaseAddr, OutputPin);
    if (0x01 & (FGPIO_HAL_ReadPortOutput(OutputFGpioBaseAddr) >> OutputPin))
    {
        bRet = false;
    }
    
    if (FGPIO_HAL_ReadPinInput(InputFGpioBaseAddr, InputPin))
    {
        bRet = false;
    }
    
    if ((1<<InputPin) & FGPIO_HAL_ReadPortInput(InputFGpioBaseAddr))
    {
        bRet = false;
    }
#endif
    
    return bRet;
}

/* This is ISR function which will toggle the LED status */
void gpio_irq_callback(void)
{
    if(PORT_HAL_IsPinIntPending(InputPortBaseAddr, InputPin)) {
        /* Toggle on BOARD LED*/
        GPIO_HAL_TogglePinOutput(LEDGpioBaseAddr, LEDPin);

        /* Clear interrupt flag*/
        PORT_HAL_ClearPinIntFlag(InputPortBaseAddr, InputPin);
        
        isr_counter++;
    }
}

/* Short delay */
void delay(int loops)
{
	  /* Prevent from function optimization */
    volatile int i;
    int periods = 100000;
    for(i=0; i<loops*periods; ++i)
    {
    }
}
