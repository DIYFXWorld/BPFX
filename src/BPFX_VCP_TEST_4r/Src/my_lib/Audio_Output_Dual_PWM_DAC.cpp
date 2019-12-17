#include	"Audio.h"
#include	"hal.h"
#include	"hal_tim.h"
#include	"Common.h"

extern TIM_HandleTypeDef htim3;

#define	HTIM			htim3
#define	TIM_CH_1	TIM_CHANNEL_1
#define	TIM_CH_2	TIM_CHANNEL_2

void Audio::Output_Start()
{
  HAL_TIM_PWM_Start( &HTIM, TIM_CH_1 );
  HAL_TIM_PWM_Start( &HTIM, TIM_CH_2 );
}

void Audio::Set_Output( int v )
{
	v = LIMIT_INT16( v );

	v += 32768;

	HTIM.Instance->CCR1 = LO_BYTE( v );
	HTIM.Instance->CCR2 = HI_BYTE( v );
}
