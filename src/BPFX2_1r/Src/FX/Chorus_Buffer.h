#ifndef	Chorus_Buffer_h
#define	Chorus_Buffer_h

#include <Array.h>
#include <stdint.h>

struct Chorus_Buffer
{
	Array<int16_t>		Memory;
	uint32_t					Pointer;

	Chorus_Buffer( int length ) : Memory( length ), Pointer( 0 ) {}
	
	void Set_Value( const int16_t& v )
	{
		Memory[ Pointer ] = v;
		++Pointer;
		if( Pointer >= Memory.Length )	Pointer = 0;
	}

	int16_t Get_Value( int distance ) const
	{
		if( distance < 0 )	distance = 0;
		if( ( uint32_t )distance >= Memory.Length )	distance = Memory.Length - 1;

		distance = Pointer - distance;

		if( distance < 0 )	distance += Memory.Length;

		return Memory[ distance ];
	}

	void Reset()
	{
		Memory.Reset();
		Pointer = 0;
	}
};

#endif
