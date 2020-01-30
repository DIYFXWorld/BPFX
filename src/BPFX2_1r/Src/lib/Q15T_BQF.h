#ifndef	Q15T_BQF_h
#define	Q15T_BQF_h

#include	"Q15T.h"
#include	"BQF.h"

struct Q15T_BQF_Params
{
	Q15T		b0_a0, b1_a0, b2_a0;
	Q15T		a1_a0, a2_a0;

	constexpr Q15T_BQF_Params():
		b0_a0(0), b1_a0(0), b2_a0(0),
		a1_a0(0), a2_a0(0)
	{
	}

	constexpr Q15T_BQF_Params( const BQF_Params& v )
	{
		b0_a0 = Q15T( v.b0 / v.a0 );
		b1_a0 = Q15T( v.b1 / v.a0 );
		b2_a0 = Q15T( v.b2 / v.a0 );
		a1_a0 = Q15T( v.a1 / v.a0 );
		a2_a0 = Q15T( v.a2 / v.a0 );
	}
};

struct Q15T_BQF
{
	Q15T		b0_a0, b1_a0, b2_a0;
	Q15T		a1_a0, a2_a0;

	Q15T		in1, in2, out1, out2;
	Q15T		output;

public:

	constexpr Q15T_BQF():
		in1( 0 ), in2( 0 ), out1( 0 ), out2( 0 )
	{
	}

	Q15T_BQF( const BQF_Params& v ):
		in1( 0 ), in2( 0 ), out1( 0 ), out2( 0 )
	{
		*this = v;
	}

	Q15T_BQF( const Q15T_BQF_Params& v ):
		in1( 0 ), in2( 0 ), out1( 0 ), out2( 0 )
	{
		*this = v;
	}

	Q15T_BQF& operator = ( const BQF_Params& v )
	{
		*this = Q15T_BQF_Params( v );
		return *this;
	}

	Q15T_BQF& operator = ( const Q15T_BQF_Params& v )
	{
		b0_a0 = v.b0_a0;
		b1_a0	= v.b1_a0;
		b2_a0 = v.b2_a0;
		a1_a0	= v.a1_a0;
		a2_a0 = v.a2_a0;
		return *this;
	}

	int operator()( int input )
	{
		output	=       ( b0_a0 * input )
						+ Mul_64( b1_a0, in1    )
						+ Mul_64( b2_a0, in2    )
						+ Mul_64( a1_a0, out1   )
						+ Mul_64( a2_a0, out2   );

		in2  = in1;
		in1  = Q15T( input );

		out2 = out1;
		out1 = output * -1;

		return output.to_round_int();
	}

	void Reset()
	{
		in1.Value = 0;
		in2.Value	= 0;
		out1.Value = 0;
		out2.Value = 0;
	}
};

#endif
