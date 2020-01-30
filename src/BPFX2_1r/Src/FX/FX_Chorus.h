#ifndef FX_CHORUS_H_
#define FX_CHORUS_H_

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>
#include <Chorus_Buffer.h>

struct FX_Chorus : public FX_Interface
{
	static constexpr Q15T_BQF_Params HPF_Params = BQF_HPF(   200.f, 0.75f );
	static constexpr Q15T_BQF_Params LPF_Params = BQF_LPF( 10000.f, 0.75f );

	static const int	DEPTH_BUFFER_LENGTH = _MS_2_LENGTH( 10, _FS_ );
	static const int	DELAY_BUFFER_LENGTH = _MS_2_LENGTH(  5, _FS_ );

	Chorus_Buffer			Buffer;
	Q15T_LFO					LFO;

	Volume<Curve_D>		Rate;
	Volume<Curve_D>		Depth;
	Volume<Curve_B>		Mix_Level;

	Q15T_BQF					HPF, LPF;

	Sub_Process_2<FX_Chorus>		Sub_Process;
	int													_input_, _output_;	// for sub process

	FX_Chorus():
		Buffer( DEPTH_BUFFER_LENGTH*2 + DELAY_BUFFER_LENGTH ),
		LFO( _FS_, int16_t_Sin_Table ), Sub_Process( this )
	{
		Mix_Level.Set_Value( UINT12_MAX*6/10 );
		HPF = HPF_Params;
		LPF = LPF_Params;
	}

	void SUB_PROCESS_0( int input )
	{
		_input_ = input;

		// Set Rate
		{
			int v = Map( Rate.Get_Value(), 0, UINT12_MAX, 41, UINT12_MAX );
			LFO.Set_Rate( Fraction( v, 409 ) );
		}

		Buffer.Set_Value( _input_ );
	}

	int SUB_PROCESS_1()
	{
		int		DEPTH = Depth.Per( DEPTH_BUFFER_LENGTH );
		Q15T	t			= LFO.Get_Value() * DEPTH + (DEPTH+1) + DELAY_BUFFER_LENGTH;
		int		m			= t.to_int();
		Q15T	delta	= t - Q15T( m );

		_output_ = ( delta * Buffer.Get_Value( m + 1 ) +	( Q15T_1 - delta ) * Buffer.Get_Value( m ) ).to_int();

		_output_ = LIMIT_INT16( _output_ );

		_output_ =  Mix_Level.Per( _output_ );

		return _output_;
	}

	int Process( int input )
	{
		return LPF( Sub_Process( HPF( input ) ) );
	}

	virtual void Set_Param_0( int v )	{ Rate.Set_Value( Map( v, 0, UINT12_MAX, 1, UINT12_MAX ) ); }
	virtual int  Get_Param_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Param_1( int v )	{ Mix_Level.Set_Value( v ); }
	virtual int  Get_Param_1() const	{ return Mix_Level.Initial_Value; }

	virtual void Set_Param_2( int v )	{ Depth.Set_Value( Map( v, 0, UINT12_MAX, 10, UINT12_MAX ) ); }
	virtual int  Get_Param_2() const	{	return Depth.Initial_Value; }

	virtual FX_ID Get_FX_ID() const	{ return FX_ID_Chorus; }

	void Reset()
	{
		LFO.Reset();
		HPF.Reset();
		LPF.Reset();
		Rate.Fast_Forward();
		Depth.Fast_Forward();
		Mix_Level.Fast_Forward();
		Buffer.Memory.Reset();
	}
};

#endif
