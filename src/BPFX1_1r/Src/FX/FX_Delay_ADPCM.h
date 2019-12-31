#ifndef	FX_Delay_ADPCM_h
#define	FX_Delay_ADPCM_h

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Delay_ADPCM_Buffer.h>
#include <FX_Config.h>

constexpr Q15T_BQF_Param FX_Delay_ADPCM_LPF_Param   = BQF_Builder( _FS_   ).LPF( 10000.f, 0.75f );
constexpr Q15T_BQF_Param FX_Delay_ADPCM_LPF_Param_2 = BQF_Builder( _FS_/2 ).LPF(  8000.f, 0.75f );

struct FX_Delay_ADPCM : public FX_Interface
{
//	static const int			BUFFER_LENGTH	= FX_DELAY_ADPCM_BUFFER_LENGTH;

	int										Time_Length;	// 0...max buffer length
	Volume<Curve_B>				Feedback;			// 0...4095
	Volume<Curve_B>				Mix_Level;		// 0...4095

	Delay_ADPCM_Buffer		Buffer;

	Q15T_BQF							LPF_Pre, LPF_Post;

	Sub_Process_2<FX_Delay_ADPCM>	Sub_Process;
	int														_input_, _output_, _delay_;

	FX_Delay_ADPCM() :
		Time_Length( 0 ), Feedback( 0 ), Mix_Level( 0 ),
		Buffer( FX_DELAY_ADPCM_BUFFER_LENGTH ),
		Sub_Process( this ),
		_input_( 0 ), _output_( 0 ), _delay_( 0 )
	{
		LPF_Pre = FX_Delay_ADPCM_LPF_Param;
		LPF_Post = FX_Delay_ADPCM_LPF_Param;
	}

	void Sub_Process_0( int input )
	{
		_input_ = input;
		Buffer.Set_Length( Time_Length );
		_delay_ = Buffer.Get_Value();
	}

	int Sub_Process_1()
	{
		_input_	+= Feedback.Per( _delay_ );
		_output_ = Mix_Level.Per( _delay_ );

		_input_  = LIMIT_INT16( _input_ );
		_output_ = LIMIT_INT16( _output_ );

		Buffer.Set_Value( _input_ );

		return _output_;
	}

	int Process( int input )
	{
		input *= 12;
		input /= 10;
		return LPF_Post( Sub_Process( LPF_Pre( input ) ) );
	}

	void Set_Param_0( int v )	{ Feedback.Set_Value( v ); }
	int Get_Param_0() const	{ return Feedback.Initial_Value; }

	void Set_Param_1( int v ) { Mix_Level.Set_Value( v ); }
	int Get_Param_1() const	{ return Mix_Level.Initial_Value; }

	void Set_Param_2( int v )
	{
		v = v / 10;
		v = v * 10;

		v = Map( v, 0, UINT12_MAX, 1, Buffer.Memory.Length-1 );

		if( Time_Length != v )
		{
			Time_Length = v;
			Reset();
		}
	}

	int Get_Param_2() const { return Map( Time_Length, 1, Buffer.Memory.Length-1, 0, UINT12_MAX ); }

	FX_ID Get_FX_ID() const	{ return FX_ID_Delay_ADPCM; }

	void Reset()
	{
		Feedback.Fast_Forward();
		Mix_Level.Fast_Forward();
		LPF_Pre.Reset();
		LPF_Post.Reset();
		Buffer.Reset();
		_input_ = _output_ = _delay_ = 0;
		Sub_Process.Reset();
	}
};

#endif
