/*
 * sub_main.cpp
 *
 *  Created on: 2017/04/30
 *      Author: gamenyge
 */

#include	"main.h"
#include	"hal.h"
#include	"Audio.h"
#include	"VCP.h"
#include	"Common.h"
#include	"Board.h"
#include	"main_cpp.h"
#include	"Line_Editor.h"
#include	"FX_Terminal.h"
#include	"FX_Process.h"
#include	"Menu.h"

extern "C" void main_cpp()
{
	Menu_Initial();

	Select_Menu( 1 );

	Audio::Start();

	printf( "Blue Pill Sound FX Project - VCP Test\n\n" );

	Print_Usage();

	Audio::Input_Active = true;

	while( 1 )
	{
		Get_Terminal_Value();
	}
}
