#ifndef	FX_Analog_Delay_h
#define	FX_Analog_Delay_h

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Delay_Buffer.h>

struct FX_Analog_Delay : public FX_Interface
{
	static const int	FS_RATIO	= 4;

	static constexpr Q15T_BQF_Params LPF_Params_0 = BQF_LPF( 3000.f, 0.75f );
	static constexpr Q15T_BQF_Params LPF_Params_1 = BQF_LPF( 1500.f, 1.f, _FS_/FS_RATIO );

//	static const int			BUFFER_LENGTH	= FX_ANALOG_DELAY_BUFFER_LENGTH;

	Volume_x<Curve_D>		Time_Length;	// 0...4095
	Volume<Curve_B>			Feedback;			// 0...4095
	Volume<Curve_B>			Mix_Level;		// 0...4095

	Delay_Buffer				Buffer;

	Q15T_BQF						LPF_Pre, LPF_Post, LPF;

	Sub_Process_4<FX_Analog_Delay>	Sub_Process;
	int															sp_input, sp_output, sp_delay; // for sub process

	FX_Analog_Delay():
		Time_Length( 0 ), Feedback( 0 ), Mix_Level( 0 ),
		Buffer( FX_ANALOG_DELAY_BUFFER_LENGTH ),
		Sub_Process( this ),
		sp_input( 0 ), sp_output( 0 ), sp_delay( 0 )
	{
		LPF_Pre 	= LPF_Params_0;
		LPF_Post	= LPF_Params_0;
		LPF 			= LPF_Params_1;
	}

	void SUB_PROCESS_0( int v )
	{
		sp_input = v;
		Buffer.Set_Length( Map( Time_Length.Get_Value(), 0, UINT12_MAX, 1, Buffer.Memory.Length-1 ) );
	}

	void SUB_PROCESS_1()
	{
		sp_delay	= Buffer.Get_Value();
	}

	void SUB_PROCESS_2()
	{
		sp_delay  = LPF( sp_delay );
		sp_input	-= Feedback * sp_delay;
		sp_output = Mix_Level * sp_delay;
	}

	int SUB_PROCESS_3()
	{
		Buffer.Set_Value( sp_input );
		return sp_output * (11/10);
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

	FX_ID Get_FX_ID() const	{ return FX_ID_Analog_Delay; }

	void Reset()
	{
		Buffer.Memory.Reset();
		Buffer.Set_Length( Map( Time_Length.Get_Value(), 0, UINT12_MAX, 1, Buffer.Memory.Length-1 ) );
		Buffer.Fast_Forward();
		Feedback.Fast_Forward();
		Mix_Level.Fast_Forward();
		LPF_Pre.Reset();
		LPF_Post.Reset();
		LPF.Reset();
		Sub_Process.Reset();
		sp_input = sp_output = sp_delay = 0;
	}
};

#endif
