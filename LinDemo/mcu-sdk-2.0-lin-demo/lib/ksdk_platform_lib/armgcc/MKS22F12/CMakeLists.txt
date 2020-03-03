INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

 
# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})


SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MKS22FN256VLL12")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MKS22FN256VLL12")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

include_directories(${ProjDirPath}/../../../../platform/CMSIS/Include)

include_directories(${ProjDirPath}/../../../../platform/devices)

include_directories(${ProjDirPath}/../../../../platform/devices/MKS22F12/include)

include_directories(${ProjDirPath}/../../../../platform/devices/MKS22F12/startup)

include_directories(${ProjDirPath}/../../../../platform/drivers)

include_directories(${ProjDirPath}/../../../../platform/drivers/common)

include_directories(${ProjDirPath}/../../../../platform/drivers/wdog)

include_directories(${ProjDirPath}/../../../../platform/drivers/uart)

include_directories(${ProjDirPath}/../../../../platform/drivers/uart)

include_directories(${ProjDirPath}/../../../../platform/drivers/tpm)

include_directories(${ProjDirPath}/../../../../middleware/sdmmc/include)

include_directories(${ProjDirPath}/../../../../middleware/sdmmc/src)

include_directories(${ProjDirPath}/../../../../platform/drivers/smc)

include_directories(${ProjDirPath}/../../../../platform/drivers/sim)

include_directories(${ProjDirPath}/../../../../platform/drivers/sai)

include_directories(${ProjDirPath}/../../../../platform/drivers/sai)

include_directories(${ProjDirPath}/../../../../platform/drivers/rtc)

include_directories(${ProjDirPath}/../../../../platform/drivers/rnga)

include_directories(${ProjDirPath}/../../../../platform/drivers/include)

include_directories(${ProjDirPath}/../../../../platform/drivers/rcm)

include_directories(${ProjDirPath}/../../../../platform/drivers/port)

include_directories(${ProjDirPath}/../../../../platform/drivers/pmc)

include_directories(${ProjDirPath}/../../../../platform/drivers/pit)

include_directories(${ProjDirPath}/../../../../platform/drivers/pdb)

include_directories(${ProjDirPath}/../../../../platform/drivers/osc)

include_directories(${ProjDirPath}/../../../../platform/drivers/mcg)

include_directories(${ProjDirPath}/../../../../platform/drivers/lpuart)

include_directories(${ProjDirPath}/../../../../platform/drivers/lpuart)

include_directories(${ProjDirPath}/../../../../platform/drivers/lpi2c)

include_directories(${ProjDirPath}/../../../../platform/drivers/lpi2c)

include_directories(${ProjDirPath}/../../../../platform/drivers/lptmr)

include_directories(${ProjDirPath}/../../../../platform/drivers/llwu)

include_directories(${ProjDirPath}/../../../../platform/drivers/gpio)

include_directories(${ProjDirPath}/../../../../platform/drivers/flexio)

include_directories(${ProjDirPath}/../../../../platform/drivers/flexcan)

include_directories(${ProjDirPath}/../../../../platform/drivers/flexcan)

include_directories(${ProjDirPath}/../../../../platform/flash)

include_directories(${ProjDirPath}/../../../../platform/drivers/ewm)

include_directories(${ProjDirPath}/../../../../platform/drivers/common)

include_directories(${ProjDirPath}/../../../../platform/drivers/edma)

include_directories(${ProjDirPath}/../../../../platform/drivers/dspi)

include_directories(${ProjDirPath}/../../../../platform/drivers/dspi)

include_directories(${ProjDirPath}/../../../../platform/drivers/dmamux)

include_directories(${ProjDirPath}/../../../../platform/drivers/dac)

include_directories(${ProjDirPath}/../../../../platform/drivers/crc)

include_directories(${ProjDirPath}/../../../../platform/drivers/common)

include_directories(${ProjDirPath}/../../../../platform/drivers/cmp)

include_directories(${ProjDirPath}/../../../../platform/devices/MKS22F12)

include_directories(${ProjDirPath}/../../../../platform/drivers/adc16)

include_directories(${ProjDirPath}/../../../../platform/CMSIS/Include)

include_directories(${ProjDirPath}/../../../../platform/devices)

include_directories(${ProjDirPath}/../../../../platform/devices/MKS22F12/include)

