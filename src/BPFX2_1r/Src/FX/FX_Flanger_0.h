#ifndef FX_Flanger_0_h_
#define FX_Flanger_0_h_

#include "FX_Config.h"
#include <Myutil.h>
#include <Volume.h>
#include "FX_Interface.h"
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>
#include "Chorus_Buffer.h"

struct FX_Flanger_0 : public FX_Interface
{
	static const int FS_RATIO = 1;

	static constexpr Q15T_BQF_Params LPF_Params = BQF_LPF( 15000.f, 0.75f );

	static const int		WIDTH = MS_2_LENGTH( 10 );
	static const int		DELAY = MS_2_LENGTH( 20 );

	Chorus_Buffer				Buffer;
	Q15T_LFO<_FS_>			LFO;
	Volume<Curve_D>			Rate;
	Volume<Curve_D>			Depth;
	Volume<Curve_H>			Delay_Time;
	Volume<Curve_B>			Feedback;
	Volume<Curve_B>			Mix_Level;

	Q15T_BQF						LPF;

	FX_Flanger_0() :
		Buffer( WIDTH*2 + DELAY ),
		LFO( Sin_Table )
	{
  	Mix_Level.Set_Value( UINT12_MAX );
		LPF = LPF_Params;
	}

	int Process( int input )
	{
		LFO.Set_Rate( Fraction( Map( Rate.Get_Value(), 0, UINT12_MAX, 41 ,UINT12_MAX ), 409 ) );

		int output = Buffer.Get_Value( LFO.Get_Value() * ( Depth * WIDTH ) + WIDTH + ( Delay_Time * DELAY ) );

		Buffer.Set_Value( LPF( input + Feedback * output ) );

		return Mix_Level * output;
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

	virtual FX_ID Get_FX_ID() const	{ return FX_ID_Flanger_0; }

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
