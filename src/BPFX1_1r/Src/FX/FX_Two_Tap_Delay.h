#ifndef	FX_Two_Tap_Delay_h
#define	FX_Two_Tap_Delay_h

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Two_Tap_Delay_Buffer.h>

struct FX_Two_Tap_Delay : public FX_Interface
{
	static const int FS_RATIO = 2;

	static constexpr Q15T_BQF_Params LPF_Params = 	BQF_LPF( _FS_/FS_RATIO/2, 0.75f );

	//	static const int				BUFFER_LENGTH	= FX_TWO_TAP_DELAY_BUFFER_LENGTH;

	int											Time_Length;	// 0...4095
	Volume<Curve_B>					Feedback;			// 0...4095
	Volume<Curve_B>					Mix_Level;		// 0...4095

	Two_Tap_Delay_Buffer		Buffer;

	Q15T_BQF								LPF_Pre, LPF_Post;

	Sub_Process_2<FX_Two_Tap_Delay>		Sub_Process;
	int																sp_input, sp_wet;
	
	bool										Lock;

	FX_Two_Tap_Delay():
		Time_Length( 0 ), Feedback( 0 ), Mix_Level( 0 ),
		Buffer( FX_TWO_TAP_DELAY_BUFFER_LENGTH ),
		Sub_Process( this ),
		sp_input( 0 ), sp_wet( 0 ),
		Lock( false )
	{
		LPF_Pre		= LPF_Params;
		LPF_Post	= LPF_Params;
	}

	void SUB_PROCESS_0( int input )
	{
		sp_input = input;

		sp_wet = Buffer.Get_Value();
	}

	int SUB_PROCESS_1()
	{
		Buffer.Set_Value( sp_input - Feedback * sp_wet );

		return Mix_Level * sp_wet;
	}

	int Process( int input )
	{
		if( Lock )	return 0;
		return LPF_Post( Sub_Process( LPF_Pre( input ) ) );
	}

	void Set_Param_0( int v )	{ Feedback.Set_Value( Map( v, 0, UINT12_MAX, 0, UINT12_MAX*8/10 ) ); }
	int Get_Param_0() const		{ return Map( Feedback.Initial_Value, 0, UINT12_MAX*8/10, 0, UINT12_MAX ); }

	void Set_Param_1( int v ) { Mix_Level.Set_Value( v ); }
	int Get_Param_1() const		{ return Mix_Level.Initial_Value; }

	void Set_Param_2( int v )
	{ 
		Lock = true;
		Time_Length = v;
		Buffer.Set_Length( Map( v, 0, UINT12_MAX, 2, Buffer.Memory.Length-1 ) );
		Lock = false;
	}

	int Get_Param_2() const		{ return Time_Length; }

	FX_ID Get_FX_ID() const	{ return FX_ID_Two_Tap_Delay; }

	void Reset()
	{
		Feedback.Fast_Forward();
		Mix_Level.Fast_Forward();
		Buffer.Set_Length( Map( Time_Length, 0, UINT12_MAX, 2, Buffer.Memory.Length-1 ) );
		LPF_Pre.Reset();
		LPF_Post.Reset();
		Sub_Process.Reset();
		sp_input = sp_wet = 0;
	}
};

#endif
