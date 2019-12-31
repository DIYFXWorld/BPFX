#ifndef	Q15T_LFO_Tri_h
#define	Q15T_LFO_Tri_h

#include "Q15T.h"
#include "Fraction.h"

#ifndef	Q15T_32767
	constexpr	Q15T	Q15T_32767 (  32767 );
#endif

#ifndef	Q15T_32768M
	constexpr	Q15T	Q15T_32768M( -32768 );
#endif

struct Q15T_LFO_Tri
{
	int		Sampling_Rate;
	Q15T	Step;
	Q15T	Theta;
	int		Phase;

	Q15T_LFO_Tri( int sampling_rate ):
		Sampling_Rate( sampling_rate ),
		Step( 0 ), Theta( 0 ), Phase( 0 )
	{
	}

	void Set_Rate( const Fraction& v )
	{
		Step = Q15T_32767 / ( Sampling_Rate / 4 ) * v;
	}

	Q15T Get_Value()
	{
		Q15T	v = Theta;

		switch( Phase )
		{
			case 0:
			{
				Theta += Step;
				if( Theta > Q15T_32767 )
				{
					Theta = Q15T_32767;
					Phase = 1;
				}
				break;
			}
			case 1:
			{
				Theta -= Step;
				if( Theta < Q15T_32768M )
				{
					Theta = Q15T_32768M;
					Phase = 0;
				}
				break;
			}
		}

		v.Value = v.to_int();
		return v;
	}

	void Reset() { Theta = Q15T( 0 ); }
};

#endif
