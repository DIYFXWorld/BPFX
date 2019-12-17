#ifndef FX_Reverb_h
#define FX_Reverb_h

#include	"Audio.h"
#include	"Q15T_BQF.h"
#include	"Common.h"
#include 	"Reverb_APF.h"
#include 	"Reverb_Comb.h"
#include 	"FX_Interface.h"
#include 	"Volume.h"
#include 	"Potentiometer_Curve.h"

#define	FX_REVERB_FS_RATIO	2

const int		Num_Combs				= 8;
const int		Num_APFs				= 4;

const Q15T	muted					( 0.f );
const Q15T	Fixed_Gain		( 0.04f );
const Q15T 	Scale_Wet			( 3.f );
const Q15T 	Scale_Damp		( 0.4f );
const Q15T 	Scale_Room		( 0.28f );
const Q15T 	Offset_Room		( 0.7f );
const Q15T	Initial_Room	( 0.5f );
const Q15T 	Initial_Damp	( 0.2f );
const Q15T 	Initial_Wet		( Q15T( 1 ) / Scale_Wet );
const Q15T	Initial_Width	( 1.f );
const Q15T	Initial_Mode	( 0.f );
const Q15T	Freeze_Mode		( 0.5f );

#define	__MS_2_LENGTH( ms )		_MS_2_LENGTH( ms, 40000 / 2 )

const int Comb_Tuning_1		= __MS_2_LENGTH( 25.3061f ); // 1116;
const int Comb_Tuning_2		= __MS_2_LENGTH( 26.9388f ); // 1188;
const int Comb_Tuning_3		= __MS_2_LENGTH( 28.9569f ); // 1277;
const int Comb_Tuning_4		= __MS_2_LENGTH( 30.7483f ); // 1356;
const int Comb_Tuning_5		= __MS_2_LENGTH( 32.2449f ); // 1422;
const int Comb_Tuning_6		= __MS_2_LENGTH( 33.8095f ); // 1491;
const int Comb_Tuning_7		= __MS_2_LENGTH( 35.3061f ); // 1557;
const int Comb_Tuning_8		= __MS_2_LENGTH( 36.6667f ); // 1617;
const int APF_Tuning_1		= __MS_2_LENGTH( 12.6077f ); // 556;
const int APF_Tuning_2		= __MS_2_LENGTH( 10.0000f ); // 441;
const int APF_Tuning_3		= __MS_2_LENGTH( 7.73243f ); // 341;
const int APF_Tuning_4		= __MS_2_LENGTH( 5.10204f ); // 225;

class FX_Reverb : public FX_Interface
{
	Q15T_BQF			LPF_0, LPF_1;

	Type_Volume<Curve_D>		Mix_Level;

	Q15T					Gain;
	Q15T					Room_Size, Room_Size_1;
	Q15T					Damp, Damp_1;
	Q15T					Wet, Wet_1, Wet_2;
	Q15T					Width;
	Q15T					Mode;

	Reverb_Comb		Combs[ Num_Combs ];
	Reverb_APF		APFs [ Num_APFs ];

	int16_t				Buf_Comb_1[ Comb_Tuning_1 ];
	int16_t				Buf_Comb_2[ Comb_Tuning_2 ];
	int16_t				Buf_Comb_3[ Comb_Tuning_3 ];
	int16_t				Buf_Comb_4[ Comb_Tuning_4 ];
	int16_t				Buf_Comb_5[ Comb_Tuning_5 ];
	int16_t				Buf_Comb_6[ Comb_Tuning_6 ];
	int16_t				Buf_Comb_7[ Comb_Tuning_7 ];
	int16_t				Buf_Comb_8[ Comb_Tuning_8 ];

	int16_t				Buf_APF_1[ APF_Tuning_1 ];
	int16_t				Buf_APF_2[ APF_Tuning_2 ];
	int16_t				Buf_APF_3[ APF_Tuning_3 ];
	int16_t				Buf_APF_4[ APF_Tuning_4 ];

	int	m_input, m_output;
	int	output, sum, count;

public:

	FX_Reverb():
/*		Gain( Q15T( 0 ) ),
		Room_Size( Q15T( 0 ) ),	Room_Size_1( Q15T( 0 ) ),
		Damp( Q15T( 0 ) ), Damp_1( Q15T( 0 ) ),
		Wet( Q15T( 0 ) ), Wet_1( Q15T( 0 ) ), Wet_2( Q15T( 0 ) ),
		Width( Q15T( 0 ) ), Mode( Q15T( 0 ) ),*/
		m_input( 0 ), m_output( 0 ), output( 0 ), sum( 0 ), count( 0 )
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

		Set_Wet( Initial_Wet );
		Set_Room_Size( Initial_Room );
		Set_Damp( Initial_Damp );
		Set_Width( Initial_Width );
		Set_Mode( Initial_Mode );

		Mute();

		Set_Room_Size( Q15T( 0.8f ) );
		Set_Wet( Q15T( 0.99f ) );

