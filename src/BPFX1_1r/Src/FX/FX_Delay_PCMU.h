#ifndef	FX_Delay_PCMU_h
#define	FX_Delay_PCMU_h

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Delay_Buffer_PCMU.h>

struct FX_Delay_PCMU : public FX_Interface
{
	static const int FS_RATIO = 2;

	static constexpr Q15T_BQF_Params LPF_Params   = BQF_LPF( _FS_/FS_RATIO/2, 0.75f );

//	static const int		BUFFER_LENGTH	= FX_DELAY_PCMU_BUFFER_LENGTH;

	Volume_x<Curve_B>		Time_Length;	// 0...4095
	Volume<Curve_B>			Feedback;			// 0...4095
	Volume<Curve_B>			Mix_Level;		// 0...4095

	Delay_Buffer_PCMU		Buffer;

	Q15T_BQF						LPF_Pre, LPF_Post;

	Sub_Process_2<FX_Delay_PCMU>	Sub_Process;
	int														sp_input, sp_output, sp_delay;

	FX_Delay_PCMU() :
		Time_Length( 0 ), Feedback( 0 ), Mix_Level( 0 ),
		Buffer( FX_DELAY_PCMU_BUFFER_LENGTH ),
		Sub_Process( this ),
		sp_input( 0 ), sp_output( 0 ), sp_delay( 0 )
	{
		LPF_Pre		= LPF_Params;
		LPF_Post	= LPF_Params;
	}

	void SUB_PROCESS_0( int v )
	{
		sp_input = v;

		Buffer.Set_Length( Map( Time_Length.Get_Value(), 0, UINT12_MAX, 1, Buffer.Memory.Length-1 ) );

		sp_delay = Buffer.Get_Value();
	}

	int SUB_PROCESS_1()
	{
		Buffer.Set_Value( sp_input - Feedback * sp_delay );

		return Mix_Level * sp_delay;
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

	FX_ID Get_FX_ID() const	{ return FX_ID_Delay_PCMU; }

	void Reset()
	{
		Feedback.Fast_Forward();
		Mix_Level.Fast_Forward();
		Buffer.Set_Length( Time_Length.Get_Value() );
		Buffer.Fast_Forward();
		Buffer.Reset();
		LPF_Pre.Reset();
		LPF_Post.Reset();
		Sub_Process.Reset();
		sp_input = sp_output = sp_delay = 0;
	}
};

#endif
