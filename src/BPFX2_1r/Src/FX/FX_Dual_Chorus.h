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

constexpr Q15T_BQF_Param FX_Dual_Chorus_HPF_Param = BQF_Builder( _FS_ ).HPF(   200.f, 0.75f );
constexpr Q15T_BQF_Param FX_Dual_Chorus_LPF_Param = BQF_Builder( _FS_ ).LPF( 10000.f, 0.75f );

struct FX_Dual_Chorus : public FX_Interface
{
	static const int	DEPTH_BUFFER_LENGTH = _MS_2_LENGTH( 10, _FS_ );
	static const int	DELAY_BUFFER_LENGTH = _MS_2_LENGTH(  5, _FS_ );

	Chorus_Buffer			Buffer_0, Buffer_1;
	Q15T_LFO					LFO_0, LFO_1;

	Volume<Curve_D>		Rate_0, Rate_1;
	Volume<Curve_D>		Depth_0, Depth_1;
	Volume<Curve_B>		Mix_Level;

	Q15T_BQF					HPF, LPF;

	Sub_Process_2<FX_Dual_Chorus>		Sub_Process;
	int														_input_, _output_;	// for sub process

	FX_Dual_Chorus():
		Buffer_0( DEPTH_BUFFER_LENGTH*2 + DELAY_BUFFER_LENGTH ),
		Buffer_1( DEPTH_BUFFER_LENGTH*2 + DELAY_BUFFER_LENGTH ),
		LFO_0( _FS_, int16_t_Sin_Table ),
		LFO_1( _FS_, int16_t_Sin_Table ),
		Sub_Process( this )
	{
		Mix_Level.Set_Value( UINT12_MAX*6/10 );
		HPF = FX_Dual_Chorus_HPF_Param;
		LPF = FX_Dual_Chorus_LPF_Param;
	}

	void Destroy() { delete this; }

	void Sub_Process_0( int input )
	{
		_input_ = input;

		// Set Rate
		{
			int v = Map( Rate_0.Get_Value(), 0, UINT12_MAX, 41, UINT12_MAX );
			LFO_0.Set_Rate( Fraction( v, 409 ) );
		}

		Buffer_0.Set_Value( _input_ );

		int		DEPTH = Depth_0.Per( DEPTH_BUFFER_LENGTH );
		Q15T	t			= LFO_0.Get_Value() * DEPTH + (DEPTH+1) + DELAY_BUFFER_LENGTH;
		int		m			= t.to_int();
		Q15T	delta	= t - Q15T( m );

		_output_ = ( ( delta          ) * Buffer_0.Get_Value( m + 1 ) +
							   ( Q15T_1 - delta ) * Buffer_0.Get_Value( m     ) ).to_int();
	}

	int Sub_Process_1()
	{
		// Set Rate
		{
			int v = Map( Rate_1.Get_Value(), 0, UINT12_MAX, 41, UINT12_MAX );
			LFO_1.Set_Rate( Fraction( v, 409 ) );
		}

		Buffer_1.Set_Value( _output_ );

		int		DEPTH = Depth_1.Per( DEPTH_BUFFER_LENGTH );
		Q15T	t			= LFO_1.Get_Value() * DEPTH + (DEPTH+1) + DELAY_BUFFER_LENGTH;
		int		m			= t.to_int();
		Q15T	delta	= t - Q15T( m );

		_output_ += ( ( delta          ) * Buffer_1.Get_Value( m + 1 ) +
							    ( Q15T_1 - delta ) * Buffer_1.Get_Value( m     ) ).to_int();

		_output_ = LIMIT_INT16( _output_ );

		_output_ =  Mix_Level.Per( _output_ );

		return _output_;
	}

	int Process( int input )
	{
		return LPF( Sub_Process( HPF( input ) ) );
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
		LPF.Reset();
		Rate_0.Fast_Forward();
		Rate_1.Fast_Forward();
		Depth_0.Fast_Forward();
		Depth_1.Fast_Forward();
		Mix_Level.Fast_Forward();
		Buffer_0.Memory.Reset();
		Buffer_1.Memory.Reset();
	}
};

#endif
