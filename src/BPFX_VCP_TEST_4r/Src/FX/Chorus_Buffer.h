#ifndef	Chorus_Buffer_C_h
#define	Chorus_Buffer_C_h

#include	<stdint.h>
#include	"Array.h"

struct Chorus_Buffer
{
	Array<int16_t>	Memory;
	uint32_t	Pointer;
	uint32_t	Max_Length;

	Chorus_Buffer( uint32_t memory_length ) :
		Memory( memory_length ), Pointer( 0 ), Max_Length( Memory.Length ) {}
	
	void Set( const int16_t& v )
	{
		Memory[ Pointer ] = v;
		++Pointer;
		if( Pointer >= Max_Length )	Pointer = 0;
	}

	int16_t Get( int distance ) const
	{
		if( distance < 0 )	distance = 0;
		if( ( uint32_t )distance >= Max_Length )	distance = Max_Length - 1;

		distance = Pointer - distance;

		if( distance < 0 )	distance += Max_Length;

		return Memory[ distance ];
	}
};

#endif
