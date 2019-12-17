#include	"Q15T_BQF.h"

Q15T_BQF& Q15T_BQF::operator = ( const BQF_Param& v )
{
	b0_a0 = Q15T( v.b0 / v.a0 );
	b1_a0 = Q15T( v.b1 / v.a0 );
	b2_a0 = Q15T( v.b2 / v.a0 );
	a1_a0 = Q15T( v.a1 / v.a0 );
	a2_a0 = Q15T( v.a2 / v.a0 );

	return *this;
}

int Q15T_BQF::Process( int input )
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

	return output.to_int();
}

/*
#define	asmv	asm volatile

int Q15T_BQF::Process( int input )
{
	int	i = input << 15;

	asmv(	"mov   r4, #0" );	// r4 = 0
	asmv(	"mov   r5, #0" );	// r5 = 0
	asmv( "smlal r4, r5, %[i0], %[i1]"::[i0] "r" (b0_a0.Value), [i1] "r" (i) );
	asmv( "smlal r4, r5, %[i0], %[i1]"::[i0] "r" (b1_a0.Value), [i1] "r" (in1.Value) );
	asmv( "smlal r4, r5, %[i0], %[i1]"::[i0] "r" (b2_a0.Value), [i1] "r" (in2.Value) );
	asmv( "smlal r4, r5, %[i0], %[i1]"::[i0] "r" (a1_a0.Value), [i1] "r" (out1.Value) );
	asmv( "smlal r4, r5, %[i0], %[i1]"::[i0] "r" (a2_a0.Value), [i1] "r" (out2.Value) );
	asmv( "lsr   r4, r4, #15" );	// r4 >> 15
	asmv( "lsl   r5, r5, #17" );	// r5 << 15
	asmv( "orr   r4, r4, r5" );		// r4 = r4 | r5
	asmv( "str   r4, [%0]"::"r" (&output.Value) );		// output = r4

	in2  = in1;
	in1  = Q15T( input );

	out2 = out1;
	out1 = output * -1;

	return output.to_int();
}
*/

void Q15T_BQF::Reset()
{
	in1 = Q15T( 0 );
	in2 = Q15T( 0 );
	out1 = Q15T( 0 );
	out2 = Q15T( 0 );
}
