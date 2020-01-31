#ifndef FX_MODULATION_DELAY_H_
#define FX_MODULATION_DELAY_H_

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>
#include <Modulation_Delay_Buffer.h>

struct FX_Modulation_Delay : public FX_Interface
{
	Sub_Process_4<FX_Modulation_Delay>	Sub_Process;
	int																	sp_input, sp_output, sp_wet, sp_fb;

	static constexpr Q15T_BQF_Params LPF_Params = BQF_LPF( 5000.f, 1.f );

	static const int	DEPTH_BUFFER_LENGTH = _MS_2_LENGTH(  20, FS_DIV_4 );
	static const int	DELAY_BUFFER_LENGTH = FX_MODULATION_DELAY_BUFFER_LENGTH;

	Modulation_Delay_Buffer			Buffer;

	Q15T_LFO<FS_DIV_4>		LFO;

	Volume<Curve_F>		Rate;				// Control_0
	Volume<Curve_B>		Delay_Time;	// Control_1
	Volume<Curve_F>		Depth;			// Control_2
	Volume<Curve_B>		Feedback;		// Control_3
	Volume<Curve_B>		Mix_Level;	// Control_4

	Q15T_BQF					LPF_0, LPF_1;

	FX_Modulation_Delay():
		Sub_Process( *this ),
		sp_input( 0 ), sp_output( 0 ), sp_wet( 0 ), sp_fb( 0 ),
		Buffer( DEPTH_BUFFER_LENGTH*2 + DELAY_BUFFER_LENGTH ),
		LFO( int16_t_Sin_Table )
	{
		LPF_0 = LPF_Params;
		LPF_1 = LPF_Params;
	}

	void SUB_PROCESS_0( int input )
	{
		sp_input = input;

		int v = Map( Rate.Get_Value(), 0, UINT12_MAX, 2, UINT12_MAX );
		LFO.Set_Rate( Fraction( v, 300 ) );
	}

	void SUB_PROCESS_1()
	{
		int	DEPTH				= Depth.Per( DEPTH_BUFFER_LENGTH-1 );
		int	DELAY_TIME	= Delay_Time.Per( DELAY_BUFFER_LENGTH-1 );

		Q15T	t			= LFO.Get_Value() * DEPTH + (DEPTH+1) + DELAY_TIME;
		int		m			= t.to_int();
		Q15T	delta	= t - Q15T( m );
		sp_wet = ( delta * Buffer.Get_Value( m + 1 ) +	( Q15T_1 - delta ) * Buffer.Get_Value( m ) ).to_int();
	}

	void SUB_PROCESS_2()
	{
		sp_fb = sp_input - Feedback.Per( sp_wet );
		sp_fb = LIMIT_INT16( sp_fb );
	}

	int SUB_PROCESS_3()
	{
		Buffer.Set_Value( sp_fb );
		sp_wet = Mix_Level.Per( sp_wet );
		sp_wet = LIMIT_INT16( sp_wet );
		return sp_wet;
	}

	int Process( int input )
	{
		return LPF_1( Sub_Process( LPF_0( input ) ) );
	}

	virtual void Set_Param_0( int v )	{ Rate.Set_Value( v ); }
	virtual int  Get_Param_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Param_1( int v )	{ Delay_Time.Set_Value( v ); }
	virtual int  Get_Param_1() const	{ return Delay_Time.Initial_Value; }

	virtual void Set_Param_2( int v )	{ Depth.Set_Value( v ); }
	virtual int  Get_Param_2() const	{	return Depth.Initial_Value; }

	virtual void Set_Param_3( int v )	{ Feedback.Set_Value( v ); }
	virtual int  Get_Param_3() const	{ return Feedback.Initial_Value; }

	virtual void Set_Param_4( int v )	{ Mix_Level.Set_Value( v ); }
	virtual int  Get_Param_4() const	{ return Mix_Level.Initial_Value; }

	virtual FX_ID Get_FX_ID() const	{ return FX_ID_Modulation_Delay; }

	void Reset()
	{
		LFO.Reset();
		Rate.Fast_Forward();
		Delay_Time.Fast_Forward();
		Depth.Fast_Forward();
		Feedback.Fast_Forward();
		Mix_Level.Fast_Forward();
		Buffer.Memory.Reset();
		LPF_0.Reset();
		LPF_1.Reset();
		sp_input = sp_output = sp_wet = sp_fb = 0;
	}
};

#endif /* FX_CHORUS_FLANGER_H_ */
