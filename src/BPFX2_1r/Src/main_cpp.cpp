#include "main_cpp.h"
#include "Audio.h"
#include "Board.h"
#include "Myutil.h"
#include "hal.h"
#include "VCP_mini.h"
#include "main.h"
#include "Port.h"
#include "FX_Terminal.h"
#include "Menu.h"
#include "Memory_Pool.h"

FX_Interface		*FX;

void Audio_Input_Callback( int v )
{
	Audio::Set_Output( v + FX->Process( v ) );
}

extern "C" void main_cpp()
{
	Menu_Initial();

	FX = Menu.Select( 1 );

	Audio::Start();

	println( "Blue Pill Sound FX Projects - BPFX Part 2\n" );

	Print_Usage();

	HAL_Delay( 1000 );

	Audio::Active();

	while( 1 )
	{
		Get_Terminal_String();
	}
}
