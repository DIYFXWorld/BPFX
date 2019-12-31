#ifndef FX_TREMOLO_H_
#define FX_TREMOLO_H_

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>
#include <Q15T_LFO_Tri.h>

struct FX_Tremolo : public FX_Interface
{
	Volume<Curve_B>		Depth;
	Volume<Curve_B>		Rate;
	Q15T_LFO					LFO;
	Q15T_LFO_Tri			LFO_Tri;

	enum Type_Mode { triangle = 1, square = 2 };
	Type_Mode					Mode;

	FX_Tremolo() :
		LFO( _FS_, int16_t_Sin_Table ),
		LFO_Tri( _FS_ ),
		Mode( triangle )
	{
		Depth.Set_Value( 50 );
		Rate.Set_Value( 50 );
		Reset();
	}

	int Process( int v )
	{
		// Set Rate
		{
			int v = Map( Rate.Get_Value(), 0, UINT12_MAX, 27, UINT12_MAX );

			if( Mode == triangle )	LFO_Tri.Set_Rate( Fraction( v, 273 ) );
			if( Mode == square   )	LFO.Set_Rate( Fraction( v, 273 ) );
		}

		if( Mode == triangle )
		{
			v = ( LFO_Tri.Get_Value() * v ).to_int();
			v = v * -1;
		}
		else if( Mode == square )
		{
			Q15T	w = LFO.Get_Value() * 5;

			if( w >   Q15T_1 )	w = Q15T_1;
			if( w < Q15T_M_1 )	w = Q15T_M_1;

			v = ( w * v ).to_int();
		}

		v  = LIMIT_INT16( v );

		return Depth.Per( v );
	}

	virtual void Set_Param_0( int v )	{ Rate.Set_Value( Map( v, 0, UINT12_MAX, 1, UINT12_MAX ) ); }
	virtual int  Get_Param_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Param_2( int v )	{ Depth.Set_Value( Map( v, 0, UINT12_MAX, 10, UINT12_MAX ) ); }
	virtual int  Get_Param_2() const	{	return Depth.Initial_Value; }

	virtual void Set_Param_4( int v )	{ Mode = ( Type_Mode )v; }
	virtual int  Get_Param_4() const	{ return ( int )Mode; }

	virtual FX_ID Get_FX_ID() const	{ return FX_ID_Tremolo; }

	void Reset()
	{
		Depth.Fast_Forward();
		Rate.Fast_Forward();
		LFO.Reset();
		LFO_Tri.Reset();
	}
};

#endif
