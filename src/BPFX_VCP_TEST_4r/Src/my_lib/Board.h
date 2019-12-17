#ifndef	Board_h
#define	Board_h

#include	"hal.h"
#include	"hal_gpio.h"

#define	USER_LED_ON()				HAL_GPIO_WritePin( GPIOC, GPIO_PIN_13, GPIO_PIN_RESET )
#define	USER_LED_OFF()			HAL_GPIO_WritePin( GPIOC, GPIO_PIN_13, GPIO_PIN_SET )
#define	USER_LED_TOGGLE()		HAL_GPIO_TogglePin( GPIOC, GPIO_PIN_13 )

#define	Get_Unique_ID()			( *( uint8_t* )( 0x1FFFF800+4 ) )


inline unsigned char Get_User_Byte_0()
{
	return *( ( unsigned char* )( 0x1FFFF800+4 ) );
}

inline unsigned char Get_User_Byte_1()
{
	return *( ( unsigned char* )( 0x1FFFF800+6 ) );
}

#endif
