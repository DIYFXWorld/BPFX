#ifndef	hal_tim_h
#define	hal_tim_h

#if	defined(STM32F103xB) || defined(STM32F103xE)
	#include "stm32f1xx_hal_tim.h"
#endif

#ifdef	STM32F303xC
	#include "stm32f3xx_hal_tim.h"
#endif

#ifdef	STM32L152xE
	#include "stm32l1xx_hal_tim.h"
#endif

#endif
