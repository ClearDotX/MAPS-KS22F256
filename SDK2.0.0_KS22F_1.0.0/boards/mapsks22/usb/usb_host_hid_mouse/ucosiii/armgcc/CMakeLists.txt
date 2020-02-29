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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MKS22FN256VLL12")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_STACK_UCOSIII")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_UCOSIII")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D_DEBUG=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNODEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MKS22FN256VLL12")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_STACK_UCOSIII")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_UCOSIII")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} muldefs")

include_directories(${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU)

include_directories(${ProjDirPath}/../../../../../../rtos/ucosiii/uC-CPU/ARM-Cortex-M4/GNU)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../devices/MKS22F12/drivers)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/config/ucosiii)

include_directories(${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source)

include_directories(${ProjDirPath}/../../../../../../rtos/ucosiii/uC-CPU)

include_directories(${ProjDirPath}/../../../../../../rtos/ucosiii/uC-LIB)

include_directories(${ProjDirPath}/../../../../../../devices/MKS22F12/utilities)

include_directories(${ProjDirPath}/../../../../../../middleware/usb)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/osa)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/include)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/host)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/host/class)

include_directories(${ProjDirPath}/../../../../../../devices/MKS22F12)

include_directories(${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU)

include_directories(${ProjDirPath}/../../../../../../rtos/ucosiii/uC-CPU/ARM-Cortex-M4/GNU)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../devices/MKS22F12/drivers)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/config/ucosiii)

include_directories(${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source)

include_directories(${ProjDirPath}/../../../../../../rtos/ucosiii/uC-CPU)

include_directories(${ProjDirPath}/../../../../../../rtos/ucosiii/uC-LIB)

include_directories(${ProjDirPath}/../../../../../../devices/MKS22F12/utilities)

include_directories(${ProjDirPath}/../../../../../../middleware/usb)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/osa)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/include)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/host)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/host/class)

include_directories(${ProjDirPath}/../../../../../../devices/MKS22F12)

add_executable(host_hid_mouse_ucosiii.elf 
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU/os_cpu.h"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU/os_cpu_a.S"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU/os_cpu_c.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uC-CPU/ARM-Cortex-M4/GNU/cpu_c.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uC-CPU/ARM-Cortex-M4/GNU/cpu_a.S"
"${ProjDirPath}/../../../../../../devices/MKS22F12/gcc/startup_MKS22F12.S"
"${ProjDirPath}/../../../../../../middleware/usb/host/usb_host_khci.c"
"${ProjDirPath}/../../../../../../middleware/usb/host/usb_host_khci.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../../middleware/usb/osa/usb_osa.h"
"${ProjDirPath}/../../../../../../middleware/usb/osa/usb_osa_ucosiii.h"
"${ProjDirPath}/../../../../../../middleware/usb/osa/usb_osa_ucosiii.c"
"${ProjDirPath}/../../../../../../middleware/usb/host/class/usb_host_hid.c"
"${ProjDirPath}/../../../../../../middleware/usb/host/class/usb_host_hid.h"
"${ProjDirPath}/../../../../../../middleware/usb/host/class/usb_host_hub.c"
"${ProjDirPath}/../../../../../../middleware/usb/host/class/usb_host_hub.h"
"${ProjDirPath}/../../../../../../middleware/usb/host/class/usb_host_hub_app.c"
"${ProjDirPath}/../../../../../../middleware/usb/host/class/usb_host_hub_app.h"
"${ProjDirPath}/../app.c"
"${ProjDirPath}/../host_mouse.c"
"${ProjDirPath}/../host_mouse.h"
"${ProjDirPath}/../usb_host_config.h"
"${ProjDirPath}/../os_cfg.h"
"${ProjDirPath}/../cpu_cfg.h"
"${ProjDirPath}/../os_cfg_app.h"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os.h"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_cfg_app.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_core.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_dbg.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_flag.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_int.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_mem.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_mon.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_msg.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_mutex.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_pend_multi.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_prio.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_q.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_sem.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_stat.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_task.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_tick.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_time.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_tmr.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_type.h"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uCOS-III/Source/os_var.c"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uC-CPU/cpu_core.h"
"${ProjDirPath}/../../../../../../rtos/ucosiii/uC-CPU/cpu_core.c"
"${ProjDirPath}/../../../../../../devices/MKS22F12/system_MKS22F12.c"
"${ProjDirPath}/../../../../../../devices/MKS22F12/system_MKS22F12.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/utilities/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/MKS22F12/utilities/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/utilities/fsl_misc_utilities.c"
"${ProjDirPath}/../../../../../../devices/MKS22F12/utilities/fsl_misc_utilities.h"
"${ProjDirPath}/../../../../../../middleware/usb/include/usb.h"
"${ProjDirPath}/../../../../../../middleware/usb/host/usb_host.h"
"${ProjDirPath}/../../../../../../middleware/usb/host/usb_host_hci.c"
"${ProjDirPath}/../../../../../../middleware/usb/host/usb_host_hci.h"
"${ProjDirPath}/../../../../../../middleware/usb/host/usb_host_devices.c"
"${ProjDirPath}/../../../../../../middleware/usb/host/usb_host_devices.h"
"${ProjDirPath}/../../../../../../middleware/usb/host/usb_host_framework.h"
"${ProjDirPath}/../../../../../../middleware/usb/host/usb_host_framework.c"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_mcg.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_mcg.c"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_osc.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_sim.c"
"${ProjDirPath}/../../../../../../devices/MKS22F12/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_common.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_smc.h"
"${ProjDirPath}/../../../../../../devices/MKS22F12/drivers/fsl_smc.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MKS22FN256xxx12_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MKS22FN256xxx12_flash.ld -static")

TARGET_LINK_LIBRARIES(host_hid_mouse_ucosiii.elf -Wl,--start-group)
target_link_libraries(host_hid_mouse_ucosiii.elf debug m)

target_link_libraries(host_hid_mouse_ucosiii.elf debug c)

target_link_libraries(host_hid_mouse_ucosiii.elf debug gcc)

target_link_libraries(host_hid_mouse_ucosiii.elf debug nosys)

target_link_libraries(host_hid_mouse_ucosiii.elf optimized m)

target_link_libraries(host_hid_mouse_ucosiii.elf optimized c)

target_link_libraries(host_hid_mouse_ucosiii.elf optimized gcc)

target_link_libraries(host_hid_mouse_ucosiii.elf optimized nosys)

TARGET_LINK_LIBRARIES(host_hid_mouse_ucosiii.elf -Wl,--end-group)

# BIN AND HEX
ADD_CUSTOM_COMMAND(TARGET host_hid_mouse_ucosiii.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Oihex ${EXECUTABLE_OUTPUT_PATH}/host_hid_mouse_ucosiii.elf ${EXECUTABLE_OUTPUT_PATH}/host_hid_mouse_ucosiii.hex)
ADD_CUSTOM_COMMAND(TARGET host_hid_mouse_ucosiii.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/host_hid_mouse_ucosiii.elf ${EXECUTABLE_OUTPUT_PATH}/host_hid_mouse_ucosiii.bin)
