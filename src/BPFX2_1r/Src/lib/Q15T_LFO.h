#ifndef	Q15T_LFO_h
#define	Q15T_LFO_h

#include	"Q15T.h"
#include	"Q15T_Wave_Table.h"
#include	"Fraction.h"

struct Q15T_LFO
{
	const Q15T			Wave_Table_Size_Per_Sampling_Rate;
				Q15T			Theta;
				Q15T			Theta_Step;
	const int16_t* const	Table;

	Q15T_LFO( int sampling_rate, const int16_t* const table ):
		Wave_Table_Size_Per_Sampling_Rate( 65535.f / sampling_rate ),
		Theta( 0 ),
		Theta_Step( 0 ),
		Table( table )
	{
	}

	void Set_Rate( const Fraction& v )
	{
		Theta_Step = Wave_Table_Size_Per_Sampling_Rate * v;
	}

	Q15T Get_Value()
	{
		Q15T	Result = Q15T_Get_Wave_Table_Data( ( uint16_t )Theta.to_int(), Table );
 		Theta = Theta + Theta_Step;
 		if( Theta.Value < 0 )	Theta = Q15T( Q15T_Raw_Value( ( Theta.Value & Q15T_MAX.Value ) + 1 ) );
		return Result;
	}

	Q15T Get_Value( const Q15T& v )
	{
		Q15T	t = Theta + v;
 		if( t.Value < 0 )	t = Q15T( Q15T_Raw_Value( ( t.Value & Q15T_MAX.Value ) + 1 ) );
		return Q15T_Get_Wave_Table_Data( ( uint16_t )t.to_int(), Table );
	}

	void Reset() { Theta = Q15T( 0 ); }
};

#endif
