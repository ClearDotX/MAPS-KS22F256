/* flexio_i2c_hal_test.c */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsl_flexio_hal.h"
#include "fsl_flexio_i2c_hal.h"
#include "fsl_clock_manager.h"
#include "board.h"
#include "fsl_device_registers.h"

#define I2C_SLAVE_DEV_ADDR    (0x1F)
#define I2C_WRITE_REG_ADDR    (0x0E)
#define I2C_READ_REG_ADDR     (0x01)
#define I2C_READ_WHO_AM_I     (0x0D)

static void hardware_init_flexio_i2c_pins(void);
static void hardware_init_flexio_clock_source(void);
static uint32_t FLEXIO_I2C_HAL_TEST_SendSingleByte(uint8_t regAddr, uint8_t dat);
static uint32_t FLEXIO_I2C_HAL_TEST_SendMultipleBytes(uint8_t regAddr, uint8_t *txDatPtr, uint32_t length);
static uint32_t FLEXIO_I2C_HAL_TEST_ReceiveSingleByte(uint8_t regAddr, uint8_t *rxDatPtr);
static uint32_t FLEXIO_I2C_HAL_TEST_ReceiveMultipleBytes(uint8_t regAddr, uint8_t *rxDatPtr, uint32_t length);

flexio_i2c_dev_t mFlexioI2cDevStruct =
{
    FLEXIO_BASE_PTR,
    7U, /* SDA. */
    5U, /* SCL. */
    {0U, 1U}, /* Shifters. */
    {0U, 1U}  /* Timers. */
};

flexio_i2c_master_config_t mFlexioI2cMasterConfigStruct =
{
    48000000UL,
    100000UL,
    0U /* xferWordCount. */ /* Dummy. */
};

#define I2C_TX_BUFFER_LEN    (6U)
#define I2C_RX_BUFFER_LEN    (6U)

uint8_t gI2cTxBuffer[I2C_TX_BUFFER_LEN] = {6U, 5U, 4U, 3U, 2U, 1U};
uint8_t gI2cRxBuffer[I2C_RX_BUFFER_LEN];

int main(void)
{
    uint32_t i;
    uint32_t errCounter = 0U;
    uint32_t totalErrCounter = 0U;

    hardware_init();
    dbg_uart_init();

    /* Configure the pin mux for flexio. */
    hardware_init_flexio_i2c_pins();

    printf("\r\n flexio_i2c HAL Test: Start...\r\n");

    /* Enable the clock for flexio. */
    hardware_init_flexio_clock_source();
    CLOCK_SYS_EnableFlexioClock(0U);

    /* Configure the global setting for flexio. */
    /* Reset the FlexIO hardware. */
    FLEXIO_HAL_SetFlexioEnableCmd(FLEXIO_BASE_PTR, false); /* Disable the Flexio first. */
    FLEXIO_HAL_SetSoftwareResetCmd(FLEXIO_BASE_PTR, true);
    FLEXIO_HAL_SetSoftwareResetCmd(FLEXIO_BASE_PTR, false);
    /* Initialize FlexIO. */
    FLEXIO_HAL_SetDozeModeCmd(FLEXIO_BASE_PTR, true);
    FLEXIO_HAL_SetDebugModeCmd(FLEXIO_BASE_PTR, true);
    FLEXIO_HAL_SetFastAccessCmd(FLEXIO_BASE_PTR, true);
    FLEXIO_HAL_SetFlexioEnableCmd(FLEXIO_BASE_PTR, true);

    for (i = 0U; i < 4U; i++)
    {
        errCounter = 0U;
        errCounter += FLEXIO_I2C_HAL_TEST_SendSingleByte(I2C_WRITE_REG_ADDR, gI2cTxBuffer[0]);
        errCounter += FLEXIO_I2C_HAL_TEST_SendMultipleBytes(I2C_WRITE_REG_ADDR, gI2cTxBuffer, I2C_TX_BUFFER_LEN);  
        errCounter += FLEXIO_I2C_HAL_TEST_ReceiveSingleByte(I2C_READ_WHO_AM_I, gI2cRxBuffer);
        errCounter += FLEXIO_I2C_HAL_TEST_ReceiveMultipleBytes(I2C_READ_REG_ADDR, gI2cRxBuffer, I2C_RX_BUFFER_LEN);
        totalErrCounter += errCounter;
    }

    printf("flexio_i2c HAL Test: End\r\n");

    if (0U == totalErrCounter)
    {
        printf("Succeed\r\n");
    }
    else
    {
        printf("Error\r\n");
    }

    return 0;
}


static void hardware_init_flexio_clock_source(void)
{
    uint32_t tmp = SIM->SOPT2 & ~SIM_SOPT2_FLEXIOSRC_MASK;
    tmp |= SIM_SOPT2_FLEXIOSRC(1U); /* IRC48M clock. */
    SIM->SOPT2 = tmp;

}
static void hardware_init_flexio_i2c_pins(void)
{
    PORTD->PCR[5] = PORT_PCR_MUX(6)
                  | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;  /* FlexIO D5 , Pull up resistor enable*/
    PORTD->PCR[7] = PORT_PCR_MUX(6)
                  | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;  /* FlexIO D7 , Pull up resistor enable*/
}

