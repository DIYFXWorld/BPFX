#ifndef	Chorus_Buffer_h
#define	Chorus_Buffer_h

#include "Array.h"
#include "Q15T.h"
#include "FX_Config.h"

struct Chorus_Buffer
{
	Array<int16_t>	Memory;

	uint32_t	Length;
	uint32_t	Write_Pointer;
	Q15T			Distance;

	Chorus_Buffer( uint32_t length ):
		Memory( length ),
		Length( length ),
		Write_Pointer( 0 ),
		Distance( ( int )length )
	{
	}

	void Set_Value( const int16_t& v )
	{
		Memory[ Write_Pointer ] = LIMIT_INT16( v );

		++Write_Pointer;

		if( Write_Pointer == Memory.Length )	Write_Pointer = 0;
	}

	int16_t Get_Value( const Q15T& distance )
	{
		int		idx0 = distance.Value >> 15;
		int		dec0 = distance.Value & 0x7FFF;
		int		dec1 = 32767 - dec0;

		idx0 = Write_Pointer - idx0;
		if( idx0 < 0 )	idx0 += Memory.Length;

		int		idx1 = idx0 + 1;
		if( idx1 == ( int )Memory.Length )	idx1 = 0;

		return Memory[ idx0 ] * dec0 / 32768 + Memory[ idx1 ] * dec1 / 32768;
	}

	int16_t Get_Value()
	{
		Distance += ( Q15T( ( int )Length ) - Distance ) / Length;
		return Get_Value( Distance );
	}

	void Set_Length( uint32_t v )
	{
		if( v < 1 )	v = 1;
		if( v > Length - 1 )	v = Memory.Length - 1;
		Length = v;
	}

	void Fast_Forward() { Distance = Q15T( ( int )Length ); }
};

#endif
