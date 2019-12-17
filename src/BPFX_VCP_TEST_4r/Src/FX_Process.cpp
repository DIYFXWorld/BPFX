/*
 * FX_Process.cpp
 *
 *  Created on: 2019/03/19
 *      Author: gamenyge
 */
#include	"hal.h"
#include	"Audio.h"
#include	"FX_Process.h"

FX_Interface					*FX;

void Audio_Input_Callback( int input )
{
	Audio::Set_Output( input + FX->Process( input ) );
}

void Audio_Invalid_Callback( int )
{
	Audio::Set_Output( 0 );
}

#include <FX_Analog_Delay.h>
#include <FX_Delay.h>
#include <FX_Modulation_Delay.h>
#include <FX_Reverse_Delay.h>
#include <FX_Chorus.h>
#include <FX_Flanger.h>
#include <FX_None.h>
#include <FX_Phaser_II.h>
#include <FX_Tremolo.h>
#include <FX_Ring_Modulation.h>
#include <FX_Reverb.h>

template	<typename T>
void Construct()
{
	Audio::Input_Active = false;
	FX->Destroy();
	FX = new T;
	Audio::Input_Active = true;
}

void Select_FX( enum_FX_ID v )
{
	if( !FX )	FX = new FX_None;

	if( v != FX->Get_ID() )
	{
		if( v == FX_ID_None )							Construct<FX_None>();
		if( v == FX_ID_Delay )						Construct<FX_Delay>();
		if( v == FX_ID_Analog_Delay )			Construct<FX_Analog_Delay>();
		if( v == FX_ID_Reverse_Delay )		Construct<FX_Reverse_Delay>();
		if( v == FX_ID_Modulation_Delay )	Construct<FX_Modulation_Delay>();
		if( v == FX_ID_Chorus )						Construct<FX_Chorus>();
		if( v == FX_ID_Phaser )						Construct<FX_Phaser_II>();
		if( v == FX_ID_Flanger )					Construct<FX_Flanger>();
		if( v == FX_ID_Tremolo )					Construct<FX_Tremolo>();
		if( v == FX_ID_Ring_Modulation )	Construct<FX_Ring_Modulation>();
		if( v == FX_ID_Reverb )						Construct<FX_Reverb>();
	}
}
