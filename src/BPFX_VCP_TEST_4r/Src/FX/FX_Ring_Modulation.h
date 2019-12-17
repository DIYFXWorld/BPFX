#ifndef FX_Ring_Modulation_H_
#define FX_Ring_Modulation_H_

#include	"hal.h"
#include	"Q15T_LFO.h"
#include	"Volume.h"
#include	"Potentiometer_Curve.h"
#include	"Audio.h"
#include	"FX_Interface.h"
#include	"Common.h"

//const Q15T			CONST_1( 1 );
//const Q15T			CONST_M1( -1 );

struct FX_Ring_Modulation : public FX_Interface
{
	Type_Volume<Curve_B>	Rate;
	Q15T_LFO_Sin					LFO;

	FX_Ring_Modulation() :
		LFO( Audio::Sampling_Rate )
	{
		Rate.Set( 50 );
	}

	void Destroy()	{ delete this; }

	int Process( int input )
	{
		// Set Rate
		{
			int v = Map( Rate.Get(), 0, UINT12_MAX, 30, UINT12_MAX );
			LFO.Rate( Fraction( v, 2 ) );
		}

		int	wet = ( LFO.Get() * input ).to_int();

		wet  = LIMIT_INT16( wet ) ;

		return wet - input;
	}

	virtual void Set_Control_0( int v )	{ Rate.Set( Map( v, 0, UINT12_MAX, 1, UINT12_MAX ) ); }
	virtual int  Get_Control_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Control_1( int )		{}
	virtual int  Get_Control_1() const	{ return 0; }

	virtual void Set_Control_2( int v )	{}
	virtual int  Get_Control_2() const	{	return 0; }

	virtual void Set_Control_3( int )		{}
	virtual int  Get_Control_3() const	{ return 0; }

	virtual void Set_Control_4( int v )	{}
	virtual int  Get_Control_4() const	{ return 0; }

	virtual int Get_Memory_Length() const	{ return 0; }

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Ring_Modulation; }

};

#endif
