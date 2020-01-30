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
	static constexpr Q15T_BQF_Params LPF_Params = BQF_LPF( 10000.f, 0.75f );

	//	static const int				BUFFER_LENGTH	= FX_TWO_TAP_DELAY_BUFFER_LENGTH;

	int											Time_Length;	// 0...max buffer length
	Volume<Curve_B>					Feedback;			// 0...4095
	Volume<Curve_B>					Mix_Level;		// 0...4095

	Two_Tap_Delay_Buffer		Buffer;

	Q15T_BQF								LPF_Pre, LPF_Post;

	Sub_Process_2<FX_Two_Tap_Delay>		Sub_Process;
	int																_input_, _output_, _delay_;

	FX_Two_Tap_Delay():
		Time_Length( 0 ), Feedback( 0 ), Mix_Level( 0 ),
		Buffer( FX_TWO_TAP_DELAY_BUFFER_LENGTH ),
		Sub_Process( this ),
		_input_( 0 ), _output_( 0 ), _delay_( 0 )
	{
		LPF_Pre		= LPF_Params;
		LPF_Post	= LPF_Params;
	}

	void SUB_PROCESS_0( int input )
	{
		_input_ = input;

		Buffer.Set_Length( Time_Length );

		_delay_ = Buffer.Get_Value();
	}

	int SUB_PROCESS_1()
	{
		_input_	-= Feedback.Per( _delay_ );
		_output_ = Mix_Level.Per( _delay_ );

		_input_  = LIMIT_INT16( _input_ );
		_output_ = LIMIT_INT16( _output_ );

		Buffer.Set_Value( _input_ );

		return _output_;
	}

	int Process( int input )
	{
		return LPF_Post( Sub_Process( LPF_Pre( input ) ) );
	}

	void Set_Param_0( int v )	{ Feedback.Set_Value( Map( v, 0, UINT12_MAX, 0, UINT12_MAX*8/10 ) ); }
	int Get_Param_0() const	{ return Map( Feedback.Initial_Value, 0, UINT12_MAX*8/10, 0, UINT12_MAX ); }

	void Set_Param_1( int v ) { Mix_Level.Set_Value( v ); }
	int Get_Param_1() const	{ return Mix_Level.Initial_Value; }

	void Set_Param_2( int v )
	{
		Time_Length = Map( v, 0, UINT12_MAX, 2, Buffer.Memory.Length-1 );
		Reset();
	}

	int Get_Param_2() const { return Map( Time_Length, 2, Buffer.Memory.Length-1, 0, UINT12_MAX ); }

	FX_ID Get_FX_ID() const	{ return FX_ID_Two_Tap_Delay; }

	void Reset()
	{
		Feedback.Fast_Forward();
		Mix_Level.Fast_Forward();
		Buffer.Set_Length( Time_Length );
		Buffer.Reset();
		_input_ = _output_ = _delay_ = 0;
		Sub_Process.Reset();
	}
};

#endif
