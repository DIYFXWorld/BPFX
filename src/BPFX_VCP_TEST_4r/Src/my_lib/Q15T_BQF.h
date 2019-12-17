#ifndef	Q15T_BQF_h
#define	Q15T_BQF_h

#include	"Q15T.h"
#include	"BQF.h"

struct Q15T_BQF
{
	Q15T		b0_a0, b1_a0, b2_a0;
	Q15T		a1_a0, a2_a0;
	Q15T		temp;

	Q15T		in1, in2, out1, out2;
	Q15T		output;

public:

	Q15T_BQF() :
		in1( 0 ), in2( 0 ), out1( 0 ), out2( 0 )
	{}

	Q15T_BQF( const BQF_Param& v ) :
		in1( 0 ), in2( 0 ), out1( 0 ), out2( 0 )
	{ *this = v; }

	int Process( int input );

	Q15T_BQF& operator = ( const BQF_Param& v );

	void Reset();
};

#endif
