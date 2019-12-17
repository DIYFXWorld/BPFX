/*
 * FX_Madulation_Delay.h
 *
 *  Created on: 2019/02/22
 *      Author: ganymede
 */

#ifndef FX_MODULATION_DELAY_H_
#define FX_MODULATION_DELAY_H_

#include	"Chorus_Buffer.h"
#include	"Q15T_LFO.h"
#include	"Q15T_BQF.h"
#include	"Volume.h"
#include	"Audio.h"
#include	"Common.h"
#include	"Potentiometer_Curve.h"
#include	"FX_ID.h"
#include	"FX_Interface.h"

#define		FX_MODULATION_DELAY_FS_RATIO	4

const Q15T	CONST_1( 1 );

const	int	FX_MODULATION_DELAY_DEPTH_LENGTH = MS_2_LENGTH( 5 );
const	int	FX_MODULATION_DELAY_DELAY_LENGTH = MS_2_LENGTH( 180 );

struct FX_Modulation_Delay : public FX_Interface
{
	bool	Reset;

	Chorus_Buffer						Buffer;
	Q15T_LFO_Sin						LFO;
	Type_Volume<Curve_F>		Rate;				// Control_0
	Type_Volume<Curve_B>		Delay_Time;	// Control_1
	Type_Volume<Curve_F>		Depth;			// Control_2
	Type_Volume<Curve_B>		Feedback;		// Control_3
	Type_Volume<Curve_B>		Mix_Level;	// Control_4

	Q15T_BQF								LPF_0, LPF_1;

	FX_Modulation_Delay() : Reset( true ),
		Buffer( FX_MODULATION_DELAY_DEPTH_LENGTH*2+FX_MODULATION_DELAY_DELAY_LENGTH ),
		LFO( Audio::Sampling_Rate/FX_MODULATION_DELAY_FS_RATIO )
	{
		LPF_0 = BQF_Builder( Audio::Sampling_Rate ).LPF( 5000.f, 0.75f );
		LPF_1 = BQF_Builder( Audio::Sampling_Rate ).LPF( 5000.f, 0.75f );
	}

	void Destroy() { delete this; }

	int _Process( int input )
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

		int	DEPTH				= Depth.Per( FX_MODULATION_DELAY_DEPTH_LENGTH-1 );
		int	DELAY_TIME	= Delay_Time.Per( FX_MODULATION_DELAY_DELAY_LENGTH-1 );

		int	wet;
		{
			Q15T	t			= LFO.Get() * DEPTH + (DEPTH+1) + DELAY_TIME;
			int		m			= t.to_int();
			Q15T	delta	= t - Q15T( m );
			wet = ( delta * Buffer.Get( m + 1 ) +	( CONST_1 - delta ) * Buffer.Get( m ) ).to_int();
		}

//		wet = LPF_1.Process( wet );

		int	fb = input - Feedback.Per( wet );

		fb = LIMIT_INT16( fb );

//		fb = LPF_0.Process( fb );

		Buffer.Set( fb );

		wet = Mix_Level.Per( wet );

//		wet = LIMIT_INT16( wet );

		return wet;
	}

	int Process( int input )
	{
		input = LPF_0.Process( input );

		int	output;
		static int	count, last, value;

		if( count < FX_MODULATION_DELAY_FS_RATIO-1 )
		{
			value += input;
			output = last;
			++count;
		}
		else
		{
			value += input;
			output = last = _Process( value / FX_MODULATION_DELAY_FS_RATIO );
			count = value = 0;
		}

		output = LPF_1.Process( output );

		return output;
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

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Modulation_Delay; }
};

#endif /* FX_CHORUS_FLANGER_H_ */
