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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_smartcard.h"
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && FSL_FEATURE_SOC_EMVSIM_COUNT
#include "fsl_smartcard_emvsim.h"
#else
#include "fsl_smartcard_uart.h"
#endif
#if defined(USING_PHY_NCN8025)
#include "fsl_smartcard_phy_ncn8025.h"
#endif
#if defined(USING_PHY_EMVSIM)
#include "fsl_smartcard_phy_emvsim.h"
#endif
#if defined(USING_PHY_GPIO)
#include "fsl_smartcard_phy_gpio.h"
#endif

#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"
/*******************************************************************************
* Definitions
******************************************************************************/
#define SMARTCARD_Control(base, handle, control, param) SMARTCARD_UART_Control(base, handle, control, 0)
#define SMARTCARD_TransferNonBlocking(base, handle, xfer) SMARTCARD_UART_TransferNonBlocking(base, handle, xfer)
#define SMARTCARD_Init(base, handle, sourceClockHz) SMARTCARD_UART_Init(base, handle, sourceClockHz)
#define SMARTCARD_Deinit(base) SMARTCARD_UART_Deinit(base)
#define SMARTCARD_GetTransferRemainingBytes(base, handle) SMARTCARD_UART_GetTransferRemainingBytes(base, handle)
#define SMARTCARD_AbortTransfer(base, handle) SMARTCARD_UART_AbortTransfer(base, handle)

