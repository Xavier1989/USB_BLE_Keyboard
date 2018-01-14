/**
 ****************************************************************************************
 *
 * @file trng.h
 *
 * @brief True Random Number Generator header file.
 *
 * Copyright (C) 2014. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef TRNG_H_
#define TRNG_H_

/**
****************************************************************************************
* @brief  Acquires 128 bits, saves them in trng_bits[]and restores the modified regs
****************************************************************************************
*/
void trng_acquire(uint8_t *trng_bits_ptr);

#endif // TRNG_H_
