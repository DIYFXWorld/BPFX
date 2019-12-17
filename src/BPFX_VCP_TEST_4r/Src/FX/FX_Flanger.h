#ifndef FX_FLANGER_H_
#define FX_FLANGER_H_

#include	"Chorus_Buffer.h"
#include	"Q15T_LFO.h"
#include	"Q15T_BQF.h"
#include	"Volume.h"
#include	"Audio.h"
#include	"Common.h"
#include	"Potentiometer_Curve.h"
#include	"FX_ID.h"
#include	"FX_Interface.h"

//const Q15T	CONST_1( 1 );

const	int	FX_FLANGER_DEPTH_LENGTH = MS_2_LENGTH( 10 );
const	int	FX_FLANGER_DELAY_LENGTH = MS_2_LENGTH( 20 );

struct FX_Flanger : public FX_Interface
{
	Chorus_Buffer							Buffer;
	Q15T_LFO_Sin							LFO;
	Type_Volume<Curve_H>			Rate;
	Type_Volume<Curve_H>			Delay_Time;
	Type_Volume<Curve_F>			Depth;
	Type_Volume<Curve_E>			Feedback;
	Type_Volume<Curve_B>			Mix_Level;

	Q15T_BQF									LPF_0, LPF_1;

	FX_Flanger() :
		Buffer( FX_FLANGER_DEPTH_LENGTH*2+FX_FLANGER_DELAY_LENGTH ),
		LFO( Audio::Sampling_Rate )
	{
		Rate.Set( 0 );
		Depth.Set( 0 );
		Delay_Time.Set( 0 );
		Mix_Level.Set( UINT12_MAX*7/10 );

		LPF_0 = BQF_Builder( Audio::Sampling_Rate ).LPF( 10000.f, 0.7f );
		LPF_1 = BQF_Builder( Audio::Sampling_Rate ).LPF( 10000.f, 0.7f );
	}

	void Destroy() { delete this; }

	int Process( int input )
	{
		if( Reset )
		{
			Buffer.Memory.Clear();
			Reset = false;
		}

		// Set Rate
		{
			int v = Map( Rate.Get(), 0, UINT12_MAX, 2, UINT12_MAX );
			LFO.Rate( Fraction( v, 300 ) );
		}

		int	DEPTH				= FX_FLANGER_DEPTH_LENGTH * Depth.Get() / UINT12_MAX;
		int	DELAY_TIME	= FX_FLANGER_DELAY_LENGTH * Delay_Time.Get() / UINT12_MAX;

		int	wet;
		{
			Q15T	t			= LFO.Get() * DEPTH + (DEPTH+1) + DELAY_TIME;
			int		m			= t.to_int();
			Q15T	delta	= t - Q15T( m );
			wet = ( delta * Buffer.Get( m + 1 ) +	( CONST_1 - delta ) * Buffer.Get( m ) ).to_int();
		}

		wet = Limit( INT16_MIN, wet, INT16_MAX );

		wet = LPF_1.Process( wet );

		int	fb = input - wet * Feedback.Get() / UINT12_MAX;

		fb = Limit( -10000, fb, 10000 );

		fb = LPF_0.Process( fb );

		Buffer.Set( fb );

		wet = wet * Mix_Level.Get() / UINT12_MAX;

		return wet;
	}

	virtual void Set_Control_0( int v )	{ Rate.Set( v ); }
	virtual int  Get_Control_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Control_1( int v )	{ Delay_Time.Set( v ); }
	virtual int  Get_Control_1() const	{ return Delay_Time.Initial_Value; }

	virtual void Set_Control_2( int v )	{ Depth.Set( v ); }
	virtual int  Get_Control_2() const	{	return Depth.Initial_Value; }

	virtual void Set_Control_3( int v )	{ Feedback.Set( v ); }
	virtual int  Get_Control_3() const	{ return Feedback.Initial_Value; }

	virtual void Set_Control_4( int v )	{ Mix_Level.Set( v ); }
	virtual int  Get_Control_4() const	{ return Mix_Level.Initial_Value; }

	virtual int Get_Memory_Length() const	{ return Buffer.Memory.Length; }

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Flanger; }
};

#endif /* FX_CHORUS_FLANGER_H_ */
