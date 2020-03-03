/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup autobaud_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      lin_lld_autobaud.h

@brief     Functions for Autobaud feature
*****************************************************************************/
 

#ifndef __LIN_LLD_AUTOBAUD
#define __LIN_LLD_AUTOBAUD

#include "lin.h"
#include "lin_lld_sci.h"
#include "lin_hw_cfg.h"
#include "lin_common_proto.h"
#include "lin_lld_timesrv.h"
#include "lin_cfg.h"
#include "lin_lld_autobaud.h"



extern  l_u16       autobaud_tmr_capture[];
extern  l_u8       autobaud_tmr_capture_index;
extern  l_u32       autobaud_bitduration;
extern  l_u16       autobaud_current;


void autobaud_adjustment(void);
void autobaud_init(void);

#endif
