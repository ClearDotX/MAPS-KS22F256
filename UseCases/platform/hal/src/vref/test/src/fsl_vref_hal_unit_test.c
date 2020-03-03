#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsl_debug_console.h"
#include "fsl_vref_hal.h"
#include "fsl_device_registers.h"
#include <assert.h>

#define DEBUG_UART_BAUD (115200)

#define ERROR_DEBUG() printf("VREF HAL UNIT TEST ERROR: function[%s] line[%d]\r\n", __func__, __LINE__)

/*! @name VREF related feature APIs*/

/* TEST EnableVref */
static inline int VREF_HAL_test_SetVrefCmd(uint32_t baseAddr, bool enable)
{
    VREF_HAL_SetVrefCmd(baseAddr, enable);
    if ( (bool)BR_VREF_SC_VREFEN(baseAddr) != enable)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
}

/* TEST EnableInternalOscillator */
static inline int VREF_HAL_test_SetInternalRegulatorCmd(uint32_t baseAddr, bool enable)
{
    VREF_HAL_SetInternalRegulatorCmd(baseAddr, enable);
    if ( (bool)BR_VREF_SC_REGEN(baseAddr) != enable)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
}

/* TEST SetTrimValue */
static inline int VREF_HAL_test_SetTrimVal(uint32_t baseAddr, uint8_t trimValue)
{
    VREF_HAL_SetTrimVal(baseAddr, trimValue);
    if (BR_VREF_TRM_TRIM(baseAddr) != trimValue)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
}

/* TEST SetMode */
static inline int VREF_HAL_test_SetBuffMode(uint32_t baseAddr, vref_mode_t mode)
{
    VREF_HAL_SetBuffMode(baseAddr, mode);
    if ( (vref_mode_t)BR_VREF_SC_MODE_LV(baseAddr) != mode)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
}

#if FSL_FEATURE_VREF_HAS_COMPENSATION

/* TEST EnableSecondOrderCurvatureCompensation */
static inline int VREF_HAL_test_SetSecondOrderCurvatureCompensationCmd(uint32_t baseAddr, bool enable)
{
    VREF_HAL_SetSecondOrderCurvatureCompensationCmd(baseAddr, enable);
    if ( (bool)BR_VREF_SC_ICOMPEN(baseAddr) != enable)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
}

#endif

/* TEST EnableChopOscillator */
#if FSL_FEATURE_VREF_HAS_CHOP_OSC
static inline int VREF_HAL_test_SetChopOscillatorCmd(uint32_t baseAddr, bool enable)
{
    VREF_HAL_SetChopOscillatorCmd(baseAddr, enable);
    if ( (bool)BR_VREF_TRM_CHOPEN(baseAddr) != enable)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
}
#endif

/* hardware initialization */
void init_hardware(void)
{
#if defined(K22F51212_SERIES)
    SIM->SCGC5 |= ( SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK);

    BW_PORT_PCRn_MUX(PORTE_BASE, 1, 3);
    BW_PORT_PCRn_MUX(PORTE_BASE, 0, 3);

    DbgConsole_Init(1, DEBUG_UART_BAUD, kDebugConsoleUART);

#elif defined(FRDM_K64F)
    SIM->SCGC5 |= ( SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK);

    BW_PORT_PCRn_MUX(PORTB_BASE, 17, 3);
    BW_PORT_PCRn_MUX(PORTB_BASE, 16, 3);

    /* Init uart driver for stdio.*/
    DbgConsole_Init(0, DEBUG_UART_BAUD, kDebugConsoleUART);

#elif defined(TWR_K64F120M)
    SIM->SCGC5 |= ( SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK);

    BW_PORT_PCRn_MUX(PORTC_BASE, 4, 3);
    BW_PORT_PCRn_MUX(PORTC_BASE, 3, 3);

    /* Init uart driver for stdio.*/
    DbgConsole_Init(1, DEBUG_UART_BAUD, kDebugConsoleUART);

#elif defined(TWR_KV31F120M)
    SIM->SCGC5 |= ( SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK);

    BW_PORT_PCRn_MUX(PORTB_BASE, 17, 3);
    BW_PORT_PCRn_MUX(PORTB_BASE, 16, 3);

    /* Init uart driver for stdio.*/
    DbgConsole_Init(0, DEBUG_UART_BAUD, kDebugConsoleUART);

#endif
}

void main(void)
{
    int32_t status = 0;
    init_hardware();

    uint32_t baseAddr = VREF_BASE;
    bool enable = true;
    vref_mode_t mode = kMode_VREF_BandgapOnly;
    uint8_t trimValue = 0x20;

    uint32_t value = 0;

    printf("\r\nVREF HAL driver unit test start!\r\n");

    status += VREF_HAL_test_SetVrefCmd(baseAddr, enable);
    status += VREF_HAL_test_SetInternalRegulatorCmd(baseAddr, enable);
    status += VREF_HAL_test_SetTrimVal(baseAddr, trimValue);
    status += VREF_HAL_test_SetBuffMode(baseAddr, mode);

#if FSL_FEATURE_VREF_HAS_COMPENSATION
    status += VREF_HAL_test_SetSecondOrderCurvatureCompensationCmd(baseAddr, enable);
#endif

#if FSL_FEATURE_VREF_HAS_CHOP_OSC
    status += VREF_HAL_test_SetChopOscillatorCmd(baseAddr, enable);
#endif

    if (status == 0)
    {
        printf("\r\nVREF HAL driver unit test Finished successfully!\r\n");
    }
    else
    {
        printf("\r\nVREF HAL driver unit test Failed!\r\n");
    }
}

/*******************************************************************************
* EOF
*******************************************************************************/

