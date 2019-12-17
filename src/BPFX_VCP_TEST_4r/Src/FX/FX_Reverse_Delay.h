#ifndef	FX_Reverse_Delay_h
#define	FX_Reverse_Delay_h

#include	"hal.h"
#include	"Array.h"
#include	"Reverse_Delay_Buffer.h"
#include	"Volume.h"
#include	"Mute.h"
#include	"FX_Interface.h"
#include	"Potentiometer_Curve.h"
#include	"Common.h"
#include	"Q15T_BQF.h"

#define		FX_REVERSE_DELAY_FS_RATIO	2
#define		FX_REVERSE_DELAY_BUFFER_LENGTH	7800

struct FX_Reverse_Delay : public FX_Interface
{
	Type_Volume<Curve_B>		Time_Length;
	Type_Volume<Curve_B>		Feedback;
	Type_Volume<Curve_B>		Mix_Level;

	Reverse_Delay_Buffer		Buffer;
	Type_Mute								Mute;

	Q15T_BQF								LPF_0, LPF_1;

	FX_Reverse_Delay()
	:	Time_Length( 1 ), Feedback( 0 ), Mix_Level( 0 ),
		Buffer( FX_REVERSE_DELAY_BUFFER_LENGTH ), Mute( 40 )
	{
		LPF_0 = BQF_Builder( Audio::Sampling_Rate ).LPF( 10000.f, 0.75f );
		LPF_1 = BQF_Builder( Audio::Sampling_Rate ).LPF( 10000.f, 0.75f );
	}

	void Destroy() { delete this; }

	int _Process( int Input )
	{
		int	Output, Delay;

		Buffer.Set_Length( Time_Length.Get() );

		if( Buffer.Pointer == Buffer.Length - Mute.Length + 1 )
			if( Buffer.Direction > 0 )
				Mute.Start();

		if( Buffer.Pointer == Mute.Length - 2 )
			if( Buffer.Direction < 0 )
				Mute.Start();

		if( Reset )
		{
			Buffer.Memory.Clear();
			Buffer.Reset();
			Reset = false;
		}

		Delay = Buffer.Get();
		Delay = Mute.Process( Delay );

		Input	-= Feedback.Per( Delay );
		Output = Mix_Level.Per( Delay );

		Input  = LIMIT_INT16( Input );
		Output = LIMIT_INT16( Output );

		Buffer.Set( Input );

		return Output;
	}

	int Process( int input )
	{
		input = LPF_0.Process( input );

		int	output;
		static int	count, last, value;

		if( count < FX_REVERSE_DELAY_FS_RATIO-1 )
		{
			value += input;
			output = last;
			++count;
		}
		else
		{
			value += input;
			output = last = _Process( value / FX_REVERSE_DELAY_FS_RATIO );
			count = value = 0;
		}

		output = LPF_1.Process( output );

		return output;
	}

	void Set_Control_0( int v )	{ Feedback.Set( v ); }
	int Get_Control_0() const	{ return Feedback.Expected_Value; }

	void Set_Control_1( int v ) { Mix_Level.Set( v ); }
	int Get_Control_1() const	{ return Mix_Level.Expected_Value; }

	void Set_Control_2( int v )
	{
		v = Map( v, 0, 4095, 1000, Buffer.Memory.Length-1 );
		Time_Length.Set( v );
	}

	int Get_Control_2() const
	{
		return Map( Time_Length.Expected_Value, 1000, Buffer.Memory.Length-1, 0, 4095 );
	}

	void Set_Control_3( int )	{}
	int Get_Control_3() const	{ return 0;}

	void Set_Control_4( int )	{}
	int Get_Control_4() const	{ return 0;}

	int Get_Memory_Length() const { return Buffer.Memory.Length; }

	enum_FX_ID Get_ID() const	{ return FX_ID_Reverse_Delay; }
};
#endif
