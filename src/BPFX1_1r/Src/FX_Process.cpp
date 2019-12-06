#include <Audio.h>
#include <hal.h>
#include "FX_Process.h"
#include "main_cpp.h"

FX_Interface		*FX;

void Audio_Input_Callback( int input )
{
	int	output = FX->Process( input );
	if( Use_Dry_Signal )	output += input;
	Audio::Set_Output( output );
}

#include <FX_Analog_Delay.h>
#include <FX_Delay.h>
#include <FX_Modulation_Delay.h>
#include <FX_Reverse_Delay.h>
#include <FX_Chorus.h>
#include <FX_Flanger.h>
#include <FX_None.h>
#include <FX_Phaser_II.h>
#include <FX_Reverb.h>
#include <FX_Two_Tap_Delay.h>
#include <FX_Delay_ADPCM.h>
#include <FX_Delay_PCMU.h>
#include <FX_Reverse_Delay_PCMU.h>
#include <FX_Delay_0.h>

template	<typename T>
void Construct()
{
	Audio::Inactive();
	FX->Destroy();
	FX = new T;
	Audio::Active();
}

extern "C" void Error_Handler();

void Select_FX( FX_ID v )
{
	if( !FX )	FX = new FX_None;

	if( v != FX->Get_FX_ID() )
	{
		     if( v == FX_ID_None )								Construct<FX_None>();
		else if( v == FX_ID_Delay )								Construct<FX_Delay>();
		else if( v == FX_ID_Analog_Delay )				Construct<FX_Analog_Delay>();
		else if( v == FX_ID_Reverse_Delay )				Construct<FX_Reverse_Delay>();
		else if( v == FX_ID_Modulation_Delay )		Construct<FX_Modulation_Delay>();
		else if( v == FX_ID_Chorus )							Construct<FX_Chorus>();
		else if( v == FX_ID_Phaser )							Construct<FX_Phaser_II>();
		else if( v == FX_ID_Flanger )							Construct<FX_Flanger>();
		else if( v == FX_ID_Reverb )							Construct<FX_Reverb>();
		else if( v == FX_ID_Two_Tap_Delay )				Construct<FX_Two_Tap_Delay>();
		else if( v == FX_ID_Delay_ADPCM )					Construct<FX_Delay_ADPCM>();
		else if( v == FX_ID_Delay_PCMU )					Construct<FX_Delay_PCMU>();
		else if( v == FX_ID_Reverse_Delay_PCMU )	Construct<FX_Reverse_Delay_PCMU>();
		else if( v == FX_ID_Delay_0 )							Construct<FX_Delay_0>();
		else Error_Handler();
	}
}
