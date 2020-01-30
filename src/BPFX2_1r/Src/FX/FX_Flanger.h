#ifndef FX_FLANGER_H_
#define FX_FLANGER_H_

#include "FX_Config.h"
#include <Myutil.h>
#include <Volume.h>
#include "FX_Interface.h"
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>
#include "Chorus_Buffer.h"

struct FX_Flanger : public FX_Interface
{
	static constexpr Q15T_BQF_Params LPF_Params = BQF_LPF( 10000.f, 1.0f );

	static const int		DEPTH_BUFFER_LENGTH = _MS_2_LENGTH( 10, _FS_ );
	static const int		DELAY_BUFFER_LENGTH = _MS_2_LENGTH( 20, _FS_ );

	Chorus_Buffer				Buffer;
	Q15T_LFO						LFO;
	Volume<Curve_D>			Rate;
	Volume<Curve_D>			Depth;
	Volume<Curve_H>			Delay_Time;
	Volume<Curve_B>			Feedback;
	Volume<Curve_B>			Mix_Level;

	Q15T_BQF						LPF;

	FX_Flanger() :
		Buffer( DEPTH_BUFFER_LENGTH*2 + DELAY_BUFFER_LENGTH ),
		LFO( _FS_, int16_t_Sin_Table )
	{
		LPF = LPF_Params;
  	Mix_Level.Set_Value( UINT12_MAX );
	}

	int Process( int input )
	{
		int	output;

		// Set Rate
		{
			int v = Map( Rate.Get_Value(), 0, UINT12_MAX, 41 ,UINT12_MAX );
			LFO.Set_Rate( Fraction( v, 409 ) );
		}

		int	DEPTH				= Depth.Per( DEPTH_BUFFER_LENGTH );
		int	DELAY_TIME	= Delay_Time.Per( DELAY_BUFFER_LENGTH );

		{
			Q15T	t			= LFO.Get_Value() * DEPTH + (DEPTH+1) + DELAY_TIME;
			int		m			= t.to_int();
			Q15T	delta	= t - Q15T( m );
			output = ( delta * Buffer.Get_Value( m + 1 ) +	( Q15T_1 - delta ) * Buffer.Get_Value( m ) ).to_int();
		}

		output = LIMIT_INT16( output );

		int	fb = input + Feedback.Per( output );

		fb = Limit( -10000, fb, 10000 );

		fb = LPF( fb );

		Buffer.Set_Value( fb );

		output = Mix_Level.Per( output );

		return output;
	}

	virtual void Set_Param_0( int v )	{ Rate.Set_Value( v ); }
	virtual int  Get_Param_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Param_1( int v )	{ Delay_Time.Set_Value( v ); }
	virtual int  Get_Param_1() const	{ return Delay_Time.Initial_Value; }

	virtual void Set_Param_2( int v )	{ Depth.Set_Value( v ); }
	virtual int  Get_Param_2() const	{	return Depth.Initial_Value; }

	virtual void Set_Param_3( int v )	{ Feedback.Set_Value( v ); }
	virtual int  Get_Param_3() const	{ return Feedback.Initial_Value; }

	virtual void Set_Param_4( int v )	{ Mix_Level.Set_Value( v ); }
	virtual int  Get_Param_4() const	{ return Mix_Level.Initial_Value; }

	virtual FX_ID Get_FX_ID() const	{ return FX_ID_Flanger; }

	void Reset()
	{
		Rate.Fast_Forward();
		Delay_Time.Fast_Forward();
		Depth.Fast_Forward();
		Feedback.Fast_Forward();
		Mix_Level.Fast_Forward();
		Buffer.Memory.Reset();
		LFO.Reset();
		LPF.Reset();
	}
};

#endif /* FX_CHORUS_FLANGER_H_ */
