#include "main_cpp.h"
#include 	"Audio.h"
#include 	"Board.h"
#include 	"Myutil.h"
#include 	"hal.h"
#include 	"VCP_mini.h"
#include	"main.h"
#include	"Menu.h"
#include	"Port.h"
#include	"FX_Process.h"
#include	"FX_Terminal.h"

bool	Use_Dry_Signal = true;
Port	Dry_Signal_Pin( _PC15_ );

extern "C" void main_cpp()
{
	Use_Dry_Signal = Dry_Signal_Pin.Read();

	Menu_Initial();

	Select_Menu( 1 );

	Audio::Start();

	println( "Blue Pill Sound FX Projects - BPFX1\n" );

	Print_Usage();

	HAL_Delay( 1000 );

	Audio::Active();

	while( 1 )
	{
		Get_Terminal_Value();
	}
}