static uint32_t FLEXIO_I2C_HAL_TEST_SendSingleByte(uint8_t regAddr, uint8_t dat)
{
    uint32_t errCounter = 0U;
    uint32_t dummy;

    mFlexioI2cMasterConfigStruct.xferWordCount = 3U; /* DevAddr + RegAddr + Single byte. */
    FLEXIO_I2C_HAL_ConfigMaster(&mFlexioI2cDevStruct, &mFlexioI2cMasterConfigStruct);

    /* Send DevAddr.*/
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, (I2C_SLAVE_DEV_ADDR << 1U) );
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);

    /* Send RegAddr */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, regAddr);
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);

    /* Send single data. */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, dat);
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);

    /* Send STOP condition. */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, 0x00000000);

    dummy = dummy; /* Avoid warning. */

    return errCounter;
}

static uint32_t FLEXIO_I2C_HAL_TEST_SendMultipleBytes(uint8_t regAddr, uint8_t *txDatPtr, uint32_t length)
{
    uint32_t errCounter = 0U;
    uint32_t dummy;
    uint32_t i;

    mFlexioI2cMasterConfigStruct.xferWordCount = 2U + length; /* DevAddr + RegAddr + multiple bytes. */
    FLEXIO_I2C_HAL_ConfigMaster(&mFlexioI2cDevStruct, &mFlexioI2cMasterConfigStruct);

    /* Send DevAddr.*/
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, (I2C_SLAVE_DEV_ADDR << 1U));
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);
    
    /* Send RegAddr */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, regAddr);
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);

    /* Send multiple bytes. */
    for (i = 0U; i < length; i++)
    {
        FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, (*txDatPtr++));
        dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);
    }

    /* Send STOP condition. */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, 0x00000000);

    dummy = dummy; /* Avoid warning. */
    return errCounter;
}

static uint32_t FLEXIO_I2C_HAL_TEST_ReceiveSingleByte(uint8_t regAddr, uint8_t *rxDatPtr)
{
    uint32_t errCounter = 0U;
    uint32_t dummy;

    /* The first frame. */
    mFlexioI2cMasterConfigStruct.xferWordCount = 2U; /* DevAddr + RegAddr. */
    FLEXIO_I2C_HAL_ConfigMaster(&mFlexioI2cDevStruct, &mFlexioI2cMasterConfigStruct);

    /* Send DevAddr.*/
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, (I2C_SLAVE_DEV_ADDR << 1U));
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);
    
    /* Send RegAddr */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, regAddr);
    dummy = FLEXIO_I2C_HAL_GetData(&mFlexioI2cDevStruct);

    /* Prepare for RESTART condition, no stop.*/
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, 0xFFFFFFFF);
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);

    /* The second frame. */
    mFlexioI2cMasterConfigStruct.xferWordCount = 2U; /* DevAddr|ReadCmd + single byte received. */
    FLEXIO_I2C_HAL_ConfigMaster(&mFlexioI2cDevStruct, &mFlexioI2cMasterConfigStruct);

    /* Generate RESTART condition + Send DevAddr|ReadCmd. */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, (I2C_SLAVE_DEV_ADDR << 1U) );
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);

    /* Generate NACK after the last byte. */
    /* Send single data. */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, 0xFFFFFFFF);
    *rxDatPtr = (uint8_t)((0xFF000000 & FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct))>>24U );

    /* Send STOP condition. */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, 0x00000000);

    dummy = dummy; /* Avoid warning. */
    return errCounter;
}

static uint32_t FLEXIO_I2C_HAL_TEST_ReceiveMultipleBytes(uint8_t regAddr, uint8_t *rxDatPtr, uint32_t length)
{
    uint32_t errCounter = 0U;
    uint32_t dummy;
    uint32_t i;

    /* The first frame. */
    mFlexioI2cMasterConfigStruct.xferWordCount = 2U; /* DevAddr + RegAddr. */
    FLEXIO_I2C_HAL_ConfigMaster(&mFlexioI2cDevStruct, &mFlexioI2cMasterConfigStruct);

    /* Send DevAddr.*/
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, (I2C_SLAVE_DEV_ADDR << 1U) );
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);

    /* Send RegAddr */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, regAddr);
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);

    /* Prepare for RESTART condition, no stop.*/
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, 0xFFFFFFFF);
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);

    /* The second frame. */
    mFlexioI2cMasterConfigStruct.xferWordCount = 1U + length; /* DevAddr|ReadCmd + single byte received. */
    FLEXIO_I2C_HAL_ConfigMaster(&mFlexioI2cDevStruct, &mFlexioI2cMasterConfigStruct);

    /* Generate RESTART condition + Send DevAddr|ReadCmd. */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, ((I2C_SLAVE_DEV_ADDR << 1U) | 1U) );
    dummy = FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct);

    /* Master generates ACK after no last byte when receving data. */
    FLEXIO_I2C_HAL_ConfigSendAck(&mFlexioI2cDevStruct);
    for (i = 0U; i < length-1U; i++)
    {
        /* Get single data. */
        FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, 0xFFFFFFFF);
        *rxDatPtr++ = (uint8_t)((0xFF000000 & FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct))>>24U);
    }

    /* Generate NACK after the last byte. */
    FLEXIO_I2C_HAL_ConfigSendNAck(&mFlexioI2cDevStruct);
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, 0xFFFFFFFF);
    *rxDatPtr = (uint8_t)((0xFF000000 & FLEXIO_I2C_HAL_GetDataPolling(&mFlexioI2cDevStruct))>>24U);

    /* Send STOP condition. */
    FLEXIO_I2C_HAL_PutDataPolling(&mFlexioI2cDevStruct, 0x00000000);

    dummy = dummy; /* Avoid warning. */
    return errCounter;
}


/* EOF. */


