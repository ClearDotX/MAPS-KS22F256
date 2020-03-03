#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsl_debug_console.h"
#include "fsl_crc_hal.h"
#include "fsl_device_registers.h"
#include "fsl_clock_manager.h"
#include <assert.h>
#include "board.h"

#define DEBUG_UART_BAUD (115200)

#define CRC_INSTANCE (0)

#define ERROR_DEBUG() printf("CRC HAL UNIT TEST ERROR: function[%s] line[%d]\r\n", __func__, __LINE__)

extern CRC_Type * const g_crcBase[CRC_INSTANCE_COUNT];

/* test write to 32-bit polynomial register */
int32_t CRC_HAL_test_SetPolyReg(CRC_Type * baseAddr, uint32_t value)
{
    CRC_HAL_SetPolyReg(baseAddr, value);
    if (CRC_RD_GPOLY(baseAddr) != value)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
};

/* test write to 16-bit polynomial register */
int32_t CRC_HAL_test_SetPolyHReg(CRC_Type * baseAddr, uint16_t value)
{
    CRC_HAL_SetPolyHReg(baseAddr, value);
    if (CRC_RD_GPOLYH(baseAddr) != value)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
};

/* test write to 16-bit polynomial register */
int32_t CRC_HAL_test_SetPolyLReg(CRC_Type * baseAddr, uint16_t value)
{
    CRC_HAL_SetPolyLReg(baseAddr, value);
    if (CRC_RD_GPOLYL(baseAddr) != value)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
};

/* test write to 32-bit control register */
int32_t CRC_HAL_test_SetCtrlReg(CRC_Type * baseAddr, uint32_t value)
{
    CRC_HAL_SetCtrlReg(baseAddr, value);
    if (CRC_RD_CTRL(baseAddr) != value)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
};

/* test set seed mode setting */
int32_t CRC_HAL_test_SetSeedOrDataMode(CRC_Type * baseAddr, bool enable)
{
    CRC_HAL_SetSeedOrDataMode(baseAddr, enable);
    if ( (bool)CRC_BRD_CTRL_WAS(baseAddr) != enable)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
};

/* test set input transpose setting */
int32_t CRC_HAL_test_SetWriteTranspose(CRC_Type * baseAddr, crc_transpose_t transp)
{
    CRC_HAL_SetWriteTranspose(baseAddr, transp);
    if ( (crc_transpose_t)CRC_BRD_CTRL_TOT(baseAddr) != transp)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
};

/* test set output transpose setting */
int32_t CRC_HAL_test_SetReadTranspose(CRC_Type * baseAddr, crc_transpose_t transp)
{
    CRC_HAL_SetReadTranspose(baseAddr, transp);
    if ( (crc_transpose_t)CRC_BRD_CTRL_TOTR(baseAddr) != transp)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
};

/* test Xor mode setting */
int32_t CRC_HAL_test_SetXorMode(CRC_Type * baseAddr, bool enable)
{
    CRC_HAL_SetXorMode(baseAddr, enable);
    if ( (bool)CRC_BRD_CTRL_FXOR(baseAddr) != enable)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
};

/* test protocol width setting */
int32_t CRC_HAL_test_SetProtocolWidth(CRC_Type * baseAddr, crc_prot_width_t width)
{
    CRC_HAL_SetProtocolWidth(baseAddr, width);
    if ( (crc_prot_width_t)CRC_BRD_CTRL_TCRC(baseAddr) != width)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
};

/* test CRC-CCITT calculation for 32-bit, 16-bit and 8-bit values */
int32_t CRC_HAL_test_CCITT(CRC_Type * baseAddr)
{
    uint32_t result;

    crc_prot_width_t crcWidth = kCrc16Bits;
    uint32_t seed = 0xFFFFU;
    uint32_t polynomial = 0x1021U;
    crc_transpose_t writeTranspose = kCrcNoTranspose;
    crc_transpose_t readTranspose = kCrcNoTranspose;
    bool complementRead = false;

    /* 1. set 16 or 32-bit crc width */
    CRC_HAL_SetProtocolWidth(baseAddr, crcWidth);

    /* 2. set transpose and complement options */
    CRC_HAL_SetWriteTranspose(baseAddr, writeTranspose);
    CRC_HAL_SetReadTranspose(baseAddr, readTranspose);
    CRC_HAL_SetXorMode(baseAddr, complementRead);

    /* 3. Write polynomial */
    CRC_HAL_SetPolyReg(baseAddr, polynomial);

    result = CRC_HAL_GetCrc32(baseAddr, 0x31323334, true, seed);
    if (result != 0x5349)
    {
        ERROR_DEBUG();
        return -1;
    }
    result = CRC_HAL_GetCrc16(baseAddr, 0x3536, false, seed);
    if (result != 0x2EF4)
    {
        ERROR_DEBUG();
        return -1;
    }
    result = CRC_HAL_GetCrc8(baseAddr, 0x37, false, seed);
    if (result != 0x7718)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
}

