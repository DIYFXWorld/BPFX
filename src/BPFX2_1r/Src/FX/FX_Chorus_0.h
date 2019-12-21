#ifndef FX_CHORUS_0_H_
#define FX_CHORUS_0_H_

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>
#include <Chorus_Buffer.h>

constexpr Q15T_BQF_Param FX_Chorus_0_HPF_Param = BQF_Builder( _FS_ ).HPF(   200.f, 0.75f );
constexpr Q15T_BQF_Param FX_Chorus_0_LPF_Param = BQF_Builder( _FS_ ).LPF( 10000.f, 0.75f );

struct FX_Chorus_0 : public FX_Interface
{
	static const int	DEPTH_BUFFER_LENGTH = _MS_2_LENGTH( 10, _FS_ );
	static const int	DELAY_BUFFER_LENGTH = _MS_2_LENGTH(  5, _FS_ );

	Chorus_Buffer			Buffer;
	Q15T_LFO					LFO;

	Volume<Curve_D>		Rate;
	Volume<Curve_D>		Depth;
	Volume<Curve_B>		Mix_Level;

	Q15T_BQF					HPF, LPF;

	FX_Chorus_0():
		Buffer( DEPTH_BUFFER_LENGTH*2 + DELAY_BUFFER_LENGTH ),
		LFO( _FS_, int16_t_Sin_Table )
	{
		Mix_Level.Set_Value( UINT12_MAX*6/10 );
		HPF = FX_Chorus_0_HPF_Param;
		LPF = FX_Chorus_0_LPF_Param;
		Reset();
	}

	void Destroy() { delete this; }

	int Process( int input )
	{
		input = HPF.Process( input );

		// Set Rate
		{
			int v = Map( Rate.Get_Value(), 0, UINT12_MAX, 41, UINT12_MAX );
			LFO.Set_Rate( Fraction( v, 409 ) );
		}

		int	DEPTH = Depth.Per( DEPTH_BUFFER_LENGTH );

		Buffer.Set_Value( input );

		Q15T	t			= LFO.Get_Value() * DEPTH + (DEPTH+1) + DELAY_BUFFER_LENGTH;
		int		m			= t.to_int();
		Q15T	delta	= t - Q15T( m );

		int	output = ( delta * Buffer.Get_Value( m + 1 ) +	( Q15T_1 - delta ) * Buffer.Get_Value( m ) ).to_int();

		output = LIMIT_INT16( output );

		output = LPF.Process( output );

		output =  Mix_Level.Per( output );

		return output;
	}

	virtual void Set_Param_0( int v )	{ Rate.Set_Value( Map( v, 0, UINT12_MAX, 1, UINT12_MAX ) ); }
	virtual int  Get_Param_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Param_1( int v )	{ Mix_Level.Set_Value( v ); }
	virtual int  Get_Param_1() const	{ return Mix_Level.Initial_Value; }

	virtual void Set_Param_2( int v )	{ Depth.Set_Value( Map( v, 0, UINT12_MAX, 10, UINT12_MAX ) ); }
	virtual int  Get_Param_2() const	{	return Depth.Initial_Value; }

	virtual FX_ID Get_FX_ID() const	{ return FX_ID_Chorus_0; }

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