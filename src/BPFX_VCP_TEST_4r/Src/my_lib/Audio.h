#ifndef	Audio_h
#define	Audio_h

#include	"Board.h"	// for User Bytes

void Audio_Input_Callback( int );
void Audio_Invalid_Callback( int );

struct Audio
{
	static const int	Sampling_Rate;
	static bool				Input_Active;
//	static int				DC_BIAS;

	static void Input_Start();
	static void Output_Start();

	static void Start()
	{
		Output_Start();
		Input_Start();
	}

	static void Set_Output( int );
/*
	static void Set_DC_BIAS()
	{
		uint8_t	v = *( ( uint8_t* )0x1FFFF806 );

		if( v != 0xFF )
		{
			if( v < 0x80 )	DC_BIAS = ( v-  0 ) *  100;
			else						DC_BIAS = ( v-127 ) * -100;
		}
		else DC_BIAS = 0;
	}*/
};

#endif