/* test CRC-KERMIT calculation for 32-bit, 16-bit and 8-bit values */
int32_t CRC_HAL_test_KERMIT(CRC_Type * baseAddr)
{
    uint32_t result;

    crc_prot_width_t crcWidth = kCrc16Bits;
    uint32_t seed = 0x0000U;
    uint32_t polynomial = 0x1021U;
    crc_transpose_t writeTranspose = kCrcTransposeBits;
    crc_transpose_t readTranspose = kCrcTransposeBits;
    bool complementRead = false;

    /* 1. set 16 or 32-bit crc width */
    CRC_HAL_SetProtocolWidth(baseAddr, crcWidth);

    /* 2. set transpose and complement options */
    CRC_HAL_SetWriteTranspose(baseAddr, writeTranspose);
    CRC_HAL_SetReadTranspose(baseAddr, readTranspose);
    CRC_HAL_SetXorMode(baseAddr, complementRead);

    /* 3. Write polynomial */
    CRC_HAL_SetPolyReg(baseAddr, polynomial);

    result = CRC_HAL_GetCrc32(baseAddr, 0x31323334, true, seed);
    if (result != 0x3288)
    {
        ERROR_DEBUG();
        return -1;
    }
    result = CRC_HAL_GetCrc16(baseAddr, 0x3536, false, seed);
    if (result != 0xFD11)
    {
        ERROR_DEBUG();
        return -1;
    }
    result = CRC_HAL_GetCrc8(baseAddr, 0x37, false, seed);
    if (result != 0x4769)
    {
        ERROR_DEBUG();
        return -1;
    }
    return 0;
}

void init_hardware(void)
{
  uint8_t i;

  /* enable clock for PORTs */
  for (i = 0; i < PORT_INSTANCE_COUNT; i++)
  {
    CLOCK_SYS_EnablePortClock(i);
  }

  /* Init board clock */
  BOARD_ClockInit();
  
  dbg_uart_init();

  CLOCK_SYS_EnableCrcClock(CRC_INSTANCE);   
}

int main(void)
{
    int32_t status = 0;
    init_hardware();
    hardware_init();

    uint16_t poly16[4] = {0xFFFF, 0x5A5A, 0xA5A5, 0x0000};
    uint32_t poly32[4] = {0xFFFFFFFF, 0x5A5A5A5A, 0xA5A5A5A5, 0x00000000};

    printf("\r\nCRC hal driver unit test start!\r\n");

    status += CRC_HAL_test_SetSeedOrDataMode(g_crcBase[CRC_INSTANCE], true);
    status += CRC_HAL_test_SetSeedOrDataMode(g_crcBase[CRC_INSTANCE], false);

    status += CRC_HAL_test_SetProtocolWidth(g_crcBase[CRC_INSTANCE], kCrc16Bits);
    status += CRC_HAL_test_SetProtocolWidth(g_crcBase[CRC_INSTANCE], kCrc32Bits);

    for (int i = 0; i < 4; i++)
    {
        status += CRC_HAL_test_SetPolyReg(g_crcBase[CRC_INSTANCE], poly32[i]);
        status += CRC_HAL_test_SetPolyHReg(g_crcBase[CRC_INSTANCE], poly16[i]);
        status += CRC_HAL_test_SetPolyLReg(g_crcBase[CRC_INSTANCE], poly16[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        /* transposition change check */
        status += CRC_HAL_test_SetWriteTranspose(g_crcBase[CRC_INSTANCE], (crc_transpose_t)i);
        status += CRC_HAL_test_SetReadTranspose(g_crcBase[CRC_INSTANCE], (crc_transpose_t)i);
    }

    status += CRC_HAL_test_SetXorMode(g_crcBase[CRC_INSTANCE], true);
    status += CRC_HAL_test_SetXorMode(g_crcBase[CRC_INSTANCE], false);

    status += CRC_HAL_test_CCITT(g_crcBase[CRC_INSTANCE]);
    status += CRC_HAL_test_KERMIT(g_crcBase[CRC_INSTANCE]);

    if (status == 0)
    {
        printf("\r\nCRC hal driver unit test Finished successfully!\r\n");
        return 0;
    }
    else
    {
        printf("\r\nCRC hal driver unit test Failed!\r\n");
        return 0;
    }
}

