#ifndef	Array_h
#define	Array_h

#include	<stdint.h>

template <typename T>
struct Array
{
	T*				Pointer;
	uint32_t	Length;

	Array() : Pointer( 0 ), Length( 0 )	{}

	Array( uint32_t length ) : Pointer( 0 ), Length( length )
	{
		Create( Length );
	}

	~Array()
	{
		Destroy();
	}

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
			Pointer = new T[ Length ];
		}
	}

	void Reset( const T& v = T() )
	{
		uint32_t	i( Length - 1 );

		while( i )
		{
			Pointer[ i ] = v;
			--i;
		}

		Pointer[ 0 ] = v;
	}

	      T& operator [] ( int i )       { return Pointer[ i ]; }
	const T& operator [] ( int i ) const { return Pointer[ i ]; }
};

template <typename T, uint32_t length>
struct Fixed_Array
{
	T					Pointer[ length ];
	uint32_t	Length;
	uint32_t	Counter;
	
	Fixed_Array():
		Length( length ), Counter( 0 )
	{
	}

	void Push_Back( T v )
	{
		Pointer[ Counter ] = v;
		++Counter;
	}

		    T& operator [] ( int idx )       { return Pointer[ idx ]; }
	const T& operator [] ( int idx ) const { return Pointer[ idx ]; }
};
#endif

#include	"Memory_Pool.h"

#ifndef	Array_Pool_h
#define	Array_Pool_h

template <typename T>
struct Array_Pool
{
	T*				Pointer;
	uint32_t	Length;

	Array_Pool():
		Pointer( 0 ),
		Length( 0 )
	{
	}

	Array_Pool( uint32_t length ):
		Pointer( 0 ),
		Length( length )
	{
		Create( Length );
	}

	~Array_Pool()
	{
		Destroy();
	}

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
			Pointer = ( T* )__Malloc__( sizeof( T ) * Length );
		}
	}

	void Reset( const T& v = T() )
	{
		uint32_t	i( Length - 1 );

		while( i )
		{
			Pointer[ i ] = v;
			--i;
		}

		Pointer[ 0 ] = v;
	}

	      T& operator [] ( int i )       { return Pointer[ i ]; }
	const T& operator [] ( int i ) const { return Pointer[ i ]; }
};

#endif