		LPF_0 = BQF_Builder( Audio::Sampling_Rate ).LPF( 10000.f, 0.75f );
		LPF_1 = BQF_Builder( Audio::Sampling_Rate ).LPF( 10000.f, 0.75f );
	}

	void Destroy(){ delete this; }

	void Mute()
	{
		if( Get_Mode() >= Freeze_Mode )		return;

		for( int i( 0 ); i < Num_Combs; ++i )	Combs[ i ].Mute();
		for( int i( 0 ); i < Num_APFs; ++i )	APFs[ i ].Mute();
	}

	void Set_Room_Size( const Q15T& v ) { Room_Size = Mul_64( v, Scale_Room ) + Offset_Room; Update(); }
	Q15T Get_Room_Size() const					{ return Div_64( ( Room_Size - Offset_Room ), Scale_Room ); }

	void Set_Damp( const Q15T& v )			{ Damp = Mul_64( v, Scale_Damp ); Update(); }
	Q15T Get_Damp() const								{ return Div_64( Damp, Scale_Damp ); }

	void Set_Wet( const Q15T& v )				{ Wet = Mul_64( v, Scale_Wet ); Update(); }
	Q15T Get_Wet() const								{ return Div_64( Wet, Scale_Wet ); }

	void Set_Width( const Q15T& v )			{ Width = v; Update(); }
	Q15T Get_Width() const							{ return Width; }

	void Set_Mode( const Q15T& v )			{ Mode = v; Update(); }
	Q15T Get_Mode() const								{ return ( Mode >= Freeze_Mode ) ? Q15T( 1 ) : Q15T( 0 ); }

	void Process_0()
	{
		m_input = ( Gain * m_input ).to_int();
		m_output	=  0;

		m_output += Combs[ 0 ].Process( m_input );
		m_output += Combs[ 1 ].Process( m_input );
		m_output += Combs[ 2 ].Process( m_input );
		m_output += Combs[ 3 ].Process( m_input );
		m_output += Combs[ 4 ].Process( m_input );
		m_output += Combs[ 5 ].Process( m_input );
	}

	void Process_1()
	{
		m_output += Combs[ 6 ].Process( m_input );
		m_output += Combs[ 7 ].Process( m_input );

		m_output  = APFs [ 0 ].Process( m_output );
		m_output  = APFs [ 1 ].Process( m_output );
		m_output  = APFs [ 2 ].Process( m_output );
		m_output  = APFs [ 3 ].Process( m_output );

//		m_output = ( Wet_1 * m_output ).to_int();

		m_output = Mix_Level.Per( m_output );

		m_output = LIMIT_INT16( m_output );
	}

	int Process( int input )
	{
		input = LPF_0.Process( input );

		if( count < FX_REVERB_FS_RATIO-1 )
		{
			Process_0();
			sum = input;
			++count;
		}
		else
		{
			Process_1();
			sum += input;
			m_input = sum / FX_REVERB_FS_RATIO;
			output = m_output;
			count = 0;
		}

		output = LPF_1.Process( output );

		return output;
	}

private:

	void Update()
	{
		Wet_1 = Wet * ( Width / 2 + Q15T( 0.5f ) );
		Wet_2 = Wet * ( ( Q15T( 1 )-Width ) / 2 );

		if( Mode >= Freeze_Mode )
		{
			Room_Size_1 = Q15T( 1 );
			Damp_1			= Q15T( 0 );
			Gain				= muted;
		}
		else
		{
			Room_Size_1	= Room_Size;
			Damp_1			= Damp;
			Gain				= Fixed_Gain;
		}

		for( int i( 0 ); i < Num_Combs; ++i )	Combs[ i ].Set_Feedback( Room_Size_1 );
		for( int i( 0 ); i < Num_Combs; ++i )	Combs[ i ].Set_Damp( Damp_1 );
	}

public :

	virtual void Set_Control_0( int v )
	{
//		Q15T	w;
//		w.Value = Map( v, 0, 4095, 0, 32767 );
//		Set_Wet( w );
		Mix_Level.Set( v );
	}

	virtual int  Get_Control_0() const
	{
//		Q15T	w = Get_Wet();
//		int	v = Map( w.Value, 0, 32767, 0, 4095 );
//		return v;
		return Mix_Level.Initial_Value;
	}

	virtual void Set_Control_1( int )		{}
	virtual int  Get_Control_1() const	{ return 0; }

	virtual void Set_Control_2( int v )
	{
		Q15T	w;
		w.Value = Map( v, 0, 4095, 0, 32767 );
		Set_Room_Size( w );
	}

	virtual int  Get_Control_2() const
	{
		Q15T	w = Get_Room_Size();
		int	v = Map( w.Value, 0, 32767, 0, 4095 );
		return v;
	}

	virtual void Set_Control_3( int )		{}
	virtual int  Get_Control_3() const	{ return 0; }

	virtual void Set_Control_4( int )		{}
	virtual int  Get_Control_4() const	{ return 0; }

	virtual int Get_Memory_Length() const	{ return 0; }

	virtual enum_FX_ID Get_ID() const		{ return FX_ID_Reverb; }
};

#endif
