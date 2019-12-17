/*
 * FX_Chorus.h
 *
 *  Created on: 2019/02/22
 *      Author: ganymede
 */

#ifndef FX_CHORUS_H_
#define FX_CHORUS_H_

#include	"FX_Interface.h"
#include	"Chorus_Buffer.h"
#include	"Q15T_LFO.h"
#include	"Q15T_BQF.h"
#include	"Volume.h"
#include	"Audio.h"
#include	"Common.h"
#include	"Potentiometer_Curve.h"

//const Q15T	CONST_1( 1 );

const	int	CHORUS_DEPTH_LENGTH = MS_2_LENGTH( 10 );
const	int	CHORUS_DELAY_LENGTH = MS_2_LENGTH(  5 );

struct FX_Chorus : public FX_Interface
{
	Chorus_Buffer						Buffer;
	Q15T_LFO_Sin						LFO;

	Type_Volume<Curve_D>		Rate;
	Type_Volume<Curve_D>		Depth;
	Type_Volume<Curve_B>		Mix_Level;

	Q15T_BQF								HPF, LPF;


	FX_Chorus() :
		Buffer( CHORUS_DEPTH_LENGTH*2+CHORUS_DELAY_LENGTH ),
		LFO( Audio::Sampling_Rate )
	{
		Mix_Level.Set( UINT12_MAX*6/10 );
		HPF = BQF_Builder( Audio::Sampling_Rate ).HPF(   200.f, 0.7 );
		LPF = BQF_Builder( Audio::Sampling_Rate ).LPF( 10000.f, 0.7 );
	}

	void Destroy() { delete this; }

	int Process( int input )
	{
		if( Reset )
		{
			LFO.Reset();
			HPF.Reset();
			LPF.Reset();
			Buffer.Memory.Clear();
			Reset = false;
		}

		input = HPF.Process( input );

		// Set Rate
		{
			int v = Map( Rate.Get(), 0, UINT12_MAX, 27, UINT12_MAX );
			LFO.Rate( Fraction( v, 273 ) );
		}

		int	DEPTH = Depth.Per( CHORUS_DEPTH_LENGTH );

		Buffer.Set( input );

		Q15T	t			= LFO.Get() * DEPTH + (DEPTH+1) + CHORUS_DELAY_LENGTH;
		int		m			= t.to_int();
		Q15T	delta	= t - Q15T( m );

		int wet = ( delta * Buffer.Get( m + 1 ) +	( CONST_1 - delta ) * Buffer.Get( m ) ).to_int();

		wet  = LIMIT_INT16( wet );

		wet = LPF.Process( wet );

		return Mix_Level.Per( wet );
	}

	virtual void Set_Control_0( int v )	{ Rate.Set( Map( v, 0, UINT12_MAX, 1, UINT12_MAX ) ); }
	virtual int  Get_Control_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Control_1( int v )	{ Mix_Level.Set( v ); }
	virtual int  Get_Control_1() const	{ return Mix_Level.Initial_Value; }

	virtual void Set_Control_2( int v )	{ Depth.Set( Map( v, 0, UINT12_MAX, 10, UINT12_MAX ) ); }
	virtual int  Get_Control_2() const	{	return Depth.Initial_Value; }

	virtual void Set_Control_3( int )		{}
	virtual int  Get_Control_3() const	{ return 0; }

	virtual void Set_Control_4( int )		{}
	virtual int  Get_Control_4() const	{ return 0; }

	virtual int Get_Memory_Length() const	{ return Buffer.Memory.Length; }

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Chorus; }
};

#endif /* FX_CHORUS_H_ */
