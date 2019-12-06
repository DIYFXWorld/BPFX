#ifndef	FX_Delay_0_h
#define	FX_Delay_0_h

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Delay_Buffer.h>

static constexpr	BQF_Param FX_Delay_0_LPF_Param = BQF_Builder( _FS_ ).LPF( 20000.f, 0.75f );

struct FX_Delay_0 : public FX_Interface
{
	static const int	BUFFER_LENGTH	= 8000;

	int								Time_Length;	// 0...max buffer length
	Volume<Curve_B>		Feedback;			// 0...4095
	Volume<Curve_B>		Mix_Level;		// 0...4095

	Delay_Buffer			Buffer;

	Q15T_BQF					LPF_Pre, LPF_Post;

	FX_Delay_0():
		Time_Length( 0 ), Feedback( 0 ), Mix_Level( 0 ),
		Buffer( BUFFER_LENGTH )
	{
		LPF_Pre = FX_Delay_0_LPF_Param;
		LPF_Post = FX_Delay_0_LPF_Param;
	}

	void Destroy() { delete this; }

	int Process( int input )
	{
		input = LPF_Pre( input );

		Buffer.Set_Length( Time_Length );

		int	delay = Buffer.Get_Value();
		input	-= Feedback.Per( delay );
		int	output = Mix_Level.Per( delay );

		input  = LIMIT_INT16( input );
		output = LIMIT_INT16( output );

		Buffer.Set_Value( input );

		output = LPF_Post( output );

		return output;
	}

	void Set_Param_0( int v )	{ Feedback.Set_Value( v ); }
	int Get_Param_0() const	{ return Feedback.Initial_Value; }

	void Set_Param_1( int v ) { Mix_Level.Set_Value( v ); }
	int Get_Param_1() const	{ return Mix_Level.Initial_Value; }

	void Set_Param_2( int v )
	{
		v = Map( v, 0, UINT12_MAX, 1, Buffer.Memory.Length-1 );
		Time_Length = v;
	}

	int Get_Param_2() const
	{
		return Map( Time_Length, 1, Buffer.Memory.Length-1, 0, UINT12_MAX );
	}

	FX_ID Get_FX_ID() const	{ return FX_ID_Delay_0; }

	void Reset()
	{
		Feedback.Fast_Forward();
		Mix_Level.Fast_Forward();
		Buffer.Set_Length( Time_Length );
		Buffer.Fast_Forward();
		Buffer.Memory.Reset();
		LPF_Pre.Reset();
		LPF_Post.Reset();
	}
};

#endif