#define SMARTCARD_PHY_Activate(base, handle, resetType) SMARTCARD_PHY_GPIO_Activate(base, handle, resetType)
#define SMARTCARD_PHY_Deactivate(base, handle) SMARTCARD_PHY_GPIO_Deactivate(base, handle)
#define SMARTCARD_PHY_Control(base, handle, control, param) SMARTCARD_PHY_GPIO_Control(base, handle, control, param)
#define SMARTCARD_PHY_Init(base, config, sourceClockHz) SMARTCARD_PHY_GPIO_Init(base, config, sourceClockHz)
#define SMARTCARD_PHY_Deinit(base, config) SMARTCARD_PHY_GPIO_Deinit(base, config)
#define SMARTCARD_PHY_GetDefaultConfig(config) SMARTCARD_PHY_GPIO_GetDefaultConfig(config)
#define MAX_TRANSFER_SIZE (258u)
#define EMVL1_ATR_BUFF_SIZE (100u)
/*******************************************************************************
* Prototypes
******************************************************************************/
#if !defined(MAPS_KS22)
static void wait_for_card_presence(smartcard_context_t *context);
#endif
static int16_t receive_atr(smartcard_context_t *context, uint8_t *buf, uint16_t length);
static void send_pps(smartcard_context_t *context, uint8_t *ppsBuff, uint8_t *rxBuff, uint8_t ppsLength);
void smartcard_interface_callback_function(void *context, void *param);
void smartcard_installTimeDelay(smartcard_context_t *context);
void timeDelay(uint32_t miliseconds);
void smartcard_interrupts_config(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && FSL_FEATURE_SOC_EMVSIM_COUNT
static EMVSIM_Type *base = BOARD_SMARTCARD_MODULE;
#else
static UART_Type *base = BOARD_SMARTCARD_MODULE;
#endif
smartcard_context_t g_smartcardContext;
/*******************************************************************************
* Code
******************************************************************************/

/* IRQ handler for uart */
void UART0_RX_TX_IRQHandler(void)
{
    SMARTCARD_UART_IRQHandler(BOARD_SMARTCARD_MODULE, &g_smartcardContext);
}
/* Error IRQ handler for uart */
void UART0_ERR_IRQHandler(void)
{
    SMARTCARD_UART_ErrIRQHandler(BOARD_SMARTCARD_MODULE, &g_smartcardContext);
}
/* Pit timer channel 0 IRQ handler */
void PIT0_IRQHandler(void)
{
    SMARTCARD_UART_TSExpiryCallback(BOARD_SMARTCARD_MODULE, &g_smartcardContext);
}
/*
 * This example will example the efficiency of the transmit/receive drivers with
 * using non-blocking/async methods. Transfer data between board and smartcard
 * (Zeitcontrol ZC7.5 RevD). The example will do transfers both in T=0 and T=1
 * modes.
 */

void smartcard_interrupts_config(void)
{
    /* Set smartcard communication peripheral interrupt priority */
    NVIC_SetPriority(BOARD_SMARTCARD_MODULE_IRQ, 8u);
#if defined(BOARD_SMARTCARD_MODULE_ERRIRQ)
    NVIC_SetPriority(BOARD_SMARTCARD_MODULE_ERRIRQ, 8u);
#endif
/* Set smartcard presence detect gpio pin interrupt priority */
#if defined(USING_PHY_NCN8025)
    NVIC_SetPriority(BOARD_SMARTCARD_IRQ_PIN_IRQ, 6u);
#endif /* USING_NCN8025_INTERFACE */
       /* Set external PIT timer interrupt priority
        * (used for initial TS char detection time-out) */
#if !(defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT))
#if !defined(BOARD_SMARTCARD_TS_TIMER_IRQ)
#error "Please specify external PIT timer interrupt !"
#else
    NVIC_SetPriority(BOARD_SMARTCARD_TS_TIMER_IRQ, 6u);
#endif
#endif
}
/*!
 * @brief Time delay required by smartcard test and smartcard driver. Function
 * waits desired time with accuracy 1ms.
 */
void timeDelay(uint32_t miliseconds)
{
    miliseconds++;
    /* Wait desired time */
    while (miliseconds > 0)
    {
        if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
        {
            miliseconds--;
        }
    }
}
/* Function initializes time delay for smartcard driver */
void smartcard_installTimeDelay(smartcard_context_t *context)
{
    /* Disable SysTick timer */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    /* Initialize Reload value to 1ms */
    SysTick->LOAD = CLOCK_GetFreq(kCLOCK_CoreSysClk) / 1000;
    /* Set clock source to processor clock */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    /* Enable SysTick timer */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    /* Store time delay function to smartcard context */
    context->timeDelay = timeDelay;
}
#if !defined(MAPS_KS22)
/*!
 * @brief This function waits for card insertion/detection
 */
static void wait_for_card_presence(smartcard_context_t *context)
{
    smartcard_interface_control_t interfaceControl = kSMARTCARD_InterfaceReadStatus;

    /* Putting delay as few boards has de-bouncing cap in card slot presence detect pin */
    timeDelay(1000);
    /* Read card presence status */
    SMARTCARD_PHY_Control(base, context, interfaceControl, 0);
    /* Check if a card is already inserted */
    if (false == context->cardParams.present)
    {
        PRINTF("Please insert a smart card to test \r\n");
    }
    do
    { /* Read card presence status */
        SMARTCARD_PHY_Control(base, context, interfaceControl, 0);
    } while (context->cardParams.present == false);
}
#endif

/*!
 * @brief Callback function for the Smartcard interface/PHY IC ISR
 */
void smartcard_interface_callback_function(void *context, void *param)
{
    /* Insertion/removal action occur.
     * Do whatever you want */
}

/*
* This function receives the ATR bytes.
*/
static int16_t receive_atr(smartcard_context_t *context, uint8_t *buf, uint16_t length)
{
    smartcard_xfer_t xfer;

    /* Set the correct context */
    context->transferState = kSMARTCARD_ReceivingState;
    xfer.size = length;
    xfer.buff = buf;
    xfer.direction = kSMARTCARD_Receive;
    /* Receive ATR bytes using non-blocking API */
    SMARTCARD_TransferNonBlocking(base, context, &xfer);
    /* Wait for transfer to finish */
    while (0 != SMARTCARD_GetTransferRemainingBytes(base, context))
    {
    }

    return (length - context->xSize);
}

/*
* This function sends a basic PPS to the card
*/
static void send_pps(smartcard_context_t *context, uint8_t *ppsBuff, uint8_t *rxBuff, uint8_t ppsLength)
{
    smartcard_xfer_t xfer;
    /* Initialize timer states in driver */
    context->timersState.wwtExpired = false;
    context->timersState.cwtExpired = false;
    context->timersState.bwtExpired = false;

    /* Reset WWT timer */
    SMARTCARD_Control(base, context, kSMARTCARD_ResetWWT, 0);
    /* Enable WWT timer before starting T=0 transport */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableWWT, 0);
    xfer.buff = ppsBuff;
    xfer.size = ppsLength;
    xfer.direction = kSMARTCARD_Transmit;
    /* Send PPS bytes to card using non-blocking API */
    SMARTCARD_TransferNonBlocking(base, context, &xfer);
    /* Wait transfer to finish */
    while (0 != SMARTCARD_GetTransferRemainingBytes(base, context))
    {
    }
    xfer.buff = rxBuff;
    xfer.size = ppsLength;
    xfer.direction = kSMARTCARD_Receive;
    /* Receive PPS bytes from card using non-blocking API */
    xfer.buff = rxBuff;
    xfer.size = ppsLength;
    SMARTCARD_TransferNonBlocking(base, context, &xfer);
    /* Wait transfer to finish */
    while (0 != SMARTCARD_GetTransferRemainingBytes(base, context))
    {
    }
    /* Disable WWT timer */
    SMARTCARD_Control(base, context, kSMARTCARD_DisableWWT, 0);
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t atrBuff[EMVL1_ATR_BUFF_SIZE];
    uint16_t i, j;
    uint32_t rcvLength = 0;
    uint8_t length;
    uint8_t T1_LRC = 0;
    uint8_t increment = 1;
    smartcard_xfer_t xfer;
    smartcard_context_t *context = &g_smartcardContext;
    uint8_t txBuff[MAX_TRANSFER_SIZE];
    uint8_t rxBuff[MAX_TRANSFER_SIZE];

    memset(context, 0, sizeof(smartcard_context_t));
    /* Initialize board hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Select the clock source for the TPM counter as MCGPLLCLK */
    CLOCK_SetTpmClock(1U);
    CLOCK_SetPllFllSelClock(1U);
    /* Initialize smartcard interrupts */
    smartcard_interrupts_config();
    /* Initialize interface configuration structure to default values */
    SMARTCARD_PHY_GetDefaultConfig(&context->interfaceConfig);
    /* Fill in SMARTCARD driver user configuration data */
    context->interfaceConfig.smartCardClock = BOARD_SMARTCARD_CLOCK_VALUE;
    context->interfaceConfig.vcc = kSMARTCARD_VoltageClassB3_3V;
    context->interfaceConfig.clockModule = BOARD_SMARTCARD_CLOCK_MODULE;
    context->interfaceConfig.clockModuleChannel = BOARD_SMARTCARD_CLOCK_MODULE_CHANNEL;
    context->interfaceConfig.clockModuleSourceClock = BOARD_SMARTCARD_CLOCK_MODULE_SOURCE_CLK;
    context->interfaceConfig.controlPort = BOARD_SMARTCARD_CONTROL_PORT;
    context->interfaceConfig.controlPin = BOARD_SMARTCARD_CONTROL_PIN;
#if defined(BOARD_SMARTCARD_IRQ_PORT)
    context->interfaceConfig.irqPort = BOARD_SMARTCARD_IRQ_PORT;
    context->interfaceConfig.irqPin = BOARD_SMARTCARD_IRQ_PIN;
#endif
    context->interfaceConfig.resetPort = BOARD_SMARTCARD_RST_PORT;
    context->interfaceConfig.resetPin = BOARD_SMARTCARD_RST_PIN;
#if defined(USING_PHY_NCN8025)
    context->interfaceConfig.vsel0Port = BOARD_SMARTCARD_VSEL0_PORT;
    context->interfaceConfig.vsel0Pin = BOARD_SMARTCARD_VSEL0_PIN;
    context->interfaceConfig.vsel1Port = BOARD_SMARTCARD_VSEL1_PORT;
    context->interfaceConfig.vsel1Pin = BOARD_SMARTCARD_VSEL1_PIN;
#endif
/* Setup the numerical id for TS detection timer */
#if defined(BOARD_SMARTCARD_TS_TIMER_ID)
    context->interfaceConfig.tsTimerId = BOARD_SMARTCARD_TS_TIMER_ID;
#endif
#if defined(USING_PHY_GPIO)
    context->interfaceConfig.dataPort = BOARD_SMARTCARD_DATA_PORT;
    context->interfaceConfig.dataPin = BOARD_SMARTCARD_DATA_PIN;
    context->interfaceConfig.dataPinMux = BOARD_SMARTCARD_DATA_PIN_MUX;
#endif
    /* Install time delay defined in application to smartcard driver. */
    smartcard_installTimeDelay(context);
    PRINTF("\r\n***** SMARTCARD Driver Send Receive functionality example *****\r\n\r\n");
    /* Initialize the smartcard module with base address and config structure*/
    SMARTCARD_Init(base, context, CLOCK_GetFreq(BOARD_SMARTCARD_CLOCK_MODULE_SOURCE_CLK));
    SMARTCARD_PHY_Init(base, &context->interfaceConfig, CLOCK_GetFreq(BOARD_SMARTCARD_CLOCK_MODULE_SOURCE_CLK));
    /* Install test/application callback function */
    context->interfaceCallback = smartcard_interface_callback_function;
#if !defined(USING_PHY_GPIO)
    /* Wait for a card inserted */
    wait_for_card_presence(context);
    PRINTF("Card inserted.\r\n");
#endif
    /* Deactivate the card */
    PRINTF("Deactivating card...");
    SMARTCARD_PHY_Deactivate(base, context);
    PRINTF("Done!\r\n");
    /* Invalidate ATR buffer first */
    memset(atrBuff, 0, EMVL1_ATR_BUFF_SIZE);
    /* Deactivate the card */
    PRINTF("Resetting/Activating card...");
    /* Enable Initial character detection first */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableInitDetect, 0);
    /* Invoke SMARTCARD IP specific function to activate the card */
    SMARTCARD_PHY_Activate(base, context, kSMARTCARD_ColdReset);
    /* Enable receiver */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableReceiverMode, 0);
    /* Clear timers status flags */
    context->timersState.adtExpired = false;
    context->timersState.wwtExpired = false;
    /* Enable ATR timer and disable wait timer */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableADT, 0);
    SMARTCARD_Control(base, context, kSMARTCARD_DisableWWT, 0);
    /* Set internal driver context to receive initial TS character */
    context->transferState = kSMARTCARD_WaitingForTSState;
    context->xIsBusy = true;
    /* Wait for Initial character. In case wrong Initial character is received, reject the card
    */
    while ((context->xIsBusy == true) && (context->transferState == kSMARTCARD_WaitingForTSState))
    { /* Wait until all the data is received, or error occurs */
    }
    if (context->transferState != kSMARTCARD_IdleState)
    { /* Reject the card */
        PRINTF("INVALID ATR received.... \r\nCard Rejected !\r\n");
        PRINTF("Example finished. \r\n");
        return 0;
    }
    /* Start WWT,CWT and ADT timer */
    context->timersState.wwtExpired = false;
    context->timersState.cwtExpired = false;
    /* Enable WWT and ADT timer before starting T=0 transport */
    SMARTCARD_Control(base, context, kSMARTCARD_ResetWWT, 0);
    SMARTCARD_Control(base, context, kSMARTCARD_EnableWWT, 0);
    /* Receiving cold ATR */
    /* Disable nack while receiving ATR */
    SMARTCARD_Control(base, context, kSMARTCARD_DisableAnack, 0);
    receive_atr(context, atrBuff, EMVL1_ATR_BUFF_SIZE);
    /* Enable the nack after receiving ATR */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableAnack, 0);
    /* Disable ATR & WorkWait timers */
    SMARTCARD_Control(base, context, kSMARTCARD_DisableADT, 0);
    SMARTCARD_Control(base, context, kSMARTCARD_DisableWWT, 0);
    /* Initialize with Zeitcontrol ZC7.5 RevD card known ATR parameters */
    context->cardParams.Fi = 372;
    context->cardParams.WI = 0x80;
    context->cardParams.currentD = 1;
    context->cardParams.CWI = 0x05;
    context->cardParams.BWI = 0x07;
    PRINTF("Done!\r\n");

    PRINTF("======================================================\r\n");
    PRINTF("Send receive functionality example in T=0 mode started!\r\n");
    /******************* Setup for T=0 transfer *******************/
    SMARTCARD_Control(base, context, kSMARTCARD_SetupT0Mode, 0);
    /* Send a PPS for T=0 mode */
    /* send a PPS indicating T=1 protocol */
    txBuff[0] = 0xFF;
    txBuff[1] = 0x00;
    txBuff[2] = 0x00;

    for (i = 0; i < 2; i++)
    {
        txBuff[2] ^= txBuff[i];
    }
    send_pps(context, txBuff, rxBuff, 3);
    /******************* Prepare and transfer a T-APDU *******************/
    /* APDU */
    txBuff[0] = 0xC0; /* CLA command */
    txBuff[1] = 0x02; /* Instruction code (0x14 for EEPROM SIZE) */
    txBuff[2] = 0x00; /* P1 */
    txBuff[3] = 0x00; /* P2 */
    txBuff[4] = 0x06; /* P3 = Le */
    /* Start WWT timer */
    context->timersState.wwtExpired = false;
    context->timersState.cwtExpired = false;
    context->timersState.bwtExpired = false;
    /* Reset WWT timer */
    SMARTCARD_Control(base, context, kSMARTCARD_ResetWWT, 0);
    /* Enable WWT timer before starting T=0 transport */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableWWT, 0);
    /* Send command using non-blocking API */
    xfer.buff = txBuff;
    xfer.direction = kSMARTCARD_Transmit;
    xfer.size = 5;
    SMARTCARD_TransferNonBlocking(base, context, &xfer);
    /* Wait until transfer is completed */
    while (0 != SMARTCARD_GetTransferRemainingBytes(base, context))
    {
    }
    /* Wait for INS byte using non-blocking API */
    xfer.buff = rxBuff;
    xfer.size = 1;
    xfer.direction = kSMARTCARD_Receive;
    SMARTCARD_TransferNonBlocking(base, context, &xfer);
    /* Wait until transfer is completed */
    while (0 != SMARTCARD_GetTransferRemainingBytes(base, context))
    {
    }
    /*
     * Now receive the remaining bytes using non-blocking API.
     * 6 bytes for Start address and size of EEPROM for Zeitcontrol ZC7.5 RevD
     * 2 bytes for status words.
     */
    xfer.buff = rxBuff + 1;
    xfer.size = 6 + 2;
    SMARTCARD_TransferNonBlocking(base, context, &xfer);
    while (0 != SMARTCARD_GetTransferRemainingBytes(base, context))
    {
    }
    /* Disable WWT timer */
    SMARTCARD_Control(base, context, kSMARTCARD_DisableWWT, 0);

    PRINTF("\r\nSmartcard EEPROM Start Address = 0x%02X%02X%02X\r\n", rxBuff[1], rxBuff[2], rxBuff[3]);
    if ((rxBuff[4] != 0x00) || (rxBuff[5] != 0x80) || (rxBuff[6] != 0x00))
    {
        PRINTF("Incorrect smartcard EEPROM Size!\r\n");
        PRINTF("Example finished. \r\n");
        return 0;
    }
    else
    {
        PRINTF("Smartcard EEPROM Size = 32 KBytes\r\n");
    }

    /* Deactivate the card */
    PRINTF("\r\nDeactivating card...");
    SMARTCARD_PHY_Deactivate(base, context);
    PRINTF("Done!\r\n");

    /******************* Setup for T=0 transfer *******************/
    SMARTCARD_Control(base, context, kSMARTCARD_SetupATRMode, 0);
    /* Invalidate ATR buffer first */
    memset(atrBuff, 0, EMVL1_ATR_BUFF_SIZE);
    /* Deactivate the card */
    PRINTF("Resetting/Activating card...");
    /* Enable initial character detection */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableInitDetect, 0);
    /* Reset and activate the card */
    SMARTCARD_PHY_Activate(base, context, kSMARTCARD_ColdReset);
    /* Enable receiver */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableReceiverMode, 0);
    /* Set default values for protocol timers flags */
    context->timersState.adtExpired = false;
    context->timersState.wwtExpired = false;
    /* Enable ATR timer and disable WorkWait timer */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableADT, 0);
    SMARTCARD_Control(base, context, kSMARTCARD_DisableWWT, 0);

    context->transferState = kSMARTCARD_WaitingForTSState;
    context->xIsBusy = true;
    /* Wait for Initial character. */
    while ((context->xIsBusy == true) && (context->transferState == kSMARTCARD_WaitingForTSState))
    {
        /* Wait until all the data is received, or error occurs */
    }
    if (context->transferState != kSMARTCARD_IdleState)
    { /* Reject the card */
        PRINTF("INVALID ATR received.... \r\nCard Rejected !\r\n");
        PRINTF("Example finished. \r\n");
        return 0;
    }
    /* Start WWT,CWT and ADT  timer */
    context->timersState.wwtExpired = false;
    context->timersState.cwtExpired = false;
    /* Enable WWT and ADT timer before starting T=0 transport */
    SMARTCARD_Control(base, context, kSMARTCARD_ResetWWT, 0);
    SMARTCARD_Control(base, context, kSMARTCARD_EnableWWT, 0);

    /* Receiving cold ATR */
    /* Disable nack while receiving ATR */
    SMARTCARD_Control(base, context, kSMARTCARD_DisableAnack, 0);
    receive_atr(context, atrBuff, EMVL1_ATR_BUFF_SIZE);
    /* Enable the nack after receiving ATR */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableAnack, 0);
    /* Disable ATR & WordWait timers */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableADT, 0);
    SMARTCARD_Control(base, context, kSMARTCARD_DisableWWT, 0);

    /* Initialize with Zeitcontrol ZC7.5 RevD card known ATR parameters */
    context->cardParams.Fi = 372;
    context->cardParams.WI = 0x80;
    context->cardParams.currentD = 1;
    context->cardParams.CWI = 0x05;
    context->cardParams.BWI = 0x07;
    PRINTF("Done!\r\n");

    PRINTF("\r\nSend receive functionality example in T=1 mode started!\r\n");
    /******************* Setup for T=0 transfer *******************/
    SMARTCARD_Control(base, context, kSMARTCARD_SetupT0Mode, 0);
    /* Send a PPS indicating T=1 protocol */
    txBuff[0] = 0xFF;
    txBuff[1] = 0x01;
    txBuff[2] = 0x00;

    for (i = 0; i < 2; i++)
    {
        txBuff[2] ^= txBuff[i];
    }
    send_pps(context, txBuff, rxBuff, 3);
    /* Set ECHO command data length */
    length = 16; /* payload */
    /* Prolog */
    txBuff[0] = 0x00;       /* NAD */
    txBuff[1] = 0;          /* I-block, PCB */
    txBuff[2] = length + 6; /* LEN */
    /* APDU */
    txBuff[3] = 0xC0;      /* CLA command */
    txBuff[4] = 0x14;      /* Instruction code (0x14 for ECHO) */
    txBuff[5] = increment; /* increment value - P1 */
    txBuff[6] = 0;         /* P2 */
    txBuff[7] = length;    /* P3 = Lc */

    /* Fill the data bytes */
    for (i = 0; i < length; i++)
    {
        txBuff[i + 8] = i;
    }
    /* Last byte is the response data length */
    txBuff[length + 8] = length;
    /* Calculate LRC */
    for (j = 0; j <= length + 8; j++)
    {
        T1_LRC ^= txBuff[j];
    }
    txBuff[length + 9] = T1_LRC;

    /******************* Setup for T=1 transfer *******************/
    SMARTCARD_Control(base, context, kSMARTCARD_SetupT1Mode, 0);
    /* Start CWT timer */
    context->timersState.wwtExpired = false;
    context->timersState.cwtExpired = false;
    context->timersState.bwtExpired = false;
    /* Enable BWT timer */
    SMARTCARD_Control(base, context, kSMARTCARD_EnableBWT, 0);
    /* Send T-APDU command using non-blocking API */
    xfer.buff = txBuff;
    /* length + 3bytes of prolog + 5bytes APDU + 1byte response data length + 1by LRC */
    xfer.size = length + 10;
    xfer.direction = kSMARTCARD_Transmit;
    SMARTCARD_TransferNonBlocking(base, context, &xfer);
    /* Wait until transfer is completed */
    while (0 != SMARTCARD_GetTransferRemainingBytes(base, context))
    {
    }
    /* Receive T-APDU response using non-blocking API */
    xfer.buff = rxBuff;
    xfer.direction = kSMARTCARD_Receive;
    xfer.size = length + 6;
    SMARTCARD_TransferNonBlocking(base, context, &xfer);
    /* Wait until transfer is completed */
    while (0 != SMARTCARD_GetTransferRemainingBytes(base, context))
    {
    }

    /* Disable CWT timer */
    SMARTCARD_Control(base, context, kSMARTCARD_DisableCWT, 0);
    /* Payload length + NAD + PCB + LEN + SW1 + SW2 + LRC, it should be length + 6 */
    rcvLength = (length + 6) - context->xSize;
    if (rcvLength != (length + 6))
    {
        PRINTF("Received length did not match!\r\n");
        PRINTF("Example finished. \r\n");
        return 0;
    }
    T1_LRC = 0;
    for (i = 0; i < rcvLength - 1; i++)
    {
        T1_LRC ^= rxBuff[i];
    }
    PRINTF("\r\n");
    /* Check if received LRC is correct */
    if (T1_LRC == rxBuff[i])
    {
        PRINTF("LRC received correctly\r\n");
    }
    else
    {
        PRINTF("LRC Error\r\n");
        PRINTF("Example finished. \r\n");
        PRINTF("Example finished. \r\n");
        return 0;
    }
    /* Check if receive data is correct */
    for (i = 0; i < length; i++)
    {
        if ((txBuff[i + 8] + increment) != rxBuff[i + 3])
        {
            PRINTF("Incorrect Echo data.\r\n");
            PRINTF("Example finished. \r\n");
            return 0;
        }
    }
    if (i == length)
    {
        PRINTF("Echo data received correctly!\r\n");
    }
    /* Check if Command response status bytes are OK */
    if ((rxBuff[i + 3] == 0x90) && (rxBuff[i + 4] == 0x0))
    {
        PRINTF("Command status OK\r\n");
    }
    else
    {
        PRINTF("Command status not OK\r\n");
    }

    PRINTF("\r\nSend receive functionality example finished!\r\n");
    PRINTF("======================================================\r\n");
    while (1)
    {
    }
}
