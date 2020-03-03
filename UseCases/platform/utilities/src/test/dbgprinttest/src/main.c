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

#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include "fsl_debug_console.h"

int main(void)
{
    char temp[100] = {0};

    hardware_init();
    dbg_uart_init();

    /* STDLIB PRINTF supports i/u/d/b/o/(#)x/(#)X/p/c/n m.nf/F format */
    int32_t a = 123456789;
    int32_t c = 97;
    PRINTF("*********************************************************\r\n");
    PRINTF("Integer test (Test number is 123456789)\r\n");
    PRINTF("Integer is %i\r\n", a);
    PRINTF("Unsigned integer is %u\r\n", a);
    PRINTF("Decimal is %d\r\n", a);
    PRINTF("Binary is %b\r\n", a);
    PRINTF("Octal is %o\r\n", a);
    PRINTF("Hex is %x, %#x, %X, %#X\r\n", a, a, a, a);
    PRINTF("Pointer to it is %p\r\n", a);
    PRINTF("ASCII 97 is character '%c'\r\n", c);

    #if PRINTF_FLOAT_ENABLE
    double b = 1234.9876543210987;
    PRINTF("*********************************************************\r\n");
    PRINTF("Float precision test (Test number is 1234.9876543210987)\r\n");
    PRINTF("Float is %f\r\n", b);
    PRINTF("Precision(set to %%2.11f) is %2.11f\r\n", b);
    #endif

    /* STDLIB SCANF supports d/u/i/o/x/c/s f/Lf format */
    int32_t sc0, sc1, sc2, sc3, sc4 = 0;
    char sc5, sc6[10];
    PRINTF("*********************************************************\r\n");
    PRINTF("SCANF test\r\n");
    PRINTF("Input 5 numbers, 1 character, 1 string:(seperate by space)\r\n");
    SCANF("%d %u %i %o %x %c %s", &sc0, &sc1, &sc2, &sc3, &sc4, &sc5, &sc6);
    PRINTF("Decimal scan of %d output in decimal format is %d\r\n", sc0, sc0);
    PRINTF("Unsigned scan of %u output in decimal format is %d\r\n", sc1, sc1);
    PRINTF("Integer scan of %i output in decimal format is %d\r\n", sc2, sc2);
    PRINTF("Octal scan of %o output in decimal format is %d\r\n", sc3, sc3);
    PRINTF("Hex scan of %x output in decimal format is %d\r\n", sc4, sc4);
    PRINTF("Char scan output is %c\r\n", sc5);
    PRINTF("String scan output is %s\r\n", sc6);

    #if SCANF_FLOAT_ENABLE
    float sf = 0;
    double sd = 0;
    PRINTF("Input 2 floating numbers:(seperate by space)\r\n");
    SCANF("%f %Lf", &sf, &sd);
    PRINTF("The number in float is %.7f\r\n", sf);
    PRINTF("The number in double is %.15f\r\n", sd);
    #endif

    PRINTF("*********************************************************\r\n");
    PRINTF("TEST END\r\n");
    return 1;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
