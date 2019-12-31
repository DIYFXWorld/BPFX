#ifndef __PCMU_H
#define __PCMU_H

#include "stm32f1xx_hal.h"

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t PCMU_Encode( int16_t sample );
int16_t PCMU_Decode( uint8_t code );

#ifdef	__cplusplus
} // extern "C"
#endif

#endif /* __PCMU_H*/

