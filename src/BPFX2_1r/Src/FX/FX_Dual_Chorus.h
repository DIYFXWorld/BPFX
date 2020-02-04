#ifndef FX_Dual_Chorus_h_
#define FX_Dual_Chorus_h_

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>
#include <Chorus_Buffer.h>

struct FX_Dual_Chorus : public FX_Interface
{
	Sub_Process_2<FX_Dual_Chorus>		Sub_Process;
	int															sp_input, sp_output;	// for sub process

	static constexpr Q15T_BQF_Params HPF_Params = BQF_HPF(   200.f, 0.75f );
	static constexpr Q15T_BQF_Params LPF_Params = BQF_LPF( 10000.f, 0.75f );

	static const int	WIDTH = _MS_2_LENGTH( 10, FS_DIV_2 );
	static const int	DELAY = _MS_2_LENGTH(  5, FS_DIV_2 );

	Chorus_Buffer			Buffer_0, Buffer_1;

	Q15T_LFO<FS_DIV_2>	LFO_0, LFO_1;

	Volume<Curve_D>		Rate_0, Rate_1;
	Volume<Curve_D>		Depth_0, Depth_1;
	Volume<Curve_B>		Mix_Level;

	Q15T_BQF					HPF, LPF_Pre, LPF_Post;

	FX_Dual_Chorus():
		Sub_Process( this ),
		sp_input( 0 ), sp_output( 0 ),
		Buffer_0( WIDTH*2 + DELAY ),
		Buffer_1( WIDTH*2 + DELAY ),
		LFO_0( Sin_Table ),
		LFO_1( Sin_Table )
	{
		Mix_Level.Set_Value( UINT12_MAX*6/10 );
		HPF = HPF_Params;
		LPF_Pre = LPF_Params;
		LPF_Post = LPF_Params;
	}

	void SUB_PROCESS_0( int input )
	{
		sp_input = input;

		LFO_0.Set_Rate( Fraction( Map( Rate_0.Get_Value(), 0, UINT12_MAX, 41, UINT12_MAX ), 409 ) );

		Buffer_0.Set_Value( sp_input );

		sp_output = Buffer_0.Get_Value( LFO_0.Get_Value() * ( Depth_0 * WIDTH ) + WIDTH + DELAY );
	}

	int SUB_PROCESS_1()
	{
		LFO_1.Set_Rate( Fraction( Map( Rate_1.Get_Value(), 0, UINT12_MAX, 41, UINT12_MAX ), 409 ) );

		Buffer_1.Set_Value( sp_output );

		return Mix_Level * Buffer_1.Get_Value( LFO_1.Get_Value() * ( Depth_1 * WIDTH ) + WIDTH + DELAY );
	}

	int Process( int input )
	{
		return LPF_Post( Sub_Process( HPF( LPF_Pre( input ) ) ) );
	}

	virtual void Set_Param_0( int v )	{ Rate_0.Set_Value( Map( v, 0, UINT12_MAX, 1, UINT12_MAX ) ); }
	virtual int  Get_Param_0() const	{ return Rate_0.Initial_Value; }

	virtual void Set_Param_1( int v )	{ Depth_0.Set_Value( Map( v, 0, UINT12_MAX, 10, UINT12_MAX ) ); }
	virtual int  Get_Param_1() const	{	return Depth_0.Initial_Value; }

	virtual void Set_Param_2( int v )	{ Rate_1.Set_Value( Map( v, 0, UINT12_MAX, 1, UINT12_MAX ) ); }
	virtual int  Get_Param_2() const	{ return Rate_1.Initial_Value; }

	virtual void Set_Param_3( int v )	{ Depth_1.Set_Value( Map( v, 0, UINT12_MAX, 10, UINT12_MAX ) ); }
	virtual int  Get_Param_3() const	{	return Depth_1.Initial_Value; }

	virtual void Set_Param_4( int v )	{ Mix_Level.Set_Value( v ); }
	virtual int  Get_Param_4() const	{ return Mix_Level.Initial_Value; }

	virtual FX_ID Get_FX_ID() const	{ return FX_ID_Dual_Chorus; }

	void Reset()
	{
		LFO_0.Reset();
		LFO_1.Reset();
		HPF.Reset();
		LPF_Pre.Reset();
		LPF_Post.Reset();
		Rate_0.Fast_Forward();
		Rate_1.Fast_Forward();
		Depth_0.Fast_Forward();
		Depth_1.Fast_Forward();
		Mix_Level.Fast_Forward();
		Buffer_0.Memory.Reset();
		Buffer_1.Memory.Reset();
		Sub_Process.Reset();
		sp_input = sp_output = 0;
	}
};

#endif
