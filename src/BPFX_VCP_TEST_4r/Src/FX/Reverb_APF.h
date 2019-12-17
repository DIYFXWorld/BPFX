#ifndef _Reverb_APF_
#define _Reverb_APF_

#include	"Q15T.h"

class Reverb_APF
{
	int16_t		*Buffer;
	int				Buf_Size;
	int				Buf_Index;

public:

	Reverb_APF()
	{
		Buf_Index = 0;
	}

	template <typename T>
	void Set_Buffer( T& buf )
	{
		Buffer 		= &buf[ 0 ];
		Buf_Size	= sizeof( buf ) / sizeof( buf[ 0 ] );
	}

	int	Process( int input )
	{
		int bufout = Buffer[ Buf_Index ];
		int	output = -input + bufout;

		int	v = input + ( bufout / 2 );

		v = LIMIT_INT16( v );

		Buffer[ Buf_Index ] = v;

		if( ++Buf_Index >= Buf_Size ) Buf_Index = 0;

		return output;
	}

	void Mute()
	{
		for( int i( 0 ); i < Buf_Size; ++i )
			Buffer[ i ] = 0;
	}
};

#endif
