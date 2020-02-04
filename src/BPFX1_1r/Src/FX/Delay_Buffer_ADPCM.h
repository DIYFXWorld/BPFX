#ifndef	Delay_Buffer_ADPCM_h
#define	Delay_Buffer_ADPCM_h

#include 	"Array_4bit.h"
#include 	"Q15T.h"
#include	<stdint.h>
#include	"ADPCM_IMA.h"
//#include	"ADPCM_YM2608.h"

struct Delay_Buffer_ADPCM
{
	MEMORY_ALLOCATOR_4BIT	Memory;
	uint32_t				Write_Pointer;
	uint32_t				Length;

	ADPCM_IMA_Encoder		Encode;
	ADPCM_IMA_Decoder		Decode;

	Delay_Buffer_ADPCM( uint32_t length ) :
		Memory( length ),
		Write_Pointer( 0 ),
		Length( length )
	{
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

		return Decode( Memory.Get_Value( i ) );
	}

	void Set_Length( uint32_t v )
	{
		if( v < 1 )	v = 1;
		if( v > Memory.Length - 1 )	v = Memory.Length - 1;
		Length = v;
		Reset();
	}

	void Reset( const int& v = 0 )
	{
		Encode.Reset();
		Decode.Reset();
		Memory.Reset( Encode( LIMIT_INT16( v ) ) );
	}
};

#endif
