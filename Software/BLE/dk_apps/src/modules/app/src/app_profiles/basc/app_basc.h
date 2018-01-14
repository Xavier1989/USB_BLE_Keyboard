/**
 ****************************************************************************************
 *
 * @file app_basc.h
 *
 * @brief BAS Client - Entry point header file.
 *
 * Copyright (C) 2013. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */
#include "rwble_config.h"

#include "app.h"                      // application definitions
#include "app_task.h"                 // application task

#include "basc.h"

extern uint8_t bas_nb;
extern struct bas_content bas[BASC_NB_BAS_INSTANCES_MAX];

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_basc_init(void);
void app_basc_enable_prf(uint16_t);
