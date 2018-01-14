/**
 ****************************************************************************************
 *
 * @file app_scppc.h
 *
 * @brief Scan Parameters Profile Client - Entry point header file.
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

extern struct scps_content scps;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_scppc_init(void);
void app_scppc_enable_prf(uint16_t);
