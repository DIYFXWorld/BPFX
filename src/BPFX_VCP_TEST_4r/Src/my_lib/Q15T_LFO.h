#ifndef	Q15T_LFO_h
#define	Q15T_LFO_h

#include	"Q15T.h"
#include	"Q15T_Cos_Sin.h"
#include	"Fraction.h"

struct Q15T_LFO_Cos
{
	const Q15T			Wave_Table_Size_Per_Sampling_Rate;
				Q15T			Theta;
				Q15T			Theta_Step;

	Q15T_LFO_Cos( int sampling_rate ) :
		Wave_Table_Size_Per_Sampling_Rate( 65535.f / sampling_rate ),
		Theta( 0 ),
		Theta_Step( 0 ){}

	void Rate( const Fraction& v )
	{
		Theta_Step = Wave_Table_Size_Per_Sampling_Rate * v;
	}

	Q15T Get()
	{
		Q15T	Result = Q15T_Cos( ( uint16_t )Theta.to_int() );
 		Theta = Theta + Theta_Step;
 		if( Theta.Value < 0 )	Theta = Theta_Step;			// Q15T��65535�𒴂���ƃ}�C�i�X�ɂȂ�
		return Result;
	}

	void Reset() { Theta = Q15T( 0 ); }
};

struct Q15T_LFO_Sin
{
	const Q15T	Wave_Table_Size_Per_Sampling_Rate;
				Q15T	Theta;
				Q15T	Theta_Step;

	Q15T_LFO_Sin( int sampling_rate ) :
		Wave_Table_Size_Per_Sampling_Rate( 65535.f / sampling_rate ),
		Theta( 0 ),
		Theta_Step( 0 ){}

	void Rate( const Fraction& v )
	{
		Theta_Step = Wave_Table_Size_Per_Sampling_Rate * v;
	}

	Q15T Get()
	{
		Q15T	Result = Q15T_Sin( ( uint16_t )Theta.to_int() );
 		Theta = Theta + Theta_Step;
 		if( Theta.Value < 0 )	Theta = Theta_Step;			// Q15T��65535�𒴂���ƃ}�C�i�X�ɂȂ�
		return Result;
	}

	void Reset() { Theta = Q15T( 0 ); }
};

#endif
