#ifndef Reverb_Comb_H
#define Reverb_Comb_H

#include "Q15T.h"

class Reverb_Comb
{
private:

	Q15T			Feedback;
	int				Filter_Store;
	Q15T			Damp_1;
	Q15T			Damp_2;
	int16_t		*Buffer;
	int				Buf_Size;
	int				Buf_Index;

public:

	Reverb_Comb()
	{
		Filter_Store	= 0.f;
		Buf_Index			= 0;
	}

	template <typename T>
	void Set_Buffer( T& buf )
	{
		Buffer 		= &buf[ 0 ];
		Buf_Size	= sizeof( buf ) / sizeof( buf[ 0 ] );
	}

	int Process( int input )
	{
		int output = Buffer[ Buf_Index ];

		Filter_Store = ( ( Damp_2 * output ) + ( Damp_1 * Filter_Store ) ).to_int();


		int v = input + ( Feedback * Filter_Store ).to_int();

		v = LIMIT_INT16( v );

		Buffer[ Buf_Index ] = v;

		if( ++Buf_Index >= Buf_Size ) Buf_Index = 0;

		return output;
	}

	void Clear()
	{
		for( int i( 0 ); i < Buf_Size; ++i )
			Buffer[ i ] = 0;
	}

	void	Set_Damp( const Q15T& v )			{ Damp_1 = v; Damp_2 = Q15T( 1 )-v; }
	Q15T	Get_Damp()										{ return Damp_1; }

	void	Set_Feedback( const Q15T& v )	{ Feedback = v; }
	Q15T	Get_Feedback() const					{ return Feedback; }
};

#endif