include_directories(${ProjDirPath}/../../../../platform/devices/MKS22F12/startup)

include_directories(${ProjDirPath}/../../../../platform/drivers)

include_directories(${ProjDirPath}/../../../../platform/drivers/common)

include_directories(${ProjDirPath}/../../../../platform/drivers/wdog)

include_directories(${ProjDirPath}/../../../../platform/drivers/uart)

include_directories(${ProjDirPath}/../../../../platform/drivers/uart)

include_directories(${ProjDirPath}/../../../../platform/drivers/tpm)

include_directories(${ProjDirPath}/../../../../middleware/sdmmc/include)

include_directories(${ProjDirPath}/../../../../middleware/sdmmc/src)

include_directories(${ProjDirPath}/../../../../platform/drivers/smc)

include_directories(${ProjDirPath}/../../../../platform/drivers/sim)

include_directories(${ProjDirPath}/../../../../platform/drivers/sai)

include_directories(${ProjDirPath}/../../../../platform/drivers/sai)

include_directories(${ProjDirPath}/../../../../platform/drivers/rtc)

include_directories(${ProjDirPath}/../../../../platform/drivers/rnga)

include_directories(${ProjDirPath}/../../../../platform/drivers/include)

include_directories(${ProjDirPath}/../../../../platform/drivers/rcm)

include_directories(${ProjDirPath}/../../../../platform/drivers/port)

include_directories(${ProjDirPath}/../../../../platform/drivers/pmc)

include_directories(${ProjDirPath}/../../../../platform/drivers/pit)

include_directories(${ProjDirPath}/../../../../platform/drivers/pdb)

include_directories(${ProjDirPath}/../../../../platform/drivers/osc)

include_directories(${ProjDirPath}/../../../../platform/drivers/mcg)

include_directories(${ProjDirPath}/../../../../platform/drivers/lpuart)

include_directories(${ProjDirPath}/../../../../platform/drivers/lpuart)

include_directories(${ProjDirPath}/../../../../platform/drivers/lpi2c)

include_directories(${ProjDirPath}/../../../../platform/drivers/lpi2c)

include_directories(${ProjDirPath}/../../../../platform/drivers/lptmr)

include_directories(${ProjDirPath}/../../../../platform/drivers/llwu)

include_directories(${ProjDirPath}/../../../../platform/drivers/gpio)

include_directories(${ProjDirPath}/../../../../platform/drivers/flexio)

include_directories(${ProjDirPath}/../../../../platform/drivers/flexcan)

include_directories(${ProjDirPath}/../../../../platform/drivers/flexcan)

include_directories(${ProjDirPath}/../../../../platform/flash)

include_directories(${ProjDirPath}/../../../../platform/drivers/ewm)

include_directories(${ProjDirPath}/../../../../platform/drivers/common)

include_directories(${ProjDirPath}/../../../../platform/drivers/edma)

include_directories(${ProjDirPath}/../../../../platform/drivers/dspi)

include_directories(${ProjDirPath}/../../../../platform/drivers/dspi)

include_directories(${ProjDirPath}/../../../../platform/drivers/dmamux)

include_directories(${ProjDirPath}/../../../../platform/drivers/dac)

include_directories(${ProjDirPath}/../../../../platform/drivers/crc)

include_directories(${ProjDirPath}/../../../../platform/drivers/common)

include_directories(${ProjDirPath}/../../../../platform/drivers/cmp)

include_directories(${ProjDirPath}/../../../../platform/devices/MKS22F12)

include_directories(${ProjDirPath}/../../../../platform/drivers/adc16)

