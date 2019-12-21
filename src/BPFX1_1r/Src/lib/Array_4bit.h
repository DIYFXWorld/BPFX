#ifndef	Array_4bit_h
#define	Array_4bit_h

#include	<stdint.h>
#include	"Memory_Pool.h"

struct Array_4bit
{
	uint8_t		*Pointer;
	uint32_t	Length;

	Array_4bit() : Pointer( 0 ), Length( 0 )	{}

	Array_4bit( uint32_t length ) : Pointer( 0 ), Length( length )
	{
		Create( Length );
	}

	~Array_4bit()	{ Destroy(); }

	void Destroy()
	{
		if( Pointer )	delete [] Pointer;
		Pointer = 0;
	}

	void Create( uint32_t length )
	{
		if( length )
		{
			Destroy();
			Length = length;
			Pointer = new uint8_t[ Length / 2 ];
		}
	}

	void Reset( const uint8_t& v = 0 )
	{
		uint32_t	i( Length / 2 - 1 );

		while( i )
		{
			Pointer[ i ] = v;
			--i;
		}

		Pointer[ 0 ] = v;
	}

	void Set_Value( uint32_t i, uint8_t v )
	{
		v = 0x0f & v;
		uint8_t *ptr = Pointer + ( i >> 1 );
		*ptr = ( i & 1 ) ? ( ( *ptr & 0xf0 ) | v ) : ( ( *ptr & 0x0f ) | ( v << 4 ) );
	}

	uint8_t Get_Value( uint32_t i ) const
	{
		const uint8_t	*ptr = Pointer + ( i >> 1 );
		return ( i & 1 ) ? ( *ptr & 0x0f ) : ( *ptr >> 4 );
	}
};

struct Array_Pool_4bit
{
	uint8_t		*Pointer;
	uint32_t	Length;

	Array_Pool_4bit() : Pointer( 0 ), Length( 0 )	{}

	Array_Pool_4bit( uint32_t length ) : Pointer( 0 ), Length( length )
	{
		Create( Length );
	}

	~Array_Pool_4bit()	{ Destroy(); }

	void Destroy()
	{
		if( Pointer )	__Free__( Pointer );
		Pointer = 0;
	}

	void Create( uint32_t length )
	{
		if( length )
		{
			Destroy();
			Length = length;
			Pointer = ( uint8_t* )__Malloc__( Length / 2 );
		}
	}

	void Reset( const uint8_t& v = 0 )
	{
		uint32_t	i( Length / 2 - 1 );

		while( i )
		{
			Pointer[ i ] = v;
			--i;
		}

		Pointer[ 0 ] = v;
	}

	void Set_Value( uint32_t i, uint8_t v )
	{
		v = 0x0f & v;
		uint8_t *ptr = Pointer + ( i >> 1 );
		*ptr = ( i & 1 ) ? ( ( *ptr & 0xf0 ) | v ) : ( ( *ptr & 0x0f ) | ( v << 4 ) );
	}

	uint8_t Get_Value( uint32_t i ) const
	{
		const uint8_t	*ptr = Pointer + ( i >> 1 );
		return ( i & 1 ) ? ( *ptr & 0x0f ) : ( *ptr >> 4 );
	}
};

#endif
