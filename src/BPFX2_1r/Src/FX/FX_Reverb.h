#ifndef FX_Reverb_h
#define FX_Reverb_h

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Reverb_APF.h>
#include <Reverb_Comb.h>

const Q15T	muted					( 0.f );
const Q15T	Fixed_Gain		( 0.04f );
const Q15T 	Scale_Damp		( 0.4f );
const Q15T 	Scale_Room		( 0.28f );
const Q15T 	Offset_Room		( 0.7f );
const Q15T	Initial_Room	( 0.5f );
const Q15T 	Initial_Damp	( 0.1f );

constexpr Q15T_BQF_Param FX_Reverb_LPF_Param	= BQF_Builder( _FS_ ).LPF( 10000.f, 0.75f );
constexpr Q15T_BQF_Param FX_Reverb_Dip_Param	= BQF_Builder( _FS_/2 ).PK( 450.f, 0.75f, -10 );

class FX_Reverb : public FX_Interface
{
	static const int		Num_Combs			= 8;
	static const int		Num_APFs			= 4;

	static const int Comb_Tuning_1		= _MS_2_LENGTH( 25.3061f, _FS_/2 ); // 1116;
	static const int Comb_Tuning_2		= _MS_2_LENGTH( 26.9388f, _FS_/2 ); // 1188;
	static const int Comb_Tuning_3		= _MS_2_LENGTH( 28.9569f, _FS_/2 ); // 1277;
	static const int Comb_Tuning_4		= _MS_2_LENGTH( 30.7483f, _FS_/2 ); // 1356;
	static const int Comb_Tuning_5		= _MS_2_LENGTH( 32.2449f, _FS_/2 ); // 1422;
	static const int Comb_Tuning_6		= _MS_2_LENGTH( 33.8095f, _FS_/2 ); // 1491;
	static const int Comb_Tuning_7		= _MS_2_LENGTH( 35.3061f, _FS_/2 ); // 1557;
	static const int Comb_Tuning_8		= _MS_2_LENGTH( 36.6667f, _FS_/2 ); // 1617;
	static const int APF_Tuning_1			= _MS_2_LENGTH( 12.6077f, _FS_/2 ); // 556;
	static const int APF_Tuning_2			= _MS_2_LENGTH( 10.0000f, _FS_/2 ); // 441;
	static const int APF_Tuning_3			= _MS_2_LENGTH( 7.73243f, _FS_/2 ); // 341;
	static const int APF_Tuning_4			= _MS_2_LENGTH( 5.10204f, _FS_/2 ); // 225;

	Volume<Curve_B>	Mix_Level;
	Volume<Curve_C>	Room_Size;

	Q15T						Gain;
	Q15T						Damp;

	Reverb_Comb			Combs[ Num_Combs ];
	Reverb_APF			APFs [ Num_APFs ];

	int16_t					Buf_Comb_1[ Comb_Tuning_1 ];
	int16_t					Buf_Comb_2[ Comb_Tuning_2 ];
	int16_t					Buf_Comb_3[ Comb_Tuning_3 ];
	int16_t					Buf_Comb_4[ Comb_Tuning_4 ];
	int16_t					Buf_Comb_5[ Comb_Tuning_5 ];
	int16_t					Buf_Comb_6[ Comb_Tuning_6 ];
	int16_t					Buf_Comb_7[ Comb_Tuning_7 ];
	int16_t					Buf_Comb_8[ Comb_Tuning_8 ];

	int16_t					Buf_APF_1[ APF_Tuning_1 ];
	int16_t					Buf_APF_2[ APF_Tuning_2 ];
	int16_t					Buf_APF_3[ APF_Tuning_3 ];
	int16_t					Buf_APF_4[ APF_Tuning_4 ];

	Q15T_BQF				LPF_0, LPF_1;
	Q15T_BQF				Dip;

	Sub_Process_2<FX_Reverb>	Sub_Process;
	int	m_input, m_output;
	int	sum, count, last;

public:

	FX_Reverb():
		Gain( Fixed_Gain ),
		Sub_Process( this ),
		m_input( 0 ), m_output( 0 )
	{
		Combs[ 0 ].Set_Buffer( Buf_Comb_1 );
		Combs[ 1 ].Set_Buffer( Buf_Comb_2 );
		Combs[ 2 ].Set_Buffer( Buf_Comb_3 );
		Combs[ 3 ].Set_Buffer( Buf_Comb_4 );
		Combs[ 4 ].Set_Buffer( Buf_Comb_5 );
		Combs[ 5 ].Set_Buffer( Buf_Comb_6 );
		Combs[ 6 ].Set_Buffer( Buf_Comb_7 );
		Combs[ 7 ].Set_Buffer( Buf_Comb_8 );

		APFs[ 0 ].Set_Buffer( Buf_APF_1 );
		APFs[ 1 ].Set_Buffer( Buf_APF_2 );
		APFs[ 2 ].Set_Buffer( Buf_APF_3 );
		APFs[ 3 ].Set_Buffer( Buf_APF_4 );

		Set_Room_Size( Initial_Room );
		Set_Damp( Initial_Damp );
		Set_Room_Size( Q15T( 0.8f ) );

		for( int i( 0 ); i < Num_Combs; ++i )	Combs[ i ].Clear();
		for( int i( 0 ); i < Num_APFs; ++i )	APFs[ i ].Clear();

		LPF_0 = FX_Reverb_LPF_Param;
		LPF_1 = FX_Reverb_LPF_Param;
		Dip   = FX_Reverb_Dip_Param;
	}

	void Destroy(){ delete this; }

	void Set_Room_Size( const Q15T& v )
	{
		Q15T w = Mul_64( v, Scale_Room ) + Offset_Room;
		Room_Size.Set_Value( w.Value >> 3 );
	}

	Q15T Get_Room_Size() const
	{
		Q15T	v;
		v.Value = Room_Size.Initial_Value << 3;
		return Div_64( ( v - Offset_Room ), Scale_Room );
	}

	void Set_Damp( const Q15T& v )
	{
		Damp = Mul_64( v, Scale_Damp );
		for( int i( 0 ); i < Num_Combs; ++i )	Combs[ i ].Set_Damp( Damp );
	}

	Q15T Get_Damp() const
	{
		return Div_64( Damp, Scale_Damp );
	}

	void Sub_Process_0( int input )
	{
		m_input = input;

		{
			Q15T	v;
			v.Value = Room_Size.Get_Value() << 3;
			for( int i( 0 ); i < Num_Combs; ++i )
				Combs[ i ].Set_Feedback( v );
		}

		m_input		= Dip.Process( m_input );
		m_input 	= ( Gain * m_input ).to_int();
		m_output	=  0;

		m_output += Combs[ 0 ].Process( m_input );
		m_output += Combs[ 1 ].Process( m_input );
		m_output += Combs[ 2 ].Process( m_input );
	}

	int Sub_Process_1()
	{
		m_output += Combs[ 3 ].Process( m_input );
		m_output += Combs[ 4 ].Process( m_input );
		m_output += Combs[ 5 ].Process( m_input );
		m_output += Combs[ 6 ].Process( m_input );
		m_output += Combs[ 7 ].Process( m_input );

		m_output  = APFs [ 0 ].Process( m_output );
		m_output  = APFs [ 1 ].Process( m_output );
		m_output  = APFs [ 2 ].Process( m_output );
		m_output  = APFs [ 3 ].Process( m_output );

		m_output = LIMIT_INT16( m_output );

		m_output = Mix_Level.Per( m_output );

		return m_output;
	}

	int Process( int input )
	{
		input = LPF_0.Process( input );

		int output = Sub_Process.Do( input );

		output = LPF_1.Process( output );

		return output;
	}

	virtual void Set_Param_0( int v ) { Mix_Level.Set_Value( v );	}
	virtual int  Get_Param_0() const	{ return Mix_Level.Initial_Value; }

	virtual void Set_Param_2( int v )	{ Set_Room_Size( Q15T( Q15T_Raw_Value( Map( v, 0, UINT12_MAX, 0, INT16_MAX ) ) ) ); }
	virtual int  Get_Param_2() const	{ return Map( Get_Room_Size().Value, 0, INT16_MAX, 0, UINT12_MAX ); }

	virtual FX_ID Get_FX_ID() const		{ return FX_ID_Reverb; }

	void Reset()
	{
		Mix_Level.Fast_Forward();
		Room_Size.Fast_Forward();

		LPF_0.Reset();
		LPF_1.Reset();
		Dip.Reset();

		for( int i( 0 ); i < Num_Combs; ++i )
			Combs[ i ].Clear();

		for( int i( 0 ); i < Num_APFs; ++i )
			APFs[ i ].Clear();

		m_input = m_output = sum = count = last = 0;

		Sub_Process.Reset();
	}
};

#endif