add_library(libksdk_platform.a STATIC
"${ProjDirPath}/../../../../platform/drivers/wdog/fsl_wdog.h"
"${ProjDirPath}/../../../../platform/drivers/wdog/fsl_wdog.c"
"${ProjDirPath}/../../../../platform/drivers/uart/fsl_uart_edma.h"
"${ProjDirPath}/../../../../platform/drivers/uart/fsl_uart_edma.c"
"${ProjDirPath}/../../../../platform/drivers/uart/fsl_uart.h"
"${ProjDirPath}/../../../../platform/drivers/uart/fsl_uart.c"
"${ProjDirPath}/../../../../platform/drivers/tpm/fsl_tpm.c"
"${ProjDirPath}/../../../../platform/drivers/tpm/fsl_tpm.h"
"${ProjDirPath}/../../../../middleware/sdmmc/include/fsl_sdspi.h"
"${ProjDirPath}/../../../../middleware/sdmmc/include/fsl_specification.h"
"${ProjDirPath}/../../../../middleware/sdmmc/src/fsl_sdspi.c"
"${ProjDirPath}/../../../../platform/drivers/smc/fsl_smc.h"
"${ProjDirPath}/../../../../platform/drivers/smc/fsl_smc.c"
"${ProjDirPath}/../../../../platform/drivers/sim/fsl_sim.c"
"${ProjDirPath}/../../../../platform/drivers/sai/fsl_sai_edma.h"
"${ProjDirPath}/../../../../platform/drivers/sai/fsl_sai_edma.c"
"${ProjDirPath}/../../../../platform/drivers/sai/fsl_sai.h"
"${ProjDirPath}/../../../../platform/drivers/sai/fsl_sai.c"
"${ProjDirPath}/../../../../platform/drivers/rtc/fsl_rtc.c"
"${ProjDirPath}/../../../../platform/drivers/rtc/fsl_rtc.h"
"${ProjDirPath}/../../../../platform/drivers/rnga/fsl_rnga.c"
"${ProjDirPath}/../../../../platform/drivers/rnga/fsl_rnga.h"
"${ProjDirPath}/../../../../platform/drivers/rcm/fsl_rcm.c"
"${ProjDirPath}/../../../../platform/drivers/rcm/fsl_rcm.h"
"${ProjDirPath}/../../../../platform/drivers/port/fsl_port.h"
"${ProjDirPath}/../../../../platform/drivers/port/fsl_port.c"
"${ProjDirPath}/../../../../platform/drivers/pmc/fsl_pmc.h"
"${ProjDirPath}/../../../../platform/drivers/pmc/fsl_pmc.c"
"${ProjDirPath}/../../../../platform/drivers/pit/fsl_pit.c"
"${ProjDirPath}/../../../../platform/drivers/pit/fsl_pit.h"
"${ProjDirPath}/../../../../platform/drivers/pdb/fsl_pdb.c"
"${ProjDirPath}/../../../../platform/drivers/pdb/fsl_pdb.h"
"${ProjDirPath}/../../../../platform/drivers/osc/fsl_osc.h"
"${ProjDirPath}/../../../../platform/drivers/mcg/fsl_mcg.c"
"${ProjDirPath}/../../../../platform/drivers/lpuart/fsl_lpuart_edma.h"
"${ProjDirPath}/../../../../platform/drivers/lpuart/fsl_lpuart_edma.c"
"${ProjDirPath}/../../../../platform/drivers/lpuart/fsl_lpuart.h"
"${ProjDirPath}/../../../../platform/drivers/lpuart/fsl_lpuart.c"
"${ProjDirPath}/../../../../platform/drivers/lpi2c/fsl_lpi2c_edma.c"
"${ProjDirPath}/../../../../platform/drivers/lpi2c/fsl_lpi2c_edma.h"
"${ProjDirPath}/../../../../platform/drivers/lpi2c/fsl_lpi2c.c"
"${ProjDirPath}/../../../../platform/drivers/lpi2c/fsl_lpi2c.h"
"${ProjDirPath}/../../../../platform/drivers/lptmr/fsl_lptmr.c"
"${ProjDirPath}/../../../../platform/drivers/lptmr/fsl_lptmr.h"
"${ProjDirPath}/../../../../platform/drivers/llwu/fsl_llwu.h"
"${ProjDirPath}/../../../../platform/drivers/llwu/fsl_llwu.c"
"${ProjDirPath}/../../../../platform/drivers/gpio/fsl_gpio.h"
"${ProjDirPath}/../../../../platform/drivers/gpio/fsl_gpio.c"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_i2c_master.c"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_i2c_master.h"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_spi_edma.c"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_spi_edma.h"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_spi.c"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_spi.h"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_uart_edma.c"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_uart_edma.h"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_uart.c"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_uart.h"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_i2s_edma.c"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_i2s_edma.h"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_i2s.c"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio_i2s.h"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio.c"
"${ProjDirPath}/../../../../platform/drivers/flexio/fsl_flexio.h"
"${ProjDirPath}/../../../../platform/drivers/flexcan/fsl_flexcan_edma.h"
"${ProjDirPath}/../../../../platform/drivers/flexcan/fsl_flexcan_edma.c"
"${ProjDirPath}/../../../../platform/drivers/flexcan/fsl_flexcan.h"
"${ProjDirPath}/../../../../platform/drivers/flexcan/fsl_flexcan.c"
"${ProjDirPath}/../../../../platform/flash/fsl_flash.h"
"${ProjDirPath}/../../../../platform/flash/SSD_FTFx_internal.c"
"${ProjDirPath}/../../../../platform/flash/flash_cache_clear.c"
"${ProjDirPath}/../../../../platform/flash/flash_command_sequence.c"
"${ProjDirPath}/../../../../platform/flash/flash_erase.c"
"${ProjDirPath}/../../../../platform/flash/flash_erase_all.c"
"${ProjDirPath}/../../../../platform/flash/flash_erase_all_execute_only_segments.c"
"${ProjDirPath}/../../../../platform/flash/flash_erase_all_unsecure.c"
"${ProjDirPath}/../../../../platform/flash/flash_get_property.c"
"${ProjDirPath}/../../../../platform/flash/flash_get_security_state.c"
"${ProjDirPath}/../../../../platform/flash/flash_init.c"
"${ProjDirPath}/../../../../platform/flash/flash_is_execute_only.c"
"${ProjDirPath}/../../../../platform/flash/flash_is_protected.c"
"${ProjDirPath}/../../../../platform/flash/flash_program.c"
"${ProjDirPath}/../../../../platform/flash/flash_program_once.c"
"${ProjDirPath}/../../../../platform/flash/flash_program_partition.c"
"${ProjDirPath}/../../../../platform/flash/flash_program_section.c"
"${ProjDirPath}/../../../../platform/flash/flash_protection.c"
"${ProjDirPath}/../../../../platform/flash/flash_read_once.c"
"${ProjDirPath}/../../../../platform/flash/flash_read_resource.c"
"${ProjDirPath}/../../../../platform/flash/flash_security_bypass.c"
"${ProjDirPath}/../../../../platform/flash/flash_set_flexram_function.c"
"${ProjDirPath}/../../../../platform/flash/flash_swap_control.c"
"${ProjDirPath}/../../../../platform/flash/flash_verify_erase.c"
"${ProjDirPath}/../../../../platform/flash/flash_verify_erase_all.c"
"${ProjDirPath}/../../../../platform/flash/flash_verify_erase_all_execute_only_segments.c"
"${ProjDirPath}/../../../../platform/flash/flash_verify_program.c"
"${ProjDirPath}/../../../../platform/drivers/ewm/fsl_ewm.c"
"${ProjDirPath}/../../../../platform/drivers/ewm/fsl_ewm.h"
"${ProjDirPath}/../../../../platform/drivers/edma/fsl_edma.h"
"${ProjDirPath}/../../../../platform/drivers/edma/fsl_edma.c"
"${ProjDirPath}/../../../../platform/drivers/dspi/fsl_dspi_edma.h"
"${ProjDirPath}/../../../../platform/drivers/dspi/fsl_dspi_edma.c"
"${ProjDirPath}/../../../../platform/drivers/dspi/fsl_dspi.h"
"${ProjDirPath}/../../../../platform/drivers/dspi/fsl_dspi.c"
"${ProjDirPath}/../../../../platform/drivers/dmamux/fsl_dmamux.h"
"${ProjDirPath}/../../../../platform/drivers/dmamux/fsl_dmamux.c"
"${ProjDirPath}/../../../../platform/drivers/dac/fsl_dac.c"
"${ProjDirPath}/../../../../platform/drivers/dac/fsl_dac.h"
"${ProjDirPath}/../../../../platform/drivers/crc/fsl_crc.c"
"${ProjDirPath}/../../../../platform/drivers/crc/fsl_crc.h"
"${ProjDirPath}/../../../../platform/drivers/cmp/fsl_cmp.c"
"${ProjDirPath}/../../../../platform/drivers/cmp/fsl_cmp.h"
"${ProjDirPath}/../../../../platform/devices/MKS22F12/fsl_clock_MKS22F12.h"
"${ProjDirPath}/../../../../platform/devices/MKS22F12/fsl_clock_MKS22F12.c"
"${ProjDirPath}/../../../../platform/drivers/adc16/fsl_adc16.c"
"${ProjDirPath}/../../../../platform/drivers/adc16/fsl_adc16.h"
)


