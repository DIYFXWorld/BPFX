#ifndef	Two_Tap_Delay_Buffer_h
#define	Two_Tap_Delay_Buffer_h

#include 	"Array_4bit.h"
#include 	"Q15T.h"
#include	<stdint.h>
#include	"ADPCM_IMA.h"
#include	"Myutil.h"

struct Two_Tap_Delay_Buffer
{
	MEMORY_ALLOCATOR_4BIT	Memory;
	uint32_t		Write_Pointer;
	uint32_t		Length;

	ADPCM_IMA_Encoder		Encode;
	ADPCM_IMA_Decoder		Decode_0, Decode_1;

	Two_Tap_Delay_Buffer( uint32_t length ):
		Memory( length ),
		Write_Pointer( 0 ),
		Length( length )
	{
		Length = Limit<uint32_t>( 1, Length, Memory.Length - 1 );
	}

	void Set_Value( const int& v )
	{
		Memory.Set_Value( Write_Pointer, Encode( LIMIT_INT16( v ) ) );
		++Write_Pointer;
		if( Write_Pointer == Memory.Length )	Write_Pointer = 0;
	}

	int16_t Get_Value()
	{
		int	i = ( int )Write_Pointer - Length;
		if( i < 0 )	i = Memory.Length + i;

		int v = Decode_0( Memory.Get_Value( i ) );

		i = ( int )Write_Pointer - ( Length / 2 );
		if( i < 0 )	i = Memory.Length + i;

		v -= Decode_1( Memory.Get_Value( i ) );

		return v;
	}

	void Set_Length( uint32_t v )
	{
		Length = Limit<uint32_t>( 1, v, Memory.Length - 1 );
		Reset();
	}

	void Reset( const int& v = 0 )
	{
		Encode.Reset();
		Decode_0.Reset();
		Decode_1.Reset();
		Memory.Reset( Encode( LIMIT_INT16( v ) ) );
	}
};

#endif
