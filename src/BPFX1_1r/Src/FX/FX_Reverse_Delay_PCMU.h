#ifndef	FX_Reverse_Delay_PCMU_h
#define	FX_Reverse_Delay_PCMU_h

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Reverse_Delay_Buffer_PCMU.h>
#include <Mute.h>

constexpr Q15T_BQF_Param FX_Revese_Delay_PCMU_LPF_Param = BQF_Builder( _FS_ ).LPF( 10000.f, 0.75f );

struct FX_Reverse_Delay_PCMU : public FX_Interface
{
//	static const int 						BUFFER_LENGTH	= 7950*2;

	Volume<Curve_B>							Time_Length;
	Volume<Curve_B>							Feedback;
	Volume<Curve_B>							Mix_Level;

	Reverse_Delay_Buffer_PCMU		Buffer;
	Type_Mute										Mute;

	Q15T_BQF										LPF_0, LPF_1;

	Sub_Process_2<FX_Reverse_Delay_PCMU>	Sub_Process;
	int	m_input, m_output, m_delay;

	FX_Reverse_Delay_PCMU():
		Time_Length( 0 ), Feedback( 0 ), Mix_Level( 0 ),
		Buffer( FX_REVERSE_DELAY_PCMU_BUFFER_LENGTH ),
		Mute( 40 ),
		Sub_Process( this ),
		m_input( 0 ), m_output( 0 ), m_delay( 0 )
	{
		LPF_0 = FX_Revese_Delay_PCMU_LPF_Param;
		LPF_1 = FX_Revese_Delay_PCMU_LPF_Param;
	}

	void Destroy() { delete this; }

	void Sub_Process_0( int input )
	{
		m_input = input;

		Buffer.Set_Length( Time_Length.Get_Value() );

		if( Buffer.Pointer == Buffer.Length - Mute.Length + 1 )
			if( Buffer.Direction > 0 )
				Mute.Start();

		if( Buffer.Pointer == Mute.Length - 2 )
			if( Buffer.Direction < 0 )
				Mute.Start();

		m_delay = Buffer.Get_Value();
		m_delay = Mute.Process( m_delay );
	}

	int Sub_Process_1()
	{
		m_input	-= Feedback.Per( m_delay );
		m_output = Mix_Level.Per( m_delay );

		m_input  = LIMIT_INT16( m_input );
		m_output = LIMIT_INT16( m_output );

		Buffer.Set_Value( m_input );

		return m_output;
	}

	int Process( int input )
	{
		input = LPF_0.Process( input );

		int	output = Sub_Process.Do( input );

		output = LPF_1.Process( output );

		return output;
	}

	void Set_Param_0( int v )	{ Feedback.Set_Value( v ); }
	int Get_Param_0() const		{ return Feedback.Initial_Value; }

	void Set_Param_1( int v ) { Mix_Level.Set_Value( v ); }
	int Get_Param_1() const		{ return Mix_Level.Initial_Value; }

	void Set_Param_2( int v )	{ Time_Length.Set_Value( Map( v, 0, UINT12_MAX, 100, Buffer.Memory.Length-1 ) ); }
	int Get_Param_2() const		{ return Time_Length.Initial_Value; }

	FX_ID Get_FX_ID() const	{ return FX_ID_Reverse_Delay_PCMU; }

	void Reset()
	{
		Time_Length.Fast_Forward();
		Feedback.Fast_Forward();
		Mix_Level.Fast_Forward();

		Buffer.Clear();
		Buffer.Set_Length( Time_Length.Get_Value() );
		Mute.Reset();
		LPF_0.Reset();
		LPF_1.Reset();
		m_input = m_output = m_delay = 0;
		Sub_Process.Reset();
	}
};
#endif
