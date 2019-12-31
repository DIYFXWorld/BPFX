#ifndef	Delay_0_Buffer_h
#define	Delay_0_Buffer_h

#include 	"Array.h"
#include 	"Q15T.h"
#include	<stdint.h>
#include "FX_Config.h"

struct Delay_0_Buffer
{
	Array<int16_t,FX_DELAY_0_BUFFER_LENGTH>	Memory;

	uint32_t	Write_Pointer;
	Q15T			Read_Pointer;
	uint32_t	Length;

	Delay_0_Buffer():
		Write_Pointer( 0 ), Read_Pointer( 0.0 ), Length( Memory.Length )
	{
	}

	void Set_Value( const int16_t& v )
	{
		Memory[ Write_Pointer ] = v;

		++Write_Pointer;

		if( Write_Pointer == Memory.Length )	Write_Pointer = 0;
	}

	int16_t Get_Value()
	{
		int	index = Read_Pointer.to_int();

		int16_t	v = Memory[ index ];

		Read_Pointer += ( Q15T( Distance() ) / Length );

		if( ( uint32_t )Read_Pointer.to_int() >= Memory.Length )	Read_Pointer = Read_Pointer - Memory.Length;

		return v;
	}

	void Set_Length( uint32_t v )
	{
		if( v < 1 )	v = 1;
		if( v > Memory.Length - 1 )	v = Memory.Length - 1;
		Length = v;
	}

	void Fast_Forward()
	{
		Read_Pointer = Q15T( int( Write_Pointer - Length ) );
		if( Read_Pointer.to_int() < 0 )	Read_Pointer = Read_Pointer + Memory.Length;
	}

	int Distance() const
	{
		int	v = Write_Pointer - Read_Pointer.to_int();
		if( v < 0 )	v += Memory.Length;
		return v;
	}
};

#endif