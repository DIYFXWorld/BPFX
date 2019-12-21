#ifndef	Reverse_Delay_Buffer_PCMU_h
#define	Reverse_Delay_Buffer_PCMU_h

#include	<stdint.h>
#include	"PCMU.h"

struct Reverse_Delay_Buffer_PCMU
{
	MEMORY_ALLOCATOR<uint8_t>	Memory;

  int	Length;
	int	Pointer;
	int	Direction;

	Reverse_Delay_Buffer_PCMU( uint32_t memory_length )
	: Memory( memory_length ),
		Length( Memory.Length ), Pointer( 0 ), Direction( 1 )
	{
	}

 	void Reset()
 	{
 		Pointer = 0;
 		Direction = 1;
 	}

	void Set_Length( int new_length )
	{
		if( new_length > ( int )Memory.Length ) new_length = Memory.Length-1;
		if( new_length <= 0 )				  	 				new_length = 1;

		if( new_length < Length )
		{
			for( uint8_t* ptr = Memory.Pointer + new_length; ptr < Memory.Pointer + Length; ++ptr )
			{
				*ptr = 0;
			}
		}

		Length = new_length;

		if( Pointer >= Length )	Pointer = Length-1;
	}

	void Set_Value( const int16_t& v )
	{
		if( Pointer >= Length )	Reset();
		if( Pointer <       0 )	Reset();

		Memory[ Pointer ] = PCMU_Encode( v );
		Pointer += Direction;

		     if( Pointer >= Length )	{ Pointer = Length-1; Direction = -1; }
		else if( Pointer <= -1 )			{ Pointer = 0;        Direction =  1; }
	}

	int16_t Get_Value() const { return PCMU_Decode( Memory[ Pointer ] ); }

//	void Set_Value( uint32_t index, const int16_t& v ) { Memory[ index ] = PCMU_Encode( v ); }
//	int16_t Get_Value( uint32_t index ) const { return PCMU_Decode( Memory[ index ] ); }

/*	void Clear()
	{
		uint8_t c = PCMU_Encode( 0 );

		for( int i( 0 ); i < Length; ++i )
			Memory[ i ] = c;
 		Pointer = 0;
 		Direction = 1;
	}
*/
	void Clear()
	{
		Memory.Reset( PCMU_Encode( 0 ) );
 		Pointer = 0;
 		Direction = 1;
	}
};

#endif
