/*
 * FX_Delay.h
 *
 *  Created on: 2018/04/06
 *      Author: gamenyge
 */
#ifndef	FX_Analog_Delay_h
#define	FX_Analog_Delay_h

#include	"hal.h"
#include	"Common.h"
#include	"Volume.h"
#include	"Analog_Delay_Buffer.h"
#include	"Q15T_BQF.h"
#include	"Audio.h"
#include	"FX_Interface.h"
#include	"Potentiometer_Curve.h"

#define		FX_ANALOG_DELAY_FS_RATIO	4
#define		FX_ANALOG_DELAY_BUFFER_LENGTH	8000

struct FX_Analog_Delay : public FX_Interface
{
	int											Time_Length;	// 0...max buffer length
	Type_Volume<Curve_B>		Feedback;			// 0...4095
	Type_Volume<Curve_B>		Mix_Level;		// 0...4095

	Analog_Delay_Buffer			Buffer;

	Q15T_BQF								LPF_0, LPF_1;

	FX_Analog_Delay()
	: Time_Length( 0 ), Feedback( 0 ), Mix_Level( 0 ),
		Buffer( FX_ANALOG_DELAY_BUFFER_LENGTH )
	{
		LPF_0 = BQF_Builder( Audio::Sampling_Rate ).LPF( 2500.f, 0.75f );
		LPF_1 = BQF_Builder( Audio::Sampling_Rate ).LPF( 2500.f, 0.75f );
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

		Buffer.Set( input );

		output = LIMIT_INT16( output );

		return output;
	}

	int Process( int input )
	{
		input = LPF_0.Process( input );

		int	output;
		static int	count, last, value;

		if( count < FX_ANALOG_DELAY_FS_RATIO-1 )
		{
			value += input;
			output = last;
			++count;
		}
		else
		{
			value += input;
			output = last = _Process( value / FX_ANALOG_DELAY_FS_RATIO );
			count = value = 0;
		}

		output = LPF_1.Process( output );

		return output;
	}

	void Set_Control_0( int v )	{ Feedback.Set( v ); }
	int Get_Control_0() const		{ return Feedback.Initial_Value; }

	void Set_Control_1( int v ) { Mix_Level.Set( v ); }
	int Get_Control_1() const		{ return Mix_Level.Initial_Value; }

	void Set_Control_2( int v )
	{
		v = Map( v, 0, UINT12_MAX, 1, Buffer.Memory.Length-1 );
		Time_Length = v;
	}

	void Set_Control_3( int )	{}
	int Get_Control_3() const	{ return 0;}

	void Set_Control_4( int )	{}
	int Get_Control_4() const	{ return 0;}

	int Get_Control_2() const
	{
		return Map( Time_Length, 1, Buffer.Memory.Length-1, 0, UINT12_MAX );
	}

	int Get_Memory_Length() const { return Buffer.Memory.Length; }

	enum_FX_ID Get_ID() const	{ return FX_ID_Analog_Delay; }
};

#endif
