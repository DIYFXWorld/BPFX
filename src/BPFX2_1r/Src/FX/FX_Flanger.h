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
	Sub_Process_2<FX_Flanger>		Sub_Process;
	int													sp_input;	// for sub process
	Q15T												sp_t;

	static constexpr Q15T_BQF_Params LPF_Params		= BQF_LPF( 10000.f, 0.75f );
	static constexpr Q15T_BQF_Params LPF_Params_2 = BQF_LPF(  8000.f, 0.90f, FS_DIV_2 );

	static const int			WIDTH = _MS_2_LENGTH( 10, FS_DIV_2 );
	static const int			DELAY = _MS_2_LENGTH( 20, FS_DIV_2 );

	Chorus_Buffer					Buffer;
	Q15T_LFO<FS_DIV_2>		LFO;
	Volume<Curve_H>				Rate;
	Volume<Curve_F>				Depth;
	Volume<Curve_H>				Delay_Time;
	Volume<Curve_E>				Feedback;
	Volume<Curve_B>				Mix_Level;

	Q15T_BQF							LPF, LPF_Pre, LPF_Post;

	FX_Flanger() :
		Sub_Process( this ),
		sp_input( 0 ), sp_t( 0 ),
		Buffer( WIDTH*2 + DELAY ),
		LFO( Sin_Table )
	{
		LPF_Pre		= LPF_Params;
		LPF_Post	= LPF_Params;
		LPF 			= LPF_Params_2;
  	Mix_Level.Set_Value( UINT12_MAX );
	}

	void SUB_PROCESS_0( int v )
	{
		sp_input = v;

		LFO.Set_Rate( Fraction( Map( Rate.Get_Value(), 0, UINT12_MAX, 41 ,UINT12_MAX ), 409 ) );

		sp_t = LFO.Get_Value() * ( Depth * WIDTH ) + WIDTH + ( Delay_Time * DELAY );
	}

	int SUB_PROCESS_1()
	{
		int output = Buffer.Get_Value( sp_t );

		int fb = Limit<-10000,10000>( sp_input - Feedback * output );

		Buffer.Set_Value( LPF( fb ) );

		return Mix_Level * output;
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
		sp_input = 0;
		sp_t.Value = 0;
	}
};

#endif /* FX_CHORUS_FLANGER_H_ */
