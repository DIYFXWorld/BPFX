#ifndef	FX_Delay_h
#define	FX_Delay_h

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Delay_Buffer.h>

struct FX_Delay : public FX_Interface
{
	static const int		FS_RATIO	= 2;

	static constexpr Q15T_BQF_Params LPF_Params = BQF_LPF( 10000.f, 0.75f );

//	static const int		BUFFER_LENGTH	= FX_DELAY_BUFFER_LENGTH;

	Volume_x<Curve_A>		Time_Length;
	Volume<Curve_B>			Feedback;
	Volume<Curve_B>			Mix_Level;

	Delay_Buffer				Buffer;

	Q15T_BQF						LPF_Pre, LPF_Post;

	Sub_Process_2<FX_Delay>	Sub_Process;
	int											sp_input, sp_output, sp_delay;	// for sub process

	FX_Delay():
		Time_Length( 0 ), Feedback( 0 ), Mix_Level( 0 ),
		Buffer( FX_DELAY_BUFFER_LENGTH ),
		Sub_Process( this ),
		sp_input( 0 ), sp_output( 0 ), sp_delay( 0 )
	{
		LPF_Pre 	= LPF_Params;
		LPF_Post 	= LPF_Params;
	}

	void SUB_PROCESS_0( int v )
	{
		sp_input = v;

		Buffer.Set_Length( Map( Time_Length.Get_Value(), 0, UINT12_MAX, 1, Buffer.Memory.Length-1 ) );

		sp_delay = Buffer.Get_Value();
	}

	int SUB_PROCESS_1()
	{
		sp_input	-= Feedback.Per( sp_delay );
		sp_output = Mix_Level.Per( sp_delay );

		sp_input  = LIMIT_INT16( sp_input );
		sp_output = LIMIT_INT16( sp_output );

		Buffer.Set_Value( sp_input );

		return sp_output;
	}

	int Process( int input )
	{
		return LPF_Post( Sub_Process( LPF_Pre( input ) ) );
	}

	void Set_Param_0( int v )	{ Feedback.Set_Value( v ); }
	int Get_Param_0() const		{ return Feedback.Initial_Value; }

	void Set_Param_1( int v ) { Mix_Level.Set_Value( v ); }
	int Get_Param_1() const		{ return Mix_Level.Initial_Value; }

	void Set_Param_2( int v )	{ Time_Length.Set_Value( v );	}
	int Get_Param_2() const		{ return Time_Length.Initial_Value; }

	void Set_Param_3( int ms )
	{
		int	length = _MS_2_LENGTH( ms, _FS_/FS_RATIO );
		length = Limit<int>( 1, length, Buffer.Memory.Length-1 );
		Time_Length.Set_Value( Map( length, 1, Buffer.Memory.Length-1, 0, UINT12_MAX ) );
	}

	int Get_Param_3() const
	{
		int length = Map( Time_Length.Initial_Value, 0, UINT12_MAX, 1, Buffer.Memory.Length-1 );
		return _LENGTH_2_MS( length, _FS_/FS_RATIO );
	}

	FX_ID Get_FX_ID() const	{ return FX_ID_Delay; }

	void Reset()
	{
		Buffer.Memory.Reset();
		Buffer.Set_Length( Map( Time_Length.Get_Value(), 0, UINT12_MAX, 1, Buffer.Memory.Length-1 ) );
		Buffer.Fast_Forward();
		Feedback.Fast_Forward();
		Mix_Level.Fast_Forward();
		LPF_Pre.Reset();
		LPF_Post.Reset();
		Sub_Process.Reset();
		sp_input = sp_output = sp_delay = 0;
	}
};

#endif
