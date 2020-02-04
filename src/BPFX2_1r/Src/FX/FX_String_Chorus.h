#ifndef FX_String_Chorus_h_
#define FX_String_Chorus_h_

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>
#include <Chorus_Buffer.h>

struct FX_String_Chorus : public FX_Interface
{
	Sub_Process_2<FX_String_Chorus>		Sub_Process;
	int																sp_input, sp_output, sp_a, sp_b;	// for sub process

	static constexpr Q15T_BQF_Params HPF_Params = BQF_HPF(   200.f, 0.75f );
	static constexpr Q15T_BQF_Params LPF_Params = BQF_LPF( 10000.f, 0.75f );

	static const int	WIDTH = _MS_2_LENGTH( 10, FS_DIV_2 );
	static const int	DELAY = _MS_2_LENGTH( 7, FS_DIV_2 );

	Chorus_Buffer			Buffer;

	Q15T_LFO<FS_DIV_2>		LFO;

	Volume<Curve_D>		Rate;
	Volume<Curve_D>		Depth;
	Volume<Curve_B>		Mix_Level;
	Volume<Curve_B>		Delay_Time;

	Q15T_BQF					HPF, LPF_Pre, LPF_Post;

	FX_String_Chorus():
		Sub_Process( this ),
		sp_input( 0 ), sp_output( 0 ), sp_a( 0 ), sp_b( 0 ),
		Buffer( WIDTH*2 + DELAY ),
		LFO( Modulated_Sin_Table )
	{
		Mix_Level.Set_Value( UINT12_MAX*6/10 );
		HPF 			= HPF_Params;
		LPF_Pre 	= LPF_Params;
		LPF_Post	= LPF_Params;
	}

	void SUB_PROCESS_0( int input )
	{
		sp_input = input;

		LFO.Set_Rate( Fraction( Map( Rate.Get_Value(), 0, UINT12_MAX, 41, UINT12_MAX ), 2047 ) );

		Buffer.Set_Value( sp_input );

		sp_a = Depth * WIDTH;
		sp_b = WIDTH + DELAY;

		sp_output  = Buffer.Get_Value( LFO.Get_Value()           * sp_a + sp_b );	// voice 1
		sp_output += Buffer.Get_Value( LFO.Get_Value( Q15T_R90 ) * sp_a + sp_b );	// voice 2
	}

	int SUB_PROCESS_1()
	{
		sp_output += Buffer.Get_Value( LFO.Get_Value( Q15T_R120 ) * sp_a + sp_b );	// voice 3
		sp_output += Buffer.Get_Value( LFO.Get_Value( Q15T_R240 ) * sp_a + sp_b );	// voice 4

		return Mix_Level * sp_output;
	}

	int Process( int input )
	{
		return LPF_Post( Sub_Process( LPF_Pre( HPF( input ) ) ) );
	}

	virtual void Set_Param_0( int v )	{ Rate.Set_Value( Map( v, 0, UINT12_MAX, 5, UINT12_MAX ) ); }
	virtual int  Get_Param_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Param_1( int v )	{ Mix_Level.Set_Value( v ); }
	virtual int  Get_Param_1() const	{ return Mix_Level.Initial_Value; }

	virtual void Set_Param_2( int v )	{ Depth.Set_Value( Map( v, 0, UINT12_MAX, 10, UINT12_MAX ) ); }
	virtual int  Get_Param_2() const	{	return Depth.Initial_Value; }

	virtual FX_ID Get_FX_ID() const	{ return FX_ID_String_Chorus; }

	void Reset()
	{
		LFO.Reset();
		HPF.Reset();
		LPF_Pre.Reset();
		LPF_Post.Reset();

		Rate.Fast_Forward();
		Depth.Fast_Forward();
		Mix_Level.Fast_Forward();

		Buffer.Memory.Reset();
		Sub_Process.Reset();
		sp_input = sp_output = sp_a = sp_b =0;
	}
};

#endif
