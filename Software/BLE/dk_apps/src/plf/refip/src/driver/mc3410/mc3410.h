#ifndef _MC3410_H
#define _MC3410_H

#include <stdint.h>

#define MC3410_ENABLE

void mc3410_reserve(void);
void mc3410_config(void);
void mc3410_init(void);
void mc3410_release(void);
void mc3410_dump(void);
void mc3410_output_raw(int16_t *xyz);
void mc3410_output(float *xyz);
void mc3410_wake_init(void);
void mc3410_wake_release(void);

#endif // _MC3410_H
