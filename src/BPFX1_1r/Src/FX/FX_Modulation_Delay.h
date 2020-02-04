#ifndef FX_MODULATION_DELAY_H_
#define FX_MODULATION_DELAY_H_

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>
#include <Q15T_LFO_Tri.h>
#include <Modulation_Delay_Buffer.h>

struct FX_Modulation_Delay : public FX_Interface
{
	static const	int FS_RATIO = 4;
	
	Sub_Process_4<FX_Modulation_Delay>	Sub_Process;
	int																	sp_input, sp_output, sp_wet, sp_fb;

	static constexpr Q15T_BQF_Params LPF_Params		= BQF_LPF( 4000.f, 0.75f );
	static constexpr Q15T_BQF_Params LPF_Params_2 = BQF_LPF( 2000.f, 1.0f, _FS_/FS_RATIO );

	static const int	WIDTH = _MS_2_LENGTH( 20, FS_DIV_4 );
	static const int	DELAY = FX_MODULATION_DELAY_BUFFER_LENGTH;

	Modulation_Delay_Buffer			Buffer;

	Q15T_LFO<FS_DIV_4>		LFO;

	Volume<Curve_F>			Rate;				// Control_0
	Volume_x<Curve_D>		Delay_Time;	// Control_1
	Volume<Curve_F>			Depth;			// Control_2
	Volume<Curve_C>			Feedback;		// Control_3
	Volume<Curve_B>			Mix_Level;	// Control_4

	Q15T_BQF						LPF_Pre, LPF_Post, LPF;;

	FX_Modulation_Delay():
		Sub_Process( this ),
		sp_input( 0 ), sp_output( 0 ), sp_wet( 0 ), sp_fb( 0 ),
		Buffer( WIDTH*2 + DELAY )
		,LFO( Sin_Table )
	{
		LPF_Pre		= LPF_Params;
		LPF_Post	= LPF_Params;
		LPF 			= LPF_Params_2;
	}

	void SUB_PROCESS_0( int input )
	{
		sp_input = input;

		Buffer.Set_Length( Map( Delay_Time.Get_Value(), 0, UINT12_MAX, WIDTH*2, DELAY-1 ) );

		LFO.Set_Rate( Fraction( Map( Rate.Get_Value(), 0, UINT12_MAX, 2, UINT12_MAX ), 300 ) );
	}

	void SUB_PROCESS_1()
	{
		sp_wet = Buffer.Get_Value( LFO.Get_Value() * ( Depth * WIDTH ) + WIDTH );
	}

	void SUB_PROCESS_2()
	{
		sp_wet = LPF( sp_wet );
		sp_fb = sp_input + Feedback * sp_wet;
	}

	int SUB_PROCESS_3()
	{
		Buffer.Set_Value( sp_fb );
		return Mix_Level * sp_wet;
	}

	int Process( int input )
	{
		return LPF_Post( Sub_Process( LPF_Pre( input ) ) );
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
		Depth.Fast_Forward();
		Feedback.Fast_Forward();
		Mix_Level.Fast_Forward();
		Buffer.Memory.Reset();
		Buffer.Fast_Forward();
		LPF_Pre.Reset();
		LPF_Post.Reset();
		LPF.Reset();
		sp_input = sp_output = sp_wet = sp_fb = 0;
	}
};

#endif /* FX_CHORUS_FLANGER_H_ */
