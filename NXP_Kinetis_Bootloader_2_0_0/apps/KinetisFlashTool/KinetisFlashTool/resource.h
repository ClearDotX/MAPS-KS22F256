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

// used by KinetisFlashTool.rc
//
#define IDM_ABOUTBOX                    0x0010
#define IDD_ABOUTBOX                    100
#define IDP_OLE_INIT_FAILED             100
#define IDS_ABOUTBOX                    101
#define IDD_KINETISFLASHTOOL_DIALOG     102
#define IDD_DIALOG_UPDATE               132
#define IDD_DIALOG_CONFIG               133
#define IDD_DIALOG_BOOTLOADERCONFIG     134
#define IDD_DIALOG_FLASH                135
#define IDI_FREESCALE                   137
#define IDC_RADIO_UART                  1001
#define IDC_RADIO_USBHID                1002
#define IDC_BUTTON_CNT_RESET            1003
#define IDC_TAB_FUNC                    1004
#define IDC_COMBO_PORT_VID              1005
#define IDC_COMBO_BAUD_PID              1006
#define IDC_EDIT_LOG                    1007
#define IDC_STATIC_STATUS               1008
#define IDC_STATIC_PORT_SET             1009
#define IDC_STATIC_BOOTLOADER_VERSION_STRING 1010
#define IDC_STATIC_PORT_VID             1011
#define IDC_STATIC_BAUD_PID             1012
#define IDC_STATIC_LOG                  1013
#define IDC_STATIC_BOOTLLOADER_VERSION  1014
#define IDC_STATIC_SERIAL_VERSION_STRING 1015
#define IDC_STATIC_SERIAL_VERSION       1016
#define IDC_STATIC_SECURITY_STATE_STRING 1017
#define IDC_STATIC_SECURITY_STATE       1018
#define IDC_STATIC_FLASH_SIZE_STRING    1019
#define IDC_STATIC_FLASH_SIZE           1020
#define IDC_STATIC_RAM_SIZE_STRING      1021
#define IDC_STATIC_RAM_SIZE             1022
#define IDC_STATIC_SECTOR_SIZE_STRING   1023
#define IDC_STATIC_SECTOR_SIZE          1024
#define IDC_STATIC_RESERVED_REGIONS_STRING 1025
#define IDC_STATIC_RESERVED_FLASH_STRING 1026
#define IDC_STATIC_RESERVED_RAM_STRING  1027
#define IDC_STATIC_RESERVED_FLASH_START 1028
#define IDC_STATIC_RESERVED_FLASH_END   1029
#define IDC_STATIC_TO_STRING1           1030
#define IDC_STATIC_TO_STRING2           1031
#define IDC_STATIC_RESERVED_RAM_START   1032
#define IDC_STATIC_TO_STRING3           1033
#define IDC_STATIC_RESERVED_RAM_END     1034
#define IDC_STATIC_TO_STRING4           1035
#define IDC_COMBO_FILE                  1050
#define IDC_BUTTON_BROWSE               1051
#define IDC_EDIT_ADDRESS                1052
#define IDC_RADIO_ERASE                 1053
#define IDC_RADIO_UNLOCK                1054
#define IDC_EDIT_KEY                    1055
#define IDC_BUTTON_UPDATE               1056
#define IDC_PROGRESS_BAR                1057
#define IDC_STATIC_FILE                 1058
#define IDC_STATIC_ADDRESS              1059
#define IDC_STATIC_KEY                  1060
#define IDC_CHECK_AUTOCNT               1061
#define IDC_STATIC_PROGRESS_BAR         1062
#define IDC_COMBO_FILE2                 1080
#define IDC_BUTTON_BROWSE2              1081
#define IDC_EDIT_BCA_BINARY             1082
#define IDC_BUTTON_CONFIG_BCA           1083
#define IDC_BUTTON_SAVE_BCA             1084
#define IDC_STATIC_FILE2                1085
#define IDC_STATIC_BCA_BINARY           1086
#define IDC_RELOAD                      1091
#define IDC_STATIC_CRC                  1092
#define IDC_CHECK_CRC                   1093
#define IDC_STATIC_CRC_STARTADDRESS     1094
#define IDC_EDIT_CRC_STARTADDRESS       1095
#define IDC_STATIC_CRC_LENGTH           1096
#define IDC_EDIT_CRC_LENGTH             1097
#define IDC_STATIC_PERIPHERALS          1098
#define IDC_CHECK_TAG                   1099
#define IDC_CHECK_PERIPHERAL_UART       1100
#define IDC_CHECK_PERIPHERAL_I2C        1101
#define IDC_CHECK_PERIPHERAL_SPI        1102
#define IDC_CHECK_PERIPHERAL_CAN        1103
#define IDC_CHECK_PERIPHERAL_USB        1104
#define IDC_STATIC_I2C                  1105
#define IDC_EDIT_I2C_ADDRESS            1107
#define IDC_STATIC_TIMEOUT              1108
#define IDC_EDIT_TIMEOUT                1110
#define IDC_STATIC_TIMEOUT_MS           1111
#define IDC_STATIC_USB                  1112
#define IDC_EDIT_USBVID                 1114
#define IDC_EDIT_CANTXID                1115
#define IDC_EDIT_USBPID                 1116
#define IDC_EDIT_CANRXID                1117
#define IDC_EDIT_USBSTRING              1118
#define IDC_STATIC_CLK                  1119
#define IDC_CHECK_ENABLE_HIGHSPEED      1120
#define IDC_EDIT_CLOCKDIVIDER           1122
#define IDC_STATIC_BOOT                 1123
#define IDC_RADIO_OSCERCLK              1125
#define IDC_RADIO_BUSCLK                1126
#define IDC_CHECK_DIRECTBOOT            1127
#define IDC_STATIC_MMCAU                1128
#define IDC_RADIO_SPEEDINDEX1           1130
#define IDC_EDIT_MMCAU                  1131
#define IDC_STATIC_OTFAD                1132
#define IDC_EDIT_OTFAD                  1134
#define IDC_STATIC_QSPI                 1135
#define IDC_EDIT_QSPI                   1137
#define IDC_STATIC_CAN                  1138
#define IDC_RADIO_SPEEDINDEX2           1139
#define IDC_RADIO_SPEEDINDEX3           1142
#define IDC_RADIO_SPEEDINDEX4           1143
#define IDC_RADIO_SPEEDINDEX5           1144
#define IDC_STATIC_PREDIV               1145
#define IDC_EDIT_PREDIV                 1146
#define IDC_STATIC_PSEG1                1147
#define IDC_EDIT_PSEG1                  1148
#define IDC_STATIC_RJW                  1149
#define IDC_EDIT_RJW                    1150
#define IDC_STATIC_PSEG2                1151
#define IDC_EDIT_PSEG2                  1152
#define IDC_STATIC_PROPSEG              1153
#define IDC_EDIT_PROPSEG                1154
#define IDC_BUTTON_CONFIG_OK            1155
#define IDC_BUTTON_CONFIG_CANCEL        1156
#define IDC_STATIC_TAG                  1157
#define IDC_CHECK_VID                   1158
#define IDC_CHECK_PID                   1159
#define IDC_CHECK_USBSTRING_POINTER     1160
#define IDC_CHECK_TIMEOUT               1161
#define IDC_CHECK_CANCLK                1162
#define IDC_CHECK_CANSPEED              1163
#define IDC_CHECK_CANTXID               1164
#define IDC_CHECK_I2CSLAVE_ADDRESS      1165
#define IDC_CHECK_QSPI_POINTER          1166
#define IDC_CHECK_OTFAD_POINTER         1167
#define IDC_CHECK_MMCAU_POINTER         1168
#define IDC_STATIC_CLKDIV               1169
#define IDC_CHECK_CANRXID               1170
#define IDC_BUTTON_CONFIG_GENERATE      1171
#define IDC_STATIC_ERASE                1200
#define IDC_COMBO_ERASE_OPT             1201
#define IDC_STATIC_IFR                  1202
#define IDC_COMBO_ERASE_MEMORYID        1203
#define IDC_STATIC_ERASE_ID             1204
#define IDC_EDIT_ERASE_ADDRESS          1205
#define IDC_STATIC_ERASE_ADDR           1206
#define IDC_EDIT_ERASE_LENGTH           1207
#define IDC_STATIC_ERASE_LENGTH         1208
#define IDC_BUTTON_ERASE                1209
#define IDC_EDIT_IFR_INDEX              1210
#define IDC_STATIC_IFR_INDEX            1211
#define IDC_EDIT_IFR_BYTECOUNT          1212
#define IDC_STATIC_IFR_BYTECOUNT        1213
#define IDC_EDIT_IFR_DATA               1214
#define IDC_STATIC_IFR_DATA             1215
#define IDC_BUTTON_IFR                  1216

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        138
#define _APS_NEXT_COMMAND_VALUE         32771
#define _APS_NEXT_CONTROL_VALUE         1182
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
