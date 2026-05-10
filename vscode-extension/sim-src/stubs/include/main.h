#pragma once
#define PROS_USE_SIMPLE_NAMES
#define PROS_USE_LITERALS
#include "api.h"

#ifdef __cplusplus
extern "C" {
#endif

void initialize(void);
void disabled(void);
void competition_initialize(void);
void autonomous(void);
void opcontrol(void);

#ifdef __cplusplus
}
#endif
