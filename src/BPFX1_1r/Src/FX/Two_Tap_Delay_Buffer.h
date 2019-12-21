#ifndef	Two_Tap_Delay_Buffer_h
#define	Two_Tap_Delay_Buffer_h

#include 	"Array_4bit.h"
#include 	"Q15T.h"
#include	<stdint.h>
#include	"ADPCM_IMA.h"
//#include	"ADPCM_YM2608.h"

struct Two_Tap_Delay_Buffer
{
	MEMORY_ALLOCATOR_4BIT	Memory;
	uint32_t		Write_Pointer;
	uint32_t		Length;

	ADPCM_IMA_Encoder		Encoder;
	ADPCM_IMA_Decoder		Decoder_0, Decoder_1;

	Two_Tap_Delay_Buffer( uint32_t memory_length ) :
		Memory( memory_length ), // Number of Element
		Write_Pointer( 0 ), Length( 0 )
	{
	}

	void Set_Value( const int16_t& v )
	{
		Memory.Set_Value( Write_Pointer, Encoder.Process( v ) );
		++Write_Pointer;
		if( Write_Pointer == Memory.Length )	Write_Pointer = 0;
	}

	int16_t Get_Value()
	{
		int	i = ( int )Write_Pointer - Length;
		if( i < 0 )	i = Memory.Length + i;

		int v = Decoder_0.Process( Memory.Get_Value( i ) );

		i = ( int )Write_Pointer - ( Length / 2 );
		if( i < 0 )	i = Memory.Length + i;

		v += Decoder_1.Process( Memory.Get_Value( i ) );

		v = LIMIT_INT16( v );

		return v;
	}

	void Set_Length( uint32_t v )
	{
		if( v < 1 )	v = 1;
		if( v > Memory.Length - 1 )	v = Memory.Length - 1;
		Length = v;
	}

	void Reset( const int16_t& v = 0 )
	{
		Encoder.Reset();
		Decoder_0.Reset();
		Decoder_1.Reset();

		Memory.Reset( Encoder.Process( v ) );
	}
};

#endif
