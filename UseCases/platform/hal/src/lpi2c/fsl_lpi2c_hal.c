/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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

#include "fsl_lpi2c_hal.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_MasterInit
 * Description   : Initialize LPI2C master to reset state.
 *
 *END**************************************************************************/
void LPI2C_HAL_MasterInit(LPI2C_Type * base)
{
    LPI2C_BWR_MCR_RST(base,0);
    LPI2C_WR_MCR(base,0);
    LPI2C_BWR_MCR_DBGEN(base,1);
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SlaveInit
 * Description   : Initialize LPI2C slave to reset state.
 *
 *END**************************************************************************/
void LPI2C_HAL_SlaveInit(LPI2C_Type * base)
{
    LPI2C_BWR_SCR_RST(base,0);
    LPI2C_WR_SCR(base,0);
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_MasterSetIntCmd
 * Description   : Enables or disables specific LPI2C master interrupt.
 *
 *END**************************************************************************/
void LPI2C_HAL_MasterSetIntCmd(LPI2C_Type * base,
                               uint32_t interrupt,
                               bool enable)
{
    uint32_t tmp = LPI2C_RD_MIER(base);

    if (enable) 
    {
        tmp |= interrupt;
        LPI2C_WR_MIER(base, tmp);
    }
    else
    {
        interrupt = ~interrupt;
        tmp &= interrupt;
        LPI2C_WR_MIER(base, tmp);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SlaveSetIntCmd
 * Description   : Enables or disables specific LPI2C slave interrupt.
 *
 *END**************************************************************************/
void LPI2C_HAL_SlaveSetIntCmd(LPI2C_Type * base,
                              uint32_t interrupt,
                              bool enable)
{
    uint32_t tmp = LPI2C_RD_SIER(base);
    if (enable) 
    {
        tmp |= interrupt;
        LPI2C_WR_SIER(base, tmp);
    }
    else
    {
        interrupt = ~interrupt;
        tmp &= interrupt;
        LPI2C_WR_SIER(base, tmp);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SetBaudRate
 * Description   : Sets the LPI2C bus frequency for master transactions.
 *
 *END**************************************************************************/
void LPI2C_HAL_SetBaudRate(LPI2C_Type * base,
                           uint32_t sourceClockInHz,
                           uint32_t kbps,
                           lpi2c_mode_t mode)
{
    uint32_t prescale = 0, clkHiCycle, clockInKHz;
    uint32_t bestPre = 0, bestclkHi = 0, computedRate, absError;
    uint32_t bestError = 0xffffffffu; 
    clockInKHz = sourceClockInHz/1000;
    /* Disable master mode. */
    LPI2C_HAL_SetMasterCmd(base, 0U);
    /* Baud rate = (sourceClockInHZ/2^prescale)/(CLKLO+1+CLKHI+1)*/
    /* Assume CLKLO = 2*CLKHI, SETHOLD = CLKHI, DATAVD = CLKHI/2.*/
    for(prescale = 1; (prescale <= 128)&& (bestError != 0); prescale = 2*prescale)
    {
        for(clkHiCycle = 1; clkHiCycle < 32; clkHiCycle++)
        {
            computedRate = (clockInKHz/prescale)/(3*clkHiCycle+2);
            absError = kbps > computedRate ? kbps - computedRate : computedRate - kbps;
            if (absError < bestError)
            {
                bestPre = prescale;
                bestclkHi = clkHiCycle;
                bestError = absError;

                /* If the error is 0, then we can stop searching
                 * because we won't find a better match.*/
                if (absError == 0)
                {
                    break;
                }
            }
        }
    }
    /* High speed mode*/
    if(mode == kLpi2cMasterHighSpeedMode)
    {
        LPI2C_BWR_MCCR1_CLKHI(base, bestclkHi);
        if(bestclkHi < 2)
        {
            LPI2C_BWR_MCCR1_CLKLO(base, 3);
            LPI2C_BWR_MCCR1_SETHOLD(base, 2);
            LPI2C_BWR_MCCR1_DATAVD(base, 1);
        }
        else
        {
            LPI2C_BWR_MCCR1_CLKLO(base, 2*bestclkHi); 
            LPI2C_BWR_MCCR1_SETHOLD(base, bestclkHi);
            LPI2C_BWR_MCCR1_DATAVD(base, bestclkHi/2);
        }
    }
    else
    {  /* standard, fast, fast mode plus and ultra-fast transfers */
        LPI2C_BWR_MCCR0_CLKHI(base, bestclkHi);
        if(bestclkHi < 2)
        {
            LPI2C_BWR_MCCR0_CLKLO(base, 3);
            LPI2C_BWR_MCCR0_SETHOLD(base, 2);
            LPI2C_BWR_MCCR0_DATAVD(base, 1);
        }
        else
        {
            LPI2C_BWR_MCCR0_CLKLO(base, 2*bestclkHi); 
            LPI2C_BWR_MCCR0_SETHOLD(base, bestclkHi);
          LPI2C_BWR_MCCR0_DATAVD(base, bestclkHi/2);
        }

    }
    for(int i =0; i< sizeof(lpi2c_prescaler); i++)
    {
      if(bestPre == lpi2c_prescaler[i])
      {
        bestPre = i;
        break;
      }
    }
    LPI2C_BWR_MCFGR1_PRESCALE(base, bestPre);
    /* Enable master mode. */
    LPI2C_HAL_SetMasterCmd(base, 1U);
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SetGlitch
 * Description   : Configure LPI2C slave to filter glitches below a certain width.
 *
 *END**************************************************************************/
void LPI2C_HAL_SetGlitch(LPI2C_Type * base, 
                         uint32_t sourceClockInHz, 
                         uint32_t glitchWidthInNs)
{
    uint32_t busCycle_ns = 1000000 / (sourceClockInHz / 1000);

    // Search for the cycle count just below the desired glitch width.
    uint32_t cycles = 0;
    while (((cycles + 1) * busCycle_ns) < glitchWidthInNs)
    {
        ++cycles;
    }

    // If we end up with zero cycles, then set the filter to a single cycle unless the
    // bus clock is greater than 10x the desired glitch width.
    if ((cycles == 0) && (busCycle_ns <= (glitchWidthInNs * 10)))
    {
        cycles = 1;
    }
    // If the cycles is greater the max cycles supported to set glitch filter,
    // then cycles should be equal to max cycles
    else if (cycles > 15)
    {
        cycles = 15;
    }
    
    LPI2C_BWR_SCFGR2_FILTSDA(base, cycles);
    LPI2C_BWR_SCFGR2_FILTSCL(base, cycles);
    LPI2C_BWR_SCR_FILTEN(base, 0x1U);
    LPI2C_BWR_SCFGR2_DATAVD(base, 0x1U);
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SendStart
 * Description   : Send a START or Repeated START signal on the LPI2C bus.
 * This function is used to initiate a new master mode transfer by sending the
 * START signal. It is also used to send a Repeated START signal when a transfer
 * is already in progress.
 *
 *END**************************************************************************/
void LPI2C_HAL_SendStart(LPI2C_Type * base, uint8_t address, lpi2c_direction_t dir)
{
    uint32_t addr = (((uint32_t)address) << 1U);
    addr |= dir;

    LPI2C_WR_MTDR(base, LPI2C_MTDR_CMD(kLpi2cMasterCmdStart) | addr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SendStop
 * Description   : Sends a STOP signal on the LPI2C bus.
 *
 *END**************************************************************************/
lpi2c_status_t LPI2C_HAL_SendStop(LPI2C_Type * base)
{
    uint32_t i = 0;

    /* Send the STOP signal */
    LPI2C_WR_MTDR(base, LPI2C_MTDR_CMD(kLpi2cMasterCmdStop));
    /* Wait for the STOP signal finish. */
    while(!LPI2C_HAL_MasterGetStatus(base, kLpi2cMasterStopDetectFlag))
    {
        if (++i == 0x400U)
        {
            /* Something is wrong because the bus is still busy. */
            return kStatus_LPI2C_StopSignalFail;
        }
        else
        {
            __asm("nop");
        }
    }

    return kStatus_LPI2C_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SlaveSetSclStretch
 * Description   : Enable/Disable SCL stretch for situations.
 *
 *END**************************************************************************/
void LPI2C_HAL_SlaveSetSclStretch(LPI2C_Type * base, 
                                  uint32_t stallType, 
                                  bool enable)
{
    uint32_t configValue = LPI2C_RD_SCFGR1(base);
    if(enable)
    {
        configValue |= stallType;
    }
    else
    {
        configValue &= ~(uint32_t)stallType;
    }
    LPI2C_WR_SCFGR1(base, configValue);
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SetAddress7bit
 * Description   : Sets the primary 7-bit slave address.
 *
 *END**************************************************************************/
void LPI2C_HAL_SetAddress7bit(LPI2C_Type * base, uint8_t address)
{
    /* Set 7-bit slave address.*/
//    uint32_t addr = (((uint32_t)address) << 1U);
    LPI2C_BWR_SAMR_ADDR0(base, address);
    
    /* Selecting 7-bit mode.*/
    LPI2C_BWR_SCFGR1_ADDRCFG(base, 0U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SetAddress10bit
 * Description   : Sets the primary slave address and enables 10-bit address mode.
 *
 *END**************************************************************************/
void LPI2C_HAL_SetAddress10bit(LPI2C_Type * base, uint16_t address)
{
    /* Set 10-bit slave address.*/
    uint32_t addr = (((uint32_t)address) << 1U);
    LPI2C_BWR_SAMR_ADDR0(base, addr);
    
    /* Selecting 10-bit mode.*/
    LPI2C_BWR_SCFGR1_ADDRCFG(base, 1U);
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SetMasterHostRequestMode
 * Description   : Enable or disable the host request feature and configures the source
 * and polarity of host request source.
 *
 *END**************************************************************************/
void LPI2C_HAL_SetMasterHostRequestMode(LPI2C_Type *base,
                                         lpi2c_master_hostreq_sel_t hostReqInput,
                                         lpi2c_master_hostreq_config_t hostReqPol,
                                         bool enable)
{
    uint32_t configValue;
    configValue = LPI2C_RD_MCFGR0(base);
    configValue = configValue &
                (~(LPI2C_MCFGR0_HREN_MASK|LPI2C_MCFGR0_HRPOL_MASK|LPI2C_MCFGR0_HRSEL_MASK));
    configValue |= (uint32_t) (enable) |(uint32_t)(hostReqPol << LPI2C_MCFGR0_HRPOL_SHIFT) |
                   (uint32_t) (hostReqInput << LPI2C_MCFGR0_HRSEL_SHIFT);
    LPI2C_WR_MCFGR0(base,configValue);
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SetMasterMatchConfigMode
 * Description   : Configures the LPI2C master data match configuration mode, the LPI2C will
 * assert DMF status flag if the data match condition is met. 
 *
 *END**************************************************************************/
void LPI2C_HAL_SetMasterMatchConfigMode(LPI2C_Type *base,
                                         lpi2c_master_match_config_t matchMode,
                                         bool rxDataMatchOnly,
                                         uint32_t match0,
                                         uint32_t match1)
{
    /* Disable master mode. */
    LPI2C_HAL_SetMasterCmd(base, 0U);
    LPI2C_BWR_MCFGR1_MATCFG(base,matchMode);
    LPI2C_BWR_MCFGR0_RDMO(base,rxDataMatchOnly);
    LPI2C_BWR_MDMR_MATCH0(base,match0);
    LPI2C_BWR_MDMR_MATCH1(base,match1);
    /* Disable master mode. */
    LPI2C_HAL_SetMasterCmd(base, 1U);
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_MasterWaitTransfer
 * Description   : Delay some clock cycles to wait LPI2C master transfer ends.
 *
 *END**************************************************************************/
void LPI2C_HAL_MasterWaitTransfer(LPI2C_Type * base)
{
    uint32_t clkLo, clkHi, prescaler, delayCycles;
    prescaler = lpi2c_prescaler[LPI2C_BRD_MCFGR1_PRESCALE(base)];
    clkLo = LPI2C_BRD_MCCR0_CLKLO(base) + 1;
    clkHi = LPI2C_BRD_MCCR0_CLKHI(base) + 1;
    delayCycles = ((clkLo+clkHi)*prescaler*18)/5;
    for(int i = 0; i < delayCycles*4; i++)
    {}
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_MasterReadByteBlocking
 * Description   : wait until there is a rx ready flag and read one byte from rx fifo.
 *
 *END**************************************************************************/
uint8_t LPI2C_HAL_MasterReadByteBlocking(LPI2C_Type * base)
{
    /* Wait until there is a rx ready request. */
    while ((!LPI2C_HAL_MasterGetStatus(base, kLpi2cMasterRxReadyFlag))&
           (LPI2C_HAL_MasterGetRxFifoCount(base) == 0))
    {}

    /* Read byte from LPI2C data register. */
    uint8_t byte = LPI2C_RD_MRDR(base);

    return byte;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_MasterWriteByteBlocking
 * Description   : Wait till a tx request flag is set and writes one byte of data 
 * to the tx fifo.
 *
 *END**************************************************************************/
bool LPI2C_HAL_MasterWriteByteBlocking(LPI2C_Type * base, uint8_t byte)
{
    /* Check if there's error during last transfer. */
    if(LPI2C_HAL_MasterGetStatus(base, kLpi2cMasterFifoErrFlag)|
       LPI2C_HAL_MasterGetStatus(base, kLpi2cMasterNackDetectFlag))
    {
      return false;
    }
    else
    {
      /* Wait until there is a tx request. */
      while (!LPI2C_HAL_MasterGetStatus(base, kLpi2cMasterTxRequestFlag))
      {
      }

      /* Write byte into LPI2C master data register. */
      LPI2C_WR_MTDR(base, byte);
      return true;
    }
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SlaveReadByteBlocking
 * Description   : Wait till a rx ready flag is set and read one byte from rx reg
 *
 *END**************************************************************************/
uint8_t  LPI2C_HAL_SlaveReadByteBlocking(LPI2C_Type * base)
{
    /* Wait until there is a rx ready. */
    while (!LPI2C_HAL_SlaveGetStatus(base, kLpi2cSlaveRxReadyFlag))
    {}
    /* Read byte from LPI2C data register. */
    uint8_t byte = LPI2C_RD_SRDR(base); 
    return byte;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SlaveWriteByteBlocking
 * Description   : Writes one byte of data to the LPI2C bus and wait till that byte is
 * transfered successfully.
 *
 *END**************************************************************************/
bool LPI2C_HAL_SlaveWriteByteBlocking(LPI2C_Type * base, uint8_t byte)
{
    /* Write byte into LPI2C slave data register. */
    LPI2C_WR_STDR(base, byte); 
    /* Wait until the transmit complete and tx request flag set. */
    while (!LPI2C_HAL_SlaveGetStatus(base, kLpi2cSlaveTxRequestFlag))
    {}
    if(LPI2C_HAL_SlaveGetStatus(base, kLpi2cSlaveBitErrFlag)|
       LPI2C_HAL_SlaveGetStatus(base, kLpi2cSlaveFifoErrFlag))
    {
      return false;
    }
    else
    {
      return true;
    }
}
/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_MasterReceiveDataPolling
 * Description   : Performs a polling receive transaction on the LPI2C bus.
 *
 *END**************************************************************************/
lpi2c_status_t LPI2C_HAL_MasterReceiveDataPolling(LPI2C_Type * base,
                                                  uint16_t slaveAddr,
                                                  const uint8_t * cmdBuff,
                                                  uint32_t cmdSize,
                                                  uint8_t * rxBuff,
                                                  uint32_t rxSize)
{
    bool is10bitAddr;
    uint8_t addrByte1, addrByte2;
    lpi2c_direction_t direction;
    /* Return if current LPI2C is already set as master. */
//    if (LPI2C_HAL_MasterGetStatus(base, kLpi2cMasterBusyFlag))
//    {
//        return kStatus_LPI2C_Busy;
//    }
    
    /* Check to see if slave address is 10 bits or not. */
    is10bitAddr = ((slaveAddr >> 10U) == 0x1EU) ? true : false;

    /* Get address byte 1 and byte 2 according address bit number. */
    if (is10bitAddr)
    {
        addrByte1 = (uint8_t)(slaveAddr >> 8U);
        addrByte2 = (uint8_t)slaveAddr;
    }
    else
    {
        addrByte1 = (uint8_t)slaveAddr;
    }
    direction = is10bitAddr||cmdBuff ? kLPI2CWrite : kLPI2CRead;
    /* Generate START signal, and send out 7-bit address. */
    LPI2C_HAL_SendStart(base, addrByte1, direction);
    /* Wait transfer ends*/
    LPI2C_HAL_MasterWaitTransfer(base);
    /* Return if no acknowledge is detected. */
    if (LPI2C_HAL_MasterGetStatus(base, kLpi2cMasterNackDetectFlag))
    {
        LPI2C_HAL_MasterClearStatus(base, kLpi2cMasterNackDetectFlag);
        return kStatus_LPI2C_ReceivedNak;
    }

    /* Send out second address byte if in 10-bit address mode*/
    if (is10bitAddr)
    {
        if (!LPI2C_HAL_MasterWriteByteBlocking(base, addrByte2))
        {
            /* Send STOP if no ACK received */
            LPI2C_HAL_SendStop(base);
            return kStatus_LPI2C_ReceivedNak;
        }
    }

    /* Send CMD buffer */
    if (cmdBuff != NULL)
    {
        while (cmdSize--)
        {
            if (!LPI2C_HAL_MasterWriteByteBlocking(base, *cmdBuff++))
            {
                /* Send STOP if no ACK received */
                LPI2C_HAL_SendStop(base);
                return kStatus_LPI2C_ReceivedNak;
            }
        }
    }
    /*--------------- Send Address Again ------------------*/
    /* Send slave address again if receiving data from 10-bit address slave,
       OR conducting a cmd receive */
    if (is10bitAddr || cmdBuff)
    {
        /* Need to generate a repeat start before changing to receive. */
        LPI2C_HAL_SendStart(base, addrByte1, kLPI2CRead);
        /* Wait transfer ends*/
        LPI2C_HAL_MasterWaitTransfer(base);
        /* Return if no acknowledge is detected. */
        if (LPI2C_HAL_MasterGetStatus(base, kLpi2cMasterNackDetectFlag))
        {
            LPI2C_HAL_MasterClearStatus(base, kLpi2cMasterNackDetectFlag);
            return kStatus_LPI2C_ReceivedNak;
        }
    }

    /* Initialize receiving of rxSize byte. */
    LPI2C_HAL_MasterSetRxMode(base, rxSize);

    /* Receive data */
    for(int i=0; i<rxSize; i++)
    {
        *rxBuff++ = LPI2C_HAL_MasterReadByteBlocking(base);
    }

    /* Send STOP */
    LPI2C_HAL_SendStop(base);

    return kStatus_LPI2C_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_MasterSendDataPolling
 * Description   : Performs a polling send transaction on the LPI2C bus.
 *
 *END**************************************************************************/
lpi2c_status_t LPI2C_HAL_MasterSendDataPolling(LPI2C_Type * base,
                                               uint16_t slaveAddr,
                                               const uint8_t * cmdBuff,
                                               uint32_t cmdSize,
                                               const uint8_t * txBuff,
                                               uint32_t txSize)
{
    uint8_t addrByte1, addrByte2;
    bool is10bitAddr;

    /* Return if current LPI2C is already set as master. */
//    if (LPI2C_HAL_MasterGetStatus(base, kLpi2cMasterBusyFlag))
//    {
//        return kStatus_LPI2C_Busy;
//    }
    
    /* Check to see if slave address is 10 bits or not. */
    is10bitAddr = ((slaveAddr >> 10U) == 0x1EU) ? true : false;

    /* Get address byte 1 and byte 2 according address bit number. */
    if (is10bitAddr)
    {
        addrByte1 = (uint8_t)(slaveAddr >> 8U);
        addrByte2 = (uint8_t)slaveAddr;
    }
    else
    {
        addrByte1 = (uint8_t)slaveAddr;
    }

    /* Generate START signal, and send out 7-bit address. */
    LPI2C_HAL_SendStart(base, addrByte1, kLPI2CWrite);
    /* Wait transfer ends*/
    LPI2C_HAL_MasterWaitTransfer(base);
    /* Return if no acknowledge is detected. */
    if (LPI2C_HAL_MasterGetStatus(base, kLpi2cMasterNackDetectFlag))
    {
        LPI2C_HAL_MasterClearStatus(base, kLpi2cMasterNackDetectFlag);
        return kStatus_LPI2C_ReceivedNak;
    }

    /* Send out second address byte if in 10-bit address mode*/
    if (is10bitAddr)
    {
        if (!LPI2C_HAL_MasterWriteByteBlocking(base, addrByte2))
        {
            /* Send STOP if no ACK received */
            LPI2C_HAL_SendStop(base);
            return kStatus_LPI2C_ReceivedNak;
        }
    }

    /* Send CMD buffer */
    if (cmdBuff != NULL)
    {
        while (cmdSize--)
        {
            if (!LPI2C_HAL_MasterWriteByteBlocking(base, *cmdBuff++))
            {
                /* Send STOP if no ACK received */
                LPI2C_HAL_SendStop(base);
                return kStatus_LPI2C_ReceivedNak;
            }
        }
    }

    /* Send data buffer */
    while (txSize--)
    {
        if (!LPI2C_HAL_MasterWriteByteBlocking(base, *txBuff++))
        {
            /* Send STOP if no ACK received */
            LPI2C_HAL_SendStop(base);
            return kStatus_LPI2C_ReceivedNak;
        }
    }

    /* Generate STOP signal. */
    LPI2C_HAL_SendStop(base);

    return kStatus_LPI2C_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SlaveSendDataPolling
 * Description   : Send out multiple bytes of data using polling method.
 *
 *END**************************************************************************/
lpi2c_status_t LPI2C_HAL_SlaveSendDataPolling(LPI2C_Type * base,
                                              const uint8_t* txBuff,
                                              uint32_t txSize)
{
    /* Wait until addressed as a slave */
    while(!LPI2C_HAL_SlaveGetStatus(base, kLpi2cSlaveAddress0MatachFlag))
    {}
    LPI2C_RD_SASR(base);
    /* While loop to transmit data */
    while(txSize--)
    {
        /* Write byte to data register */
        if (!LPI2C_HAL_SlaveWriteByteBlocking(base, *txBuff++))
        {
            return kStatus_LPI2C_Error;
        }
    }

    return kStatus_LPI2C_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPI2C_HAL_SlaveReceiveDataPolling
 * Description   : Receive multiple bytes of data using polling method.
 *
 *END**************************************************************************/
lpi2c_status_t LPI2C_HAL_SlaveReceiveDataPolling(LPI2C_Type * base,
                                                 uint8_t *rxBuff,
                                                 uint32_t rxSize)
{
    /* Wait until addressed as a slave */
    while(!LPI2C_HAL_SlaveGetStatus(base, kLpi2cSlaveAddress0MatachFlag))
    {}
    LPI2C_RD_SASR(base);
    /* While loop to receive data */
    while(rxSize--)
    {
        /* Read byte from data register  */
        *rxBuff++ = LPI2C_HAL_SlaveReadByteBlocking(base);
    }

    return kStatus_LPI2C_Success;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

