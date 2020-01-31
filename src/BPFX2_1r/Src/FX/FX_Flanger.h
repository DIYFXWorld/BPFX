#ifndef FX_Flanger_h_
#define FX_Flanger_h_

#include "FX_Config.h"
#include <Myutil.h>
#include <Volume.h>
#include "FX_Interface.h"
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>
#include "Chorus_Buffer.h"
#include "Sub_Process.h"

struct FX_Flanger : public FX_Interface
{
	static constexpr Q15T_BQF_Params LPF_Params		= BQF_LPF( 10000.f, 0.75f );
	static constexpr Q15T_BQF_Params LPF_Params_2 = BQF_LPF(  8000.f, 0.75f, _FS_/2 );

	static const int		DEPTH_BUFFER_LENGTH = _MS_2_LENGTH( 10, _FS_/2 );
	static const int		DELAY_BUFFER_LENGTH = _MS_2_LENGTH( 20, _FS_/2 );

	Chorus_Buffer				Buffer;
	Q15T_LFO						LFO;
	Volume<Curve_H>			Rate;
	Volume<Curve_F>			Depth;
	Volume<Curve_H>			Delay_Time;
	Volume<Curve_E>			Feedback;
	Volume<Curve_B>			Mix_Level;

	Q15T_BQF						LPF, LPF_Pre, LPF_Post;

	Sub_Process_2<FX_Flanger>		Sub_Process;
	int													_input_, DEPTH, DELAY_TIME;	// for sub process

	FX_Flanger() :
		Buffer( DEPTH_BUFFER_LENGTH*2 + DELAY_BUFFER_LENGTH ),
		LFO( _FS_/2, int16_t_Sin_Table ),
	Sub_Process( this )
	
	{
		LPF_Pre		= LPF_Params;
		LPF_Post	= LPF_Params;
		LPF 			= LPF_Params_2;
  	Mix_Level.Set_Value( UINT12_MAX );
	}

	void SUB_PROCESS_0( int v )
	{
		_input_ = v;

		// Set Rate
		{
			int v = Map( Rate.Get_Value(), 0, UINT12_MAX, 41 ,UINT12_MAX );
			LFO.Set_Rate( Fraction( v, 409 ) );
		}

		DEPTH				= Depth.Per( DEPTH_BUFFER_LENGTH );
		DELAY_TIME	= Delay_Time.Per( DELAY_BUFFER_LENGTH );
	}

	int SUB_PROCESS_1()
	{
		int	output;

		{
			Q15T	t			= LFO.Get_Value() * DEPTH + (DEPTH+1) + DELAY_TIME;
			int		m			= t.to_int();
			Q15T	delta	= t - Q15T( m );
			output = ( delta * Buffer.Get_Value( m + 1 ) +	( Q15T_1 - delta ) * Buffer.Get_Value( m ) ).to_int();
		}

		output = LIMIT_INT16( output );

		int	fb = _input_ - Feedback.Per( output );

		fb = LIMIT_INT16( fb );

		fb = LPF( fb );

		Buffer.Set_Value( fb );

		output = Mix_Level.Per( output );

		return output;
	}

	int Process( int input )
	{
		return LPF_Post( Sub_Process( LPF_Pre( input ) ) );
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
		LPF_Pre.Reset();
		LPF_Post.Reset();
		Sub_Process.Reset();
	}
};

#endif /* FX_CHORUS_FLANGER_H_ */
