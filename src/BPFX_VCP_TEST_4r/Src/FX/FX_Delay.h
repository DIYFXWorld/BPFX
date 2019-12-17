/*
 * FX_Delay.h
 *
 *  Created on: 2018/04/06
 *      Author: gamenyge
 */
#ifndef	FX_Delay_h
#define	FX_Delay_h

#include	"hal.h"
#include	"Common.h"
#include	"Volume.h"
#include	"Potentiometer_Curve.h"
#include	"Delay_Buffer.h"
#include	"Audio.h"
#include	"FX_Interface.h"
#include	"Q15T_BQF.h"

#define		FX_DELAY_FS_RATIO	2
#define		FX_DELAY_BUFFER_LENGTH	8000

struct FX_Delay : public FX_Interface
{
	int										Time_Length;	// 0...max buffer length
	Type_Volume<Curve_B>	Feedback;			// 0...4095
	Type_Volume<Curve_B>	Mix_Level;		// 0...4095

	Delay_Buffer					Buffer;

	Q15T_BQF							LPF_0, LPF_1;

	FX_Delay() :
		Time_Length( 0 ), Feedback( 0 ), Mix_Level( 0 ),
		Buffer( FX_DELAY_BUFFER_LENGTH )
	{
#define	FREQ	10000.f
#define	Q			0.75f
		LPF_0 = BQF_Builder( Audio::Sampling_Rate ).LPF( FREQ, Q );
		LPF_1 = BQF_Builder( Audio::Sampling_Rate ).LPF( FREQ, Q );
	}

	void Destroy() { delete this; }

	int _Process( int input )
	{
		int	output, delay;

		Buffer.Set_Length( Time_Length );

		if( Reset )
		{
			Buffer.Memory.Clear();
			Buffer.Reset();
			Reset = false;
		}

		delay = Buffer.Get();

		input	-= Feedback.Per( delay );
		output = Mix_Level.Per( delay );

		input  = LIMIT_INT16( input );
		output = LIMIT_INT16( output );

		Buffer.Set( input );

		return output;
	}

	int Process( int input )
	{
		input = LPF_0.Process( input );

		int	output;
		static int	count, last, value;

		if( count < FX_DELAY_FS_RATIO-1 )
		{
			value += input;
			output = last;
			++count;
		}
		else
		{
			value += input;
			output = last = _Process( value / FX_DELAY_FS_RATIO );
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
		v = Map( v, 0, 4095, 1, Buffer.Memory.Length-1 );
		Time_Length = v;
	}

	int Get_Control_2() const
	{
		return Map( Time_Length, 1, Buffer.Memory.Length-1, 0, 4095 );
	}

	void Set_Control_3( int )	{}
	int Get_Control_3() const	{ return 0;}

	void Set_Control_4( int )	{}
	int Get_Control_4() const	{ return 0;}

	int Get_Memory_Length() const { return Buffer.Memory.Length; }

	enum_FX_ID Get_ID() const	{ return FX_ID_Delay; }
};

#endif
