#ifndef FX_CHORUS_0_H_
#define FX_CHORUS_0_H_

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>
#include <Chorus_Buffer.h>

struct FX_Chorus_0 : public FX_Interface
{
	static constexpr Q15T_BQF_Params HPF_Params = BQF_HPF(   200.f, 0.75f );
	static constexpr Q15T_BQF_Params LPF_Params = BQF_LPF( 10000.f, 0.75f );

	static const int	WIDTH = MS_2_LENGTH( 10 );
	static const int	DELAY = MS_2_LENGTH(  5 );

	Chorus_Buffer			Buffer;
	Q15T_LFO<_FS_>			LFO;

	Volume<Curve_D>		Rate;
	Volume<Curve_D>		Depth;
	Volume<Curve_B>		Mix_Level;

	Q15T_BQF					HPF, LPF;

	FX_Chorus_0():
		Buffer( WIDTH*2 + DELAY ),
		LFO( Sin_Table )
	{
		Mix_Level.Set_Value( UINT12_MAX*6/10 );
		HPF = HPF_Params;
		LPF = LPF_Params;
		Reset();
	}

	int Process( int input )
	{
		input = HPF( input );

		// Set Rate
		{
			int v = Map( Rate.Get_Value(), 0, UINT12_MAX, 41, UINT12_MAX );
			LFO.Set_Rate( Fraction( v, 409 ) );
		}

		Buffer.Set_Value( input );

		int	output = Buffer.Get_Value( LFO.Get_Value() * ( Depth * WIDTH ) + WIDTH + DELAY );

		output = LPF( output );

		output = Mix_Level * output;

		return output;
	}

	virtual void Set_Param_0( int v )	{ Rate.Set_Value( Map( v, 0, UINT12_MAX, 1, UINT12_MAX ) ); }
	virtual int  Get_Param_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Param_1( int v )	{ Mix_Level.Set_Value( v ); }
	virtual int  Get_Param_1() const	{ return Mix_Level.Initial_Value; }

	virtual void Set_Param_2( int v )	{ Depth.Set_Value( Map( v, 0, UINT12_MAX, 10, UINT12_MAX ) ); }
	virtual int  Get_Param_2() const	{	return Depth.Initial_Value; }

	virtual FX_ID Get_FX_ID() const	{ return FX_ID_Chorus_0; }

	void Reset()
	{
		LFO.Reset();
		HPF.Reset();
		LPF.Reset();
		Rate.Fast_Forward();
		Depth.Fast_Forward();
		Mix_Level.Fast_Forward();
		Buffer.Memory.Reset();
		Buffer.Fast_Forward();
	}
};

#endif
