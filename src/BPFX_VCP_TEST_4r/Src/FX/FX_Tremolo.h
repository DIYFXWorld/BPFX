#ifndef FX_TREMOLO_H_
#define FX_TREMOLO_H_

#include	"hal.h"
#include	"Q15T_LFO.h"
#include	"Volume.h"
#include	"Potentiometer_Curve.h"
#include	"Audio.h"
#include	"FX_Interface.h"
#include	"Common.h"

//const Q15T			CONST_1( 1 );
const Q15T			CONST_M1( -1 );

struct FX_Tremolo : public FX_Interface
{
	Type_Volume<Curve_B>	Depth;
	Type_Volume<Curve_B>	Rate;
	Q15T_LFO_Sin					LFO;

	enum Type_Mode { sine = 1, square = 2 };
	Type_Mode							Mode;

	FX_Tremolo() :
		LFO( Audio::Sampling_Rate ), Mode( sine )
	{
		Depth.Set( 50 );
		Rate.Set( 50 );
	}

	void Destroy()	{ delete this; }

	int Process( int v )
	{
		// Set Rate
		{
			int v = Map( Rate.Get(), 0, UINT12_MAX, 27, UINT12_MAX );
			LFO.Rate( Fraction( v, 273 ) );
		}

		if( Mode == sine )
		{
			v = ( LFO.Get() * v ).to_int();
			v = v * -1;
		}
		else if( Mode == square )
		{
			Q15T	w = LFO.Get() * 5;

			if( w > CONST_1 )		w = CONST_1;
			if( w < CONST_M1 )	w = CONST_M1;

			v = ( w * v ).to_int();
		}

		v  = LIMIT_INT16( v );

		return Depth.Per( v );
	}

	virtual void Set_Control_0( int v )	{ Rate.Set( Map( v, 0, UINT12_MAX, 1, UINT12_MAX ) ); }
	virtual int  Get_Control_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Control_1( int )		{}
	virtual int  Get_Control_1() const	{ return 0; }

	virtual void Set_Control_2( int v )	{ Depth.Set( Map( v, 0, UINT12_MAX, 10, UINT12_MAX ) ); }
	virtual int  Get_Control_2() const	{	return Depth.Initial_Value; }

	virtual void Set_Control_3( int )		{}
	virtual int  Get_Control_3() const	{ return 0; }

	virtual void Set_Control_4( int v )	{ Mode = ( Type_Mode )v; }
	virtual int  Get_Control_4() const	{ return ( int )Mode; }

	virtual int Get_Memory_Length() const	{ return 0; }

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Tremolo; }

};

#endif
